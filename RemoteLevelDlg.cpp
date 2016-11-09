
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
    m_LevelID[0]   = m_LevelID[1]   = m_LevelID[2]   = m_LevelID[3]   = LEVEL_ID_DEF;
    m_LevelTime[0] = m_LevelTime[1] = m_LevelTime[2] = m_LevelTime[3] = LEVEL_TIME_DEF;
    m_LevelData[0] = m_LevelData[1] = m_LevelData[2] = m_LevelData[3] = LEVEL_DATA_DEF;
}

void CRemoteLevelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT1,   m_LevelID[0]);
    DDX_Text(pDX, IDC_EDIT1_1, m_LevelTime[0]);
    DDX_Text(pDX, IDC_EDIT1_2, m_LevelData[0]);
    DDX_Control(pDX, IDC_PROGRESS1, m_Progress[0]);

    DDX_Text(pDX, IDC_EDIT2,   m_LevelID[1]);
    DDX_Text(pDX, IDC_EDIT2_1, m_LevelTime[1]);
    DDX_Text(pDX, IDC_EDIT2_2, m_LevelData[1]);
    DDX_Control(pDX, IDC_PROGRESS2, m_Progress[1]);

    DDX_Text(pDX, IDC_EDIT3,   m_LevelID[2]);
    DDX_Text(pDX, IDC_EDIT3_1, m_LevelTime[2]);
    DDX_Text(pDX, IDC_EDIT3_2, m_LevelData[2]);
    DDX_Control(pDX, IDC_PROGRESS3, m_Progress[2]);

    DDX_Text(pDX, IDC_EDIT4,   m_LevelID[3]);
    DDX_Text(pDX, IDC_EDIT4_1, m_LevelTime[3]);
    DDX_Text(pDX, IDC_EDIT4_2, m_LevelData[3]);
    DDX_Control(pDX, IDC_PROGRESS4, m_Progress[3]);
}

