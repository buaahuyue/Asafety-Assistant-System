
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Asafety Assistant System.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

CString ConvertString(CString strText)//��langchn.ini�ļ���ȡ������ΪstrText��ֵ����
{
	char *val = new char[200];
	CString strIniPath,strRet;
	
	memset(val,0,200);
	GetPrivateProfileString("String",strText,"",val,200,"./langchn.ini");//��string�ֶ����������ΪstrText��ֵȡ����
	strRet = val;
	if(strRet.GetLength()==0)//���û��ȡ����
	{
		//If there is no corresponding string in ini file then set it to be default value(English)
		strRet=strText;
	}
	delete val;
	return strRet;
}
//Set static text in dialogux box (English->current language)  
void g_SetWndStaticText(CWnd * pWnd)
{
	CString strCaption,strText;
	
	//Set main widnow title 
	pWnd->GetWindowText(strCaption);
	if(strCaption.GetLength()>0)
	{
		strText=ConvertString(strCaption);
		pWnd->SetWindowText(strText);
	}
	
	//Set small window title 
	CWnd * pChild=pWnd->GetWindow(GW_CHILD);
	CString strClassName;
	while(pChild)
	{
		//////////////////////////////////////////////////////////////////////////		
		//Added by Jackbin 2005-03-11
		strClassName = ((CRuntimeClass*)pChild->GetRuntimeClass())->m_lpszClassName;
		if(strClassName == "CEdit")
		{
			//The next small window 
			pChild=pChild->GetWindow(GW_HWNDNEXT);
			continue;
		}
		
		//////////////////////////////////////////////////////////////////////////	
		
		//Set current language text in small window
		pChild->GetWindowText(strCaption);
		strText=ConvertString(strCaption);
		pChild->SetWindowText(strText);
		
		//The next small window 
		pChild=pChild->GetWindow(GW_HWNDNEXT);
	}
}
