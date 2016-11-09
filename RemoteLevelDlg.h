
// RemoteLevelDlg.h : ͷ�ļ�
//

#pragma once

#include "LevelMeter.h"
#include "TextProgressCtrl.h"
#include "WaterPot.h"


#define LEVEL_SHOW_MAX      4                           // ����ʾ��Һλ���������
#define LEVEL_ID_DEF        _T("#��Һλ��")              // Ĭ���ַ���----Һλ�Ʊ��
#define LEVEL_ID_FMT        _T("%u��Һλ��")             // ��ʽ���ַ���--Һλ�Ʊ��
#define LEVEL_TIME_DEF      _T("####-##-## ##:##:##")   // Ĭ���ַ���----Һλ�Ʋ���ʱ��
#define LEVEL_TIME_FMT      _T("%Y-%m-%d %H:%M:%S")     // ��ʽ���ַ���--Һλ�Ʋ���ʱ��
#define LEVEL_DATA_DEF      _T("###��")                 // Ĭ���ַ���----Һλ�Ʋ�������
#define LEVEL_DATA_FMT      _T("%.2f��")                // ��ʽ���ַ���--Һλ�Ʋ�������



//class C




// CRemoteLevelDlg �Ի���
class CRemoteLevelDlg : public CDialogEx
{
// ����
public:
	CRemoteLevelDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REMOTELEVEL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON       m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
public:
    afx_msg void OnClose();


public:
    afx_msg void    OnTimer(UINT_PTR nIDEvent);                 // ��Ϣ������--��ʱ��
    static UINT     MessageThread(LPVOID pParam);               // ������Ϣ�̺߳���

protected:
    afx_msg LRESULT OnLevelMsg(WPARAM wParam, LPARAM lParam);   // ��Ϣ������--Һλ
protected:
    CLevelMeter     m_LevelMeter;                               // Һλ��������
protected:
    CString         m_LevelID[LEVEL_SHOW_MAX];                  // Һλ�Ʊ��
    CString         m_LevelTime[LEVEL_SHOW_MAX];                // Һλ�Ʋ���ʱ��
    CString         m_LevelData[LEVEL_SHOW_MAX];                // Һλ�Ʋ�������
    CWaterPot       m_Progress[LEVEL_SHOW_MAX];                 // Һλָʾ������
    CImage          m_Image;                                    // ͸������ͼƬ
protected:
    int             m_LevelPreVal[LEVEL_SHOW_MAX];              // ǰһ��Һλ������ֵ
    BOOL            m_LevelAram[LEVEL_SHOW_MAX];                // Һλ����״̬
protected:
    int             m_ctHeight[LEVEL_SHOW_MAX];                 // ˮ�޸߶�
    int             m_arHeigth[LEVEL_SHOW_MAX];                 // �����߶�
    int             m_cbtValue[LEVEL_SHOW_MAX];                 // ˮλУ׼
};
