
// STM32_PR.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CSTM32PRApp:
// 有关此类的实现，请参阅 STM32_PR.cpp
//

class CSTM32PRApp : public CWinApp
{
public:
	CSTM32PRApp();

// 重写
public:
	virtual BOOL InitInstance();

	CString GetLastError(DWORD dwError);

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSTM32PRApp theApp;
