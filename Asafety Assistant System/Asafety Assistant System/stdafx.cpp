
// stdafx.cpp : 只包括标准包含文件的源文件
// Asafety Assistant System.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

CString ConvertString(CString strText)//从langchn.ini文件中取出名字为strText的值返回
{
	char *val = new char[200];
	CString strIniPath,strRet;
	
	memset(val,0,200);
	GetPrivateProfileString("String",strText,"",val,200,"./langchn.ini");//将string字段下面的名字为strText的值取出来
	strRet = val;
	if(strRet.GetLength()==0)//如果没能取出来
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
