
// RemoteLevel.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRemoteLevelApp:
// �йش����ʵ�֣������ RemoteLevel.cpp
//

class CRemoteLevelApp : public CWinApp
{
public:
	CRemoteLevelApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRemoteLevelApp theApp;