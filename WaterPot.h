#pragma once


// CWaterPot

#define WATERPOTNAME _T("CWaterPot")        // Class Name

#ifndef PBM_SETBARCOLOR
#define PBM_SETBARCOLOR		(WM_USER+9)
#endif
#ifndef PBM_SETBKCOLOR
#define PBM_SETBKCOLOR		CCM_SETBKCOLOR
#endif


class CWaterPot : public CWnd
{
    DECLARE_DYNAMIC(CWaterPot)

public:
    CWaterPot();
    virtual ~CWaterPot();

public:
    void  SetImage(CImage *image);
    void  SetHeight(float fHeight);
    float SetPos(float fPos);

protected:
    void CommonPaint();
    BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

protected:
    DECLARE_MESSAGE_MAP()


protected:
   BOOL     	m_bShowPercent;             // true to show percent complete as text
   COLORREF 	m_crBarClr,  m_crBarBkClr;  // bar colors
   COLORREF 	m_crTextClr, m_crTextBkClr; // text colors
   float        m_fHeight;                  // water pot height
   float        m_fPos;                     // current position within water pot
   CImage      *m_Image;                    // Í¸Ã÷±³¾°Í¼Æ¬

public:
    afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
    afx_msg void    OnPaint();
    afx_msg LRESULT OnSetBarColor(WPARAM, LPARAM);
    afx_msg LRESULT OnSetBarBkColor(WPARAM, LPARAM);
};


