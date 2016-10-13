
// RemoteLevelDlg.h : ͷ�ļ�
//

#pragma once

#include "LevelMeter.h"


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


protected:
    afx_msg LRESULT OnLevelMsg(WPARAM wParam, LPARAM lParam);
protected:
    CLevelMeter     m_LevelMeter;
public:
    CString         m_LevelID[4];
    CString         m_LevelTime[4];
    CString         m_LevelData[4];
    CProgressCtrl   m_Progress[4];
};
