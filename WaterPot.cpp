// WaterPot.cpp : 实现文件
//

#include "stdafx.h"
#include "WaterPot.h"

////////////////////////////////////////////////////////////////////////////////
// CWaterPot

IMPLEMENT_DYNAMIC(CWaterPot, CWnd)

BEGIN_MESSAGE_MAP(CWaterPot, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_MESSAGE(PBM_SETBARCOLOR, OnSetBarColor)
    ON_MESSAGE(PBM_SETBKCOLOR, OnSetBarBkColor)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////

CWaterPot::CWaterPot()
{
    // default colors
    m_crBarClr    = CLR_DEFAULT;
    m_crBarBkClr  = CLR_DEFAULT;
    m_crTextClr   = CLR_DEFAULT;
    m_crTextBkClr = CLR_DEFAULT;

    // default to show percent complete
    m_bShowPercent = TRUE;

    // default water pot height, current position
    m_fHeight = 1.0f;
    m_fPos  = 0.0f;

    m_Image = NULL;

    RegisterWindowClass();
}

CWaterPot::~CWaterPot()
{
}


BOOL CWaterPot::RegisterWindowClass(HINSTANCE hInstance)
{
    if(!hInstance) hInstance = AfxGetInstanceHandle();

    WNDCLASS windowclass;
    if( !::AfxCtxGetClassInfo(hInstance, WATERPOTNAME, &windowclass) )
    {
           windowclass.style = CS_DBLCLKS;
           windowclass.lpfnWndProc = ::DefWindowProc;
           windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
           windowclass.hInstance = hInstance;
           windowclass.hIcon = NULL;
           windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
           windowclass.hbrBackground = ::GetSysColorBrush(COLOR_BTNFACE);
           windowclass.lpszMenuName  = NULL;
           windowclass.lpszClassName = WATERPOTNAME;

           if( !::AfxRegisterClass(&windowclass) )
           {
               AfxThrowResourceException();
               return FALSE;
           }
    }

    return TRUE;
}

void CWaterPot::SetImage(CImage *image)
{
    m_Image = image;

    CommonPaint();
}

void CWaterPot::SetHeight(float fHeight)
{
    m_fHeight = (fHeight == 0) ? 0.001f : fHeight;

    CommonPaint();
}

float CWaterPot::SetPos(float fPos)
{
    float fOldPos = m_fPos;

    m_fPos = (fPos > m_fHeight) ? m_fHeight : fPos;

    CommonPaint();
    return( fOldPos );
}

void CWaterPot::CommonPaint()
{
    //
    // common place to mess with redraw options
    //
    // note that Invalidate() by itself won't cause
    // a repaint when this control is used in a modeless
    // dialog box
    //
    // also note that the marquee timer won't work in a modeless
    // dialog - to use marquee mode in a modeless dialog the code
    // that is running will have to pump Windows messages
    //

    RedrawWindow();
}


////////////////////////////////////////////////////////////////////////////////
// CWaterPot 消息处理程序

LRESULT CWaterPot::OnSetBarColor(WPARAM, LPARAM crBar)
{
    // call a common routine to redraw window
    CommonPaint();

    // set new bar color, returning old one
    COLORREF crOldBarClr = m_crBarClr;
    m_crBarClr = (COLORREF)crBar;
    return ((LRESULT)crOldBarClr);
}

LRESULT CWaterPot::OnSetBarBkColor(WPARAM, LPARAM crBarBk)
{
    // call a common routine to redraw window
    CommonPaint();

    // set new bar background color, returning old one
    COLORREF crOldBarBkClr = m_crBarBkClr;
    m_crBarBkClr = (COLORREF)crBarBk;
    return ((LRESULT)crOldBarBkClr);
}

BOOL CWaterPot::OnEraseBkgnd(CDC* pDC)
{
//  return CWnd::OnEraseBkgnd(pDC);
    return TRUE;
}

void CWaterPot::OnPaint()
{
    // get colors to use
    COLORREF crBarColor    = (COLORREF)( (m_crBarClr    == CLR_DEFAULT) ? ::GetSysColor(COLOR_HIGHLIGHT)  : m_crBarClr    );
    COLORREF crBarBkColor  = (COLORREF)( (m_crBarBkClr  == CLR_DEFAULT) ? ::GetSysColor(COLOR_BTNFACE)    : m_crBarBkClr  );
    COLORREF crTextColor   = (COLORREF)( (m_crTextClr   == CLR_DEFAULT) ? ::GetSysColor(COLOR_WINDOWTEXT) : m_crTextClr   );
    COLORREF crTextBkColor = (COLORREF)( (m_crTextBkClr == CLR_DEFAULT) ? ::GetSysColor(COLOR_WINDOW)     : m_crTextBkClr );

    double  dFraction = m_fPos / m_fHeight;
    CRect   botRect, topRect, ClientRect;
    GetClientRect(&ClientRect);
    if( ClientRect.Width() > 4 ) {
        ClientRect.right -= 4;
    } else {
        ClientRect.right  = 0;
    }
    if( ClientRect.Height() > 4 ) {
        ClientRect.bottom -= 4;
    } else {
        ClientRect.bottom  = 0;
    }
    botRect = topRect = ClientRect;
    botRect.top = botRect.bottom - (int)((botRect.Height()) * dFraction);
    topRect.bottom = botRect.top;

    {   // 绘制液位显示
        CPaintDC    PaintDC(this);
        CMemDC      memDC(PaintDC, this);
        CDC        &dc = memDC.GetDC();

//      DrawEdge(dc, ClientRect, EDGE_SUNKEN, BF_ADJUST | BF_RECT | BF_FLAT);
        if( botRect.Height() && botRect.Width() ) {
            dc.FillSolidRect(botRect, crBarColor);
        }
        if( topRect.Height() && topRect.Width() ) {
            dc.FillSolidRect(topRect, crBarBkColor);
        }
    }

    {   // 绘制水罐, 显示字符
        CClientDC   PaintDC(this);
        CMemDC      memDC(PaintDC, this);
        CDC        &dc = memDC.GetDC();

        if( (m_Image != NULL) && (m_Image->IsNull() != TRUE) )
        {
            CRect  wndRect;
            GetWindowRect(&wndRect);  ScreenToClient(&wndRect);
            m_Image->Draw(dc, wndRect);
        }

        // select progress bar font if there is one, else select parent font
        if( GetFont() ) {
            dc.SelectObject(GetFont());
        } else {
            dc.SelectObject(GetParent()->GetFont());
        }

        // draw text if needed

        CString str1, str2;
        CSize   szText1, szText2;
        CPoint  ptStart1, ptStart2;
        CRgn    rgn;
        GetWindowText(str1);  if( str1.GetLength() == 0 )  str1 = _T("水位高度(米)");
        str2.Format(_T("%.2f/%.2f -- (%d%%)"), m_fPos, m_fHeight, (int)((dFraction * 100.0) + 0.5));
        szText1 = dc.GetOutputTextExtent(str1); 
        szText2 = dc.GetOutputTextExtent(str2);

        if( (botRect.Height() >= (szText1.cy + szText2.cy + 6 + 6)) ) {
            ptStart1.y = botRect.top + 6;
            ptStart2.y = ptStart1.y + szText1.cy;
        }
        else if( (topRect.Height() >= (szText1.cy + szText2.cy + 6 + 6)) ) {
            ptStart1.y = topRect.bottom - szText1.cy - szText2.cy - 6;
            ptStart2.y = ptStart1.y + szText1.cy;
        }
        else {
            ptStart1.y = ClientRect.top + ((ClientRect.Height() - (szText1.cy + szText2.cy)) / 2);
            ptStart2.y = ptStart1.y + szText1.cy;
        }
        ptStart1.x = ClientRect.left + ((ClientRect.Width() - szText1.cx) / 2);
        ptStart2.x = ClientRect.left + ((ClientRect.Width() - szText2.cx) / 2);

        rgn.CreateRectRgn(ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(crTextColor);
        dc.ExtTextOut(ptStart1.x, ptStart1.y, ETO_CLIPPED, &ClientRect, str1, NULL);
        rgn.DeleteObject();
        rgn.CreateRectRgn(ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(crTextColor);
        dc.ExtTextOut(ptStart2.x, ptStart2.y, ETO_CLIPPED, &ClientRect, str2, NULL);
        rgn.DeleteObject();

//      if( str1.GetLength() )
//      {
//          CPoint ptStart;
//          CRgn   rgn;
//          CSize  szText1 = dc.GetOutputTextExtent(str1);
// 
//          if( (botRect.Height() >= (szText1.cy + 6))/* && (botRect.Height() > (ClientRect.Height() / 4))*/ ) {
//              ptStart.y = botRect.top + 6;
//          }
//          else if( (topRect.Height() >= (szText1.cy + 6))/* && (topRect.Height() > (ClientRect.Height() / 4))*/ ) {
//              ptStart.y = topRect.bottom - (szText1.cy + 6);
//          }
//          else {
//              ptStart.y = ClientRect.top + ((ClientRect.Height() - szText1.cy) / 2);
//          }
//          ptStart.x = ClientRect.left + ((ClientRect.Width() - szText1.cx) / 2);
// 
//          rgn.CreateRectRgn(ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);
//          dc.SelectClipRgn(&rgn);
//          dc.SetTextColor(crTextColor);
//          dc.ExtTextOut(ptStart.x, ptStart.y, ETO_CLIPPED, &ClientRect, str1, NULL);
//          rgn.DeleteObject();
//      }
    }
}



