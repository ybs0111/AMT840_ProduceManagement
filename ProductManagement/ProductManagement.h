
// ProductManagement.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CProductManagementApp:
// �� Ŭ������ ������ ���ؼ��� ProductManagement.cpp�� �����Ͻʽÿ�.
//

class CProductManagementApp : public CWinApp
{
public:
	CProductManagementApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CProductManagementApp theApp;