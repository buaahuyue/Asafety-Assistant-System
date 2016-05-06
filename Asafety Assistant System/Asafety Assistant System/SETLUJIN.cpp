// SETLUJIN.cpp : 实现文件
//

#include "stdafx.h"
#include "Asafety Assistant System.h"
#include "SETLUJIN.h"
#include "afxdialogex.h"


// CSETLUJIN 对话框

IMPLEMENT_DYNAMIC(CSETLUJIN, CDialog)

CSETLUJIN::CSETLUJIN(CWnd* pParent /*=NULL*/)
	: CDialog(CSETLUJIN::IDD, pParent)
{

}

CSETLUJIN::~CSETLUJIN()
{
}

void CSETLUJIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSETLUJIN, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSETLUJIN::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CSETLUJIN::OnBnClickedOk)
END_MESSAGE_MAP()


// CSETLUJIN 消息处理程序
BOOL CSETLUJIN::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\";//得到xml文件的绝对路劲ipictype1
	CString lujing;
	GetPrivateProfileString("ConfigInfo","tplujin","C:\\Users\\Administrator\\Desktop\\tanyiming",lujing.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	SetDlgItemText(IDC_EDIT1,lujing);
	lujing.ReleaseBuffer();
	GetPrivateProfileString("ConfigInfo","videolujin","C:\\Users\\Administrator\\Desktop\\tanyiming",lujing.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	SetDlgItemText(IDC_EDIT2,lujing);
	lujing.ReleaseBuffer();
	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CSETLUJIN::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file1(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("文件(*.*)||"),AfxGetMainWnd());
	if(file1.DoModal()==IDOK)
	{
		CString strFilePath1=file1.GetFolderPath();//.GetPathName();
		SetDlgItemText(IDC_EDIT1,strFilePath1);
	
	}
}

CString CSETLUJIN::GETSaveList()
{
	return strFilePath;
}
CString CSETLUJIN::GETSaveList1()
{
	return strFilePath1;
}
void CSETLUJIN::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1,strFilePath);
	GetDlgItemText(IDC_EDIT2,strFilePath1);
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\";//得到xml文件的绝对路劲ipictype1
	//CString lujing;
	::WritePrivateProfileString("ConfigInfo","tplujin",strFilePath,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","videolujin",strFilePath1,sFilePath);//ip
	CDialog::OnOK();
}
