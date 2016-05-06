#pragma once


// CSETLUJIN 对话框

class CSETLUJIN : public CDialog
{
	DECLARE_DYNAMIC(CSETLUJIN)

public:
	CSETLUJIN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSETLUJIN();

// 对话框数据
	enum { IDD = IDD_lujin };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString strFilePath;
	CString strFilePath1;
	CString GETSaveList();
	CString GETSaveList1();
};