BEGIN_MESSAGE_MAP(CRemoteLevelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_WM_LEVEL_MSG(OnLevelMsg)
    ON_WM_TIMER()
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
    m_Image.Load(_T(".//pot.png"));                 // 透明背景图片
    if( m_Image.GetBPP() == 32 )
    {
        for(int i = 0; i < m_Image.GetWidth(); i++)
        {
            for(int j = 0; j < m_Image.GetHeight(); j++)
            {
                unsigned char* pucColor = reinterpret_cast<unsigned char *>(m_Image.GetPixelAddress(i , j));
                pucColor[0] = pucColor[0] * pucColor[3] / 255;
                pucColor[1] = pucColor[1] * pucColor[3] / 255;
                pucColor[2] = pucColor[2] * pucColor[3] / 255;
            }
        }
    }
    m_LevelMeter.Start(2317, this->GetSafeHwnd());  // 液位测量服务启动
    for(int i = 0;  i < LEVEL_SHOW_MAX;  i++)
    {
        // 初始化警报信息
        CString strTemp;  strTemp.Format(_T("Level.%u"), i + 1);
        m_ctHeight[i]    = ::GetPrivateProfileInt(strTemp, _T("ctHeigth"), 2000, _T(".//Config.ini"));
        m_arHeigth[i]    = ::GetPrivateProfileInt(strTemp, _T("arHeigth"), 1000, _T(".//Config.ini"));
        m_cbtValue[i]    = ::GetPrivateProfileInt(strTemp, _T("cbtValue"),    0, _T(".//Config.ini"));
        m_LevelPreVal[i] = m_arHeigth[i] + 1;
        m_LevelAram[i]   = FALSE;

        // 设置进度条颜色、范围
        ::SendMessage(m_Progress[i].GetSafeHwnd(), PBM_SETBKCOLOR,  0, RGB(255,255,255));
        ::SendMessage(m_Progress[i].GetSafeHwnd(), PBM_SETBARCOLOR, 0, RGB(47, 162,219));
        m_Progress[i].SetWindowText(_T("水位(米): "));
        m_Progress[i].SetHeight((float)m_ctHeight[i] / 1000);
        m_Progress[i].SetPos((float)m_LevelPreVal[i] / 1000);
        m_Progress[i].SetImage(&m_Image);
    }
    SetTimer(ID_TIMER_ALARM, 1000, NULL);           // 警报处理定时器

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

//  	CPaintDC dc(this); // 用于绘制的设备上下文
//      CRect wndRect;
//      if( m_Image != NULL )
//      {
//          if( m_Image.IsNull() != TRUE )
//          {
//              for( int i = 0;  i < LEVEL_SHOW_MAX;  i++ )
//              {
//                  ::GetWindowRect(m_Progress[i], &wndRect);  ScreenToClient(&wndRect);
//                  m_Image.Draw(dc.GetSafeHdc(), wndRect);
//              }
//          }
//      }
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


////////////////////////////////////////////////////////////////////////////////
// 消息处理函数--定时器
void CRemoteLevelDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if( nIDEvent == ID_TIMER_ALARM )    // 警报处理定时器
    {
        static BOOL s_bfPlay = FALSE;
        int         i;

        for( i = 0;  i < LEVEL_SHOW_MAX;  i++ )
        {
            if( m_LevelAram[i] && !s_bfPlay ) {     // 有液位警报
                ::PlaySound( _T(".//Alarm.wav"), NULL,
                             SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT
                           );                       // ---播放警报音
                s_bfPlay = TRUE;
                break;
            }
        }
        if( (i >= LEVEL_SHOW_MAX) && s_bfPlay ) {   // 无液位警报
            ::PlaySound(NULL, NULL, 0);             // ---停止播放警报音
            s_bfPlay = FALSE;
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}

// 警报消息线程函数
UINT CRemoteLevelDlg::MessageThread(LPVOID pParam)
{
    CString  strTemp;
    CTime    time = CTime::GetCurrentTime();

    strTemp.Format(_T("%s\n%u号液位低于最低数"), (LPCTSTR)time.Format(LEVEL_TIME_FMT), (int)pParam);
    ::MessageBox( NULL, strTemp, _T("低液位警报"),
                  MB_OK | MB_ICONERROR | MB_SYSTEMMODAL | MB_SETFOREGROUND
                );

    return( 0 );
}

// 消息处理函数--液位
LRESULT CRemoteLevelDlg::OnLevelMsg(WPARAM wParam, LPARAM lParam)
{
    CString   strTemp;
    int       i;
    LevelMsg* lpMsg    = (LevelMsg*)wParam;
    int       levelVal = lpMsg->m_LevelVal;
    UINT      levelID  = lpMsg->m_LevelID;
    CTime     levelTime(lpMsg->m_time);
    delete    lpMsg;
    UpdateData(TRUE);

    for(strTemp.Format(LEVEL_ID_FMT, levelID), i = 0;  i < LEVEL_SHOW_MAX;  i++)
    {                                               // 查找上次显示位置
        if( m_LevelID[i] == strTemp ) {
            break;
        }
    }
    if( i >= LEVEL_SHOW_MAX ) {                     // 未找到上次显示位置
        for(i = 0;  i < LEVEL_SHOW_MAX;  i++)
        {                                           // 查找未使用显示位置
            if( m_LevelID[i] == LEVEL_ID_DEF ) {
                m_LevelID[i] = strTemp;             // ---显示液位计编号
                m_Progress[i].ShowWindow(SW_SHOW);  // ---进度条显示可见
                break;
            }
        }
    }

    if( i < LEVEL_SHOW_MAX )                        // 有效使用位置
    {
        levelVal = m_ctHeight[i] + m_cbtValue[i] - levelVal;        // 有效液位数值(单位mm)

        strTemp.Format(LEVEL_DATA_FMT, ((float)levelVal) / 1000);
        m_LevelData[i] = strTemp;                                   // 显示液位数值
        m_LevelTime[i] = levelTime.Format(LEVEL_TIME_FMT);          // 显示测量时间
        m_Progress[i].SetPos((float)levelVal / 1000);               // 显示液位指示进度条

        m_LevelAram[i] = (levelVal < m_arHeigth[i]);                     // 液位警报状态
        if( (m_LevelPreVal[i] >= m_arHeigth[i]) && m_LevelAram[i] ) {
          //::MessageBeep(MB_ICONERROR);
            AfxBeginThread(MessageThread, (LPVOID)levelID);         // 显示低液位警报对话框
        }
        m_LevelPreVal[i] = levelVal;                                // 前一个液位测量数值
    }

    UpdateData(FALSE);
    return( 0 );
}


