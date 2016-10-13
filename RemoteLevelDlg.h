
// RemoteLevelDlg.h : 头文件
//

#pragma once

#include "LevelMeter.h"


// CRemoteLevelDlg 对话框
class CRemoteLevelDlg : public CDialogEx
{
// 构造
public:
	CRemoteLevelDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REMOTELEVEL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON       m_hIcon;

	// 生成的消息映射函数
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
