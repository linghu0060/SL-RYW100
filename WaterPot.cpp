// WaterPot.cpp : 实现文件
//

#include "stdafx.h"
#include "CMemoryDC.h"
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
    CPaintDC    PaintDC(this);      // device context for painting
    CMemoryDC   dc(&PaintDC);       // memory device context

    // get colors to use
    COLORREF crBarColor    = (COLORREF)( (m_crBarClr    == CLR_DEFAULT) ? ::GetSysColor(COLOR_HIGHLIGHT)  : m_crBarClr    );
    COLORREF crBarBkColor  = (COLORREF)( (m_crBarBkClr  == CLR_DEFAULT) ? ::GetSysColor(COLOR_BTNFACE)    : m_crBarBkClr  );
    COLORREF crTextColor   = (COLORREF)( (m_crTextClr   == CLR_DEFAULT) ? ::GetSysColor(COLOR_WINDOWTEXT) : m_crTextClr   );
    COLORREF crTextBkColor = (COLORREF)( (m_crTextBkClr == CLR_DEFAULT) ? ::GetSysColor(COLOR_WINDOW)     : m_crTextBkClr );

    double dFraction = m_fPos / m_fHeight;

    // select progress bar font if there is one, else select parent font
    if( GetFont() )
        dc.SelectObject(GetFont());
    else
        dc.SelectObject(GetParent()->GetFont());

    // 擦除
    CRect wndRect;
    GetWindowRect(&wndRect);  ScreenToClient(&wndRect);
    FillRect(dc, wndRect, (HBRUSH)(GetClassLong(m_hWnd, GCL_HBRBACKGROUND)));

    // 绘制边框
    CRect botRect, topRect, ClientRect;
    GetClientRect(&ClientRect);
    botRect = topRect = ClientRect;
//  DrawEdge(dc, ClientRect, EDGE_SUNKEN, BF_ADJUST | BF_RECT | BF_FLAT);

    // 绘制液位
    botRect.top = botRect.bottom - (int)((botRect.Height()) * dFraction);
    topRect.bottom = botRect.top;
    dc.FillSolidRect(botRect, crBarColor);
    dc.FillSolidRect(topRect, crBarBkColor);

    // draw text if needed
    CString str;  GetWindowText(str);
    if(m_bShowPercent) {
        str.AppendFormat(_T("%.2f/%.2f(%d%%)"), m_fPos, m_fHeight, (int)((dFraction * 100.0) + 0.5));
    }
    if( str.GetLength() )
    {
        CSize szText = dc.GetOutputTextExtent(str);
        CPoint ptStart;
        if( topRect.Height() >= szText.cy ) {
            ptStart.y = topRect.bottom - szText.cy;
        }
        else if( botRect.Height() >= szText.cy ) {
            ptStart.y = botRect.top;
        }
        else {
            ptStart.y = ClientRect.top + ((ClientRect.Height() - szText.cy) / 2);
        }
        ptStart.x = ClientRect.left + ((ClientRect.Width() - szText.cx) / 2);

		CRgn rgn;
		rgn.CreateRectRgn(botRect.left, botRect.top, botRect.right, botRect.bottom);
		dc.SelectClipRgn(&rgn);
		dc.SetTextColor(crTextBkColor);
		dc.ExtTextOut(ptStart.x, ptStart.y, ETO_CLIPPED, &ClientRect, str, NULL);
		rgn.DeleteObject();

		rgn.CreateRectRgn(topRect.left, topRect.top, topRect.right, topRect.bottom);
		dc.SelectClipRgn(&rgn);
		dc.SetTextColor(crTextColor);
		dc.ExtTextOut(ptStart.x, ptStart.y, ETO_CLIPPED, &ClientRect, str, NULL);
		rgn.DeleteObject();
    }

    if( m_Image != NULL ) { 
        if( m_Image->IsNull() != TRUE ) { 
            m_Image->Draw(dc.GetSafeHdc(), wndRect);
        }
    }
}



