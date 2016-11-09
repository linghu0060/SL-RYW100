
// RemoteLevelDlg.h : 头文件
//

#pragma once

#include "LevelMeter.h"
#include "TextProgressCtrl.h"
#include "WaterPot.h"


#define LEVEL_SHOW_MAX      4                           // 可显示的液位计最大数量
#define LEVEL_ID_DEF        _T("#号液位计")              // 默认字符串----液位计编号
#define LEVEL_ID_FMT        _T("%u号液位计")             // 格式化字符串--液位计编号
#define LEVEL_TIME_DEF      _T("####-##-## ##:##:##")   // 默认字符串----液位计测量时间
#define LEVEL_TIME_FMT      _T("%Y-%m-%d %H:%M:%S")     // 格式化字符串--液位计测量时间
#define LEVEL_DATA_DEF      _T("###米")                 // 默认字符串----液位计测量数据
#define LEVEL_DATA_FMT      _T("%.2f米")                // 格式化字符串--液位计测量数据



//class C




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


public:
    afx_msg void    OnTimer(UINT_PTR nIDEvent);                 // 消息处理函数--定时器
    static UINT     MessageThread(LPVOID pParam);               // 警报消息线程函数

protected:
    afx_msg LRESULT OnLevelMsg(WPARAM wParam, LPARAM lParam);   // 消息处理函数--液位
protected:
    CLevelMeter     m_LevelMeter;                               // 液位测量对象
protected:
    CString         m_LevelID[LEVEL_SHOW_MAX];                  // 液位计编号
    CString         m_LevelTime[LEVEL_SHOW_MAX];                // 液位计测量时间
    CString         m_LevelData[LEVEL_SHOW_MAX];                // 液位计测量数据
    CWaterPot       m_Progress[LEVEL_SHOW_MAX];                 // 液位指示进度条
    CImage          m_Image;                                    // 透明背景图片
protected:
    int             m_LevelPreVal[LEVEL_SHOW_MAX];              // 前一个液位测量数值
    BOOL            m_LevelAram[LEVEL_SHOW_MAX];                // 液位警报状态
protected:
    int             m_ctHeight[LEVEL_SHOW_MAX];                 // 水罐高度
    int             m_arHeigth[LEVEL_SHOW_MAX];                 // 报警高度
    int             m_cbtValue[LEVEL_SHOW_MAX];                 // 水位校准
};
