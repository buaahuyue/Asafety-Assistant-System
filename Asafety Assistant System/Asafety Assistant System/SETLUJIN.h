#pragma once


// CSETLUJIN �Ի���

class CSETLUJIN : public CDialog
{
	DECLARE_DYNAMIC(CSETLUJIN)

public:
	CSETLUJIN(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSETLUJIN();

// �Ի�������
	enum { IDD = IDD_lujin };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
