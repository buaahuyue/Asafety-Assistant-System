
// Asafety Assistant System.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAsafetyAssistantSystemApp:
// �йش����ʵ�֣������ Asafety Assistant System.cpp
//

class CAsafetyAssistantSystemApp : public CWinApp
{
public:
	CAsafetyAssistantSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAsafetyAssistantSystemApp theApp;