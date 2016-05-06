#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSet �Ի���

class CSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSet)

public:
	CSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSet();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_CANSHU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString  GetDvrIP();//���ip�ؼ��ϵ�ip��ַ
	CIPAddressCtrl m_DvrIPAddr;

	WORD wDVRPort;//=(WORD)nPort;//��ö˿�
	char *pchDVRIP;//���ip��ַ
	char *pchUserName;//�û���
	char *pchPassword;//����
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
	int GPS_m_nBaud;       //������
	int GPS_m_nCom;         //���ں�
	char GPS_m_cParity;    //У��
	int GPS_m_nDatabits;    //����λ
	int GPS_m_nStopbits;    //ֹͣλ
	int GPRS_m_nBaud;       //������
	int GPRS_m_nCom;         //���ں�
	char GPRS_m_cParity;    //У��
	int GPRS_m_nDatabits;    //����λ
	int GPRS_m_nStopbits;    //ֹͣλ
	int Radar_m_nBaud;       //������
	int Radar_m_nCom;         //���ں�
	char Radar_m_cParity;    //У��
	int Radar_m_nDatabits;    //����λ
	int Radar_m_nStopbits;    //ֹͣλ
	int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12;

	// Ԥ������
	double AlarmP[6]; // ����Ԥ���źŴ洢

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
