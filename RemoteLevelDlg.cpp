
// RemoteLevelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteLevel.h"
#include "RemoteLevelDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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
// CRemoteLevelDlg �Ի���

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


// CRemoteLevelDlg ��Ϣ�������

BOOL CRemoteLevelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_Image.Load(_T(".//pot.png"));                 // ͸������ͼƬ
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
    m_LevelMeter.Start(2317, this->GetSafeHwnd());  // Һλ������������
    for(int i = 0;  i < LEVEL_SHOW_MAX;  i++)
    {
        // ��ʼ��������Ϣ
        CString strTemp;  strTemp.Format(_T("Level.%u"), i + 1);
        m_ctHeight[i]    = ::GetPrivateProfileInt(strTemp, _T("ctHeigth"), 2000, _T(".//Config.ini"));
        m_arHeigth[i]    = ::GetPrivateProfileInt(strTemp, _T("arHeigth"), 1000, _T(".//Config.ini"));
        m_cbtValue[i]    = ::GetPrivateProfileInt(strTemp, _T("cbtValue"),    0, _T(".//Config.ini"));
        m_LevelPreVal[i] = m_arHeigth[i] + 1;
        m_LevelAram[i]   = FALSE;

        // ���ý�������ɫ����Χ
        ::SendMessage(m_Progress[i].GetSafeHwnd(), PBM_SETBKCOLOR,  0, RGB(255,255,255));
        ::SendMessage(m_Progress[i].GetSafeHwnd(), PBM_SETBARCOLOR, 0, RGB(47, 162,219));
        m_Progress[i].SetWindowText(_T("ˮλ(��): "));
        m_Progress[i].SetHeight((float)m_ctHeight[i] / 1000);
        m_Progress[i].SetPos((float)m_LevelPreVal[i] / 1000);
        m_Progress[i].SetImage(&m_Image);
    }
    SetTimer(ID_TIMER_ALARM, 1000, NULL);           // ��������ʱ��

//	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRemoteLevelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

//  	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRemoteLevelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRemoteLevelDlg::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���
//  CDialogEx::OnOK();
}


void CRemoteLevelDlg::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
 // CDialogEx::OnCancel();
}


void CRemoteLevelDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    EndDialog(IDCANCEL);
}


////////////////////////////////////////////////////////////////////////////////
// ��Ϣ������--��ʱ��
void CRemoteLevelDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if( nIDEvent == ID_TIMER_ALARM )    // ��������ʱ��
    {
        static BOOL s_bfPlay = FALSE;
        int         i;

        for( i = 0;  i < LEVEL_SHOW_MAX;  i++ )
        {
            if( m_LevelAram[i] && !s_bfPlay ) {     // ��Һλ����
                ::PlaySound( _T(".//Alarm.wav"), NULL,
                             SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT
                           );                       // ---���ž�����
                s_bfPlay = TRUE;
                break;
            }
        }
        if( (i >= LEVEL_SHOW_MAX) && s_bfPlay ) {   // ��Һλ����
            ::PlaySound(NULL, NULL, 0);             // ---ֹͣ���ž�����
            s_bfPlay = FALSE;
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}

// ������Ϣ�̺߳���
UINT CRemoteLevelDlg::MessageThread(LPVOID pParam)
{
    CString  strTemp;
    CTime    time = CTime::GetCurrentTime();

    strTemp.Format(_T("%s\n%u��Һλ���������"), (LPCTSTR)time.Format(LEVEL_TIME_FMT), (int)pParam);
    ::MessageBox( NULL, strTemp, _T("��Һλ����"),
                  MB_OK | MB_ICONERROR | MB_SYSTEMMODAL | MB_SETFOREGROUND
                );

    return( 0 );
}

// ��Ϣ������--Һλ
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
    {                                               // �����ϴ���ʾλ��
        if( m_LevelID[i] == strTemp ) {
            break;
        }
    }
    if( i >= LEVEL_SHOW_MAX ) {                     // δ�ҵ��ϴ���ʾλ��
        for(i = 0;  i < LEVEL_SHOW_MAX;  i++)
        {                                           // ����δʹ����ʾλ��
            if( m_LevelID[i] == LEVEL_ID_DEF ) {
                m_LevelID[i] = strTemp;             // ---��ʾҺλ�Ʊ��
                m_Progress[i].ShowWindow(SW_SHOW);  // ---��������ʾ�ɼ�
                break;
            }
        }
    }

    if( i < LEVEL_SHOW_MAX )                        // ��Чʹ��λ��
    {
        levelVal = m_ctHeight[i] + m_cbtValue[i] - levelVal;        // ��ЧҺλ��ֵ(��λmm)

        strTemp.Format(LEVEL_DATA_FMT, ((float)levelVal) / 1000);
        m_LevelData[i] = strTemp;                                   // ��ʾҺλ��ֵ
        m_LevelTime[i] = levelTime.Format(LEVEL_TIME_FMT);          // ��ʾ����ʱ��
        m_Progress[i].SetPos((float)levelVal / 1000);               // ��ʾҺλָʾ������

        m_LevelAram[i] = (levelVal < m_arHeigth[i]);                     // Һλ����״̬
        if( (m_LevelPreVal[i] >= m_arHeigth[i]) && m_LevelAram[i] ) {
          //::MessageBeep(MB_ICONERROR);
            AfxBeginThread(MessageThread, (LPVOID)levelID);         // ��ʾ��Һλ�����Ի���
        }
        m_LevelPreVal[i] = levelVal;                                // ǰһ��Һλ������ֵ
    }

    UpdateData(FALSE);
    return( 0 );
}


