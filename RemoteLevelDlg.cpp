
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
    m_LevelID[0]   = m_LevelID[1]   = m_LevelID[2]   = m_LevelID[3]   = _T("δ����");
    m_LevelTime[0] = m_LevelTime[1] = m_LevelTime[2] = m_LevelTime[3] = _T("");
    m_LevelData[0] = m_LevelData[1] = m_LevelData[2] = m_LevelData[3] = _T("������");
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
    m_LevelMeter.Start(2317, this->GetSafeHwnd());

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

LRESULT CRemoteLevelDlg::OnLevelMsg(WPARAM wParam, LPARAM lParam)
{
    LevelMsg* lpMsg = (LevelMsg*)wParam;
    CString   LevelID, LevelTime, LevelData;
    CTime     ctime(lpMsg->m_time);
    LevelID.Format(_T("%u��Һλ��"), lpMsg->m_LevelID);
    LevelData.Format(_T("%.2f��"), 1.73 - lpMsg->m_LevelVal);
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
        if( m_LevelID[i] == _T("δ����") ) {
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

