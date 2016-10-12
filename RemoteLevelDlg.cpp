
// RemoteLevelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteLevel.h"
#include "RemoteLevelDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////////////
// CRemoteLevelDlg 对话框

CRemoteLevelDlg::CRemoteLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteLevelDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_LevelID[0]   = m_LevelID[1]   = m_LevelID[2]   = m_LevelID[3]   = _T("未连接");
    m_LevelTime[0] = m_LevelTime[1] = m_LevelTime[2] = m_LevelTime[3] = _T("");
    m_LevelData[0] = m_LevelData[1] = m_LevelData[2] = m_LevelData[3] = _T("无数据");
}

void CRemoteLevelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1,   m_LevelID[0]);
    DDX_Text(pDX, IDC_EDIT1_1, m_LevelTime[0]);
    DDX_Text(pDX, IDC_EDIT1_2, m_LevelData[0]);
    DDX_Text(pDX, IDC_EDIT2,   m_LevelID[1]);
    DDX_Text(pDX, IDC_EDIT2_1, m_LevelTime[1]);
    DDX_Text(pDX, IDC_EDIT2_2, m_LevelData[1]);
    DDX_Text(pDX, IDC_EDIT3,   m_LevelID[2]);
    DDX_Text(pDX, IDC_EDIT3_1, m_LevelTime[2]);
    DDX_Text(pDX, IDC_EDIT3_2, m_LevelData[2]);
    DDX_Text(pDX, IDC_EDIT4,   m_LevelID[3]);
    DDX_Text(pDX, IDC_EDIT4_1, m_LevelTime[3]);
    DDX_Text(pDX, IDC_EDIT4_2, m_LevelData[3]);
}

BEGIN_MESSAGE_MAP(CRemoteLevelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_WM_LEVEL_MSG(OnLevelMsg)
END_MESSAGE_MAP()


// CRemoteLevelDlg 消息处理程序

BOOL CRemoteLevelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_LevelMeter.Start(2317, this->GetSafeHwnd());

//	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
    GetDlgItem(IDC_BTN_EXIT)->SetFocus();
    return FALSE;
}

void CRemoteLevelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteLevelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRemoteLevelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRemoteLevelDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
//  CDialogEx::OnOK();
}


void CRemoteLevelDlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
 // CDialogEx::OnCancel();
}


void CRemoteLevelDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    EndDialog(IDCANCEL);
}

LRESULT CRemoteLevelDlg::OnLevelMsg(WPARAM wParam, LPARAM lParam)
{
    LevelMsg* lpMsg = (LevelMsg*)wParam;
    CString   LevelID, LevelTime, LevelData;
    CTime     ctime(lpMsg->m_time);
    LevelID.Format(_T("%u号液位计"), lpMsg->m_LevelID);
    LevelData.Format(_T("%.2f米"), 1.73 - lpMsg->m_LevelVal);
    LevelTime = ctime.Format(_T("%Y-%m-%d %H:%M:%S"));
    delete lpMsg;

    UpdateData(TRUE);
    for(int i = 0;  i < 4;  i++ )
    {
        if( m_LevelID[i] == LevelID ) {
            m_LevelTime[i] = LevelTime;
            m_LevelData[i] = LevelData;
            goto RETURN;
        }
    }
    for(int i = 0;  i < 4;  i++)
    {
        if( m_LevelID[i] == _T("未连接") ) {
            m_LevelID[i]   = LevelID;
            m_LevelTime[i] = LevelTime;
            m_LevelData[i] = LevelData;
            goto RETURN;
        }
    }

  RETURN:
    UpdateData(FALSE);
    return( 0 );
}

