#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSet 对话框

class CSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSet)

public:
	CSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSet();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_CANSHU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString  GetDvrIP();//获得ip控件上的ip地址
	CIPAddressCtrl m_DvrIPAddr;

	WORD wDVRPort;//=(WORD)nPort;//获得端口
	char *pchDVRIP;//获得ip地址
	char *pchUserName;//用户名
	char *pchPassword;//密码
	CComboBox GPS_chuankou;
	CComboBox GPS_botelv;
	CComboBox GPS_shujuwei;
	afx_msg void OnBnClickedCancel();
	CComboBox GPS_tingzhiwei;
	CComboBox GPS_jiaoyanwei;
	CComboBox GPRS_chuankou;
	CComboBox GPRS_botelv;
	CComboBox GPRS_shujuwei;
	CComboBox GPRS_tingzhiwei;
	CComboBox GPRS_jiaoyanwei;
	CComboBox Radar_chuankou;
	CComboBox Radar_botelv;
	CComboBox Radar_shujuwei;
	CComboBox Radar_tingzhiwei;
	CComboBox Radar_jiaoyanwei;
	afx_msg void OnCbnSelchangeCombo1();
	int GPS_m_nBaud;       //波特率
	int GPS_m_nCom;         //串口号
	char GPS_m_cParity;    //校验
	int GPS_m_nDatabits;    //数据位
	int GPS_m_nStopbits;    //停止位
	int GPRS_m_nBaud;       //波特率
	int GPRS_m_nCom;         //串口号
	char GPRS_m_cParity;    //校验
	int GPRS_m_nDatabits;    //数据位
	int GPRS_m_nStopbits;    //停止位
	int Radar_m_nBaud;       //波特率
	int Radar_m_nCom;         //串口号
	char Radar_m_cParity;    //校验
	int Radar_m_nDatabits;    //数据位
	int Radar_m_nStopbits;    //停止位
	int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12;

	// 预警参数
	double AlarmP[6]; // 按照预警信号存储

	afx_msg void OnCbnSelchangeCombo11();
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo12();
	afx_msg void OnCbnSelchangeCombo8();
	afx_msg void OnCbnSelchangeCombo17();
	afx_msg void OnCbnSelchangeCombo15();
	afx_msg void OnCbnSelchangeCombo10();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnCbnSelchangeCombo13();
	afx_msg void OnCbnSelchangeCombo9();
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg void OnCbnSelchangeCombo14();
	afx_msg void OnCbnSelchangeCombo16();
	afx_msg void OnCbnSelchangeComboAlarm();
	CComboBox Combox_SetAlarmPara;
};
