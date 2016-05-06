
// Asafety Assistant SystemDlg.h : ͷ�ļ�
//

#pragma once

#include "video\\ptzScreen.h"//2013.10.13
#include "haikang\\GeneralDef.h"
#include "haikang\\DataType.h"
#include "haikang\\DecodeCardSdk.h"
#include "haikang\\plaympeg4.h"
#include "haikang\\PlayM4.h"
#include "SerialPort.h"
#include "afxwin.h"
#define TIMER1 1
#define TIMER2 2
#define WM_USERMESSAGE			    (WM_USER + 100)         //�����ǵ�״̬��Ϣ
#define WM_USERMESSAGE1			    (WM_USER + 101)         //�����ǵ�״̬��Ϣ
// CAsafetyAssistantSystemDlg �Ի���
enum AlarmClass{ SAFE, THREAT, DANGEROUS, GODBLESSYOU};


class CAsafetyAssistantSystemDlg : public CDialogEx
{
// ����
public:
	CAsafetyAssistantSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ASAFETYASSISTANTSYSTEM_DIALOG };
public:
	BOOL m_bFullScreenMode;        //ȫ����ʾ��־
private: 
    WINDOWPLACEMENT m_OldWndPlacement;         //��������ԭ����λ�� 
    CRect m_FullScreenRect;        //��ʾȫ����ʾʱ�Ĵ���λ��
public: 
    void FullScreenModeOn(); 
    void FullScreenModeOff(); 

	CPtzScreen		m_ptzScreen;
	CRect			m_clientRect;
	CRect			m_screenRect;
	int				m_CurScreen;
	CCriticalSection m_cs;
	CCriticalSection m_csPos;
	BOOL			m_bWndExitDecode[16];
	BOOL			m_bWndExitCycle[16];
	int		m_posX;
	int		m_posY;
	int		m_posZoom;
	WORD wDVRPort;//=(WORD)nPort;//��ö˿�
	char *pchDVRIP;
	char *pchUserName;
	char *pchPassword;
	long m_LoginID;//�жϺ����ķ������Ƿ��¼
	LOCAL_DEVICE_INFO m_struDeviceInfo;//��ŵ�ǰ��Ƶ��������ͨ��������Ϣ
	int  PortNum;//��ǰNVS���õ�����ͷ����
	long m_lPlayHandle;
	int camera_EN;
	int GPS_EN;
	int GPRS_EN;
	int Radar_EN;
public:
	void  UpdataScreenPos();//����ǰ�������뵽ͼƬ�ؼ���
	void  MessageProc(int nWndID, UINT message);//����Ļ����������Ҽ�������ʱ���õ�ǰѡ��Ļ��������ʾ�������ǰѡ�еĻ���
	BOOL GetParams(int nWndID, int type);
	void SetParams(int nWndID, int type);
	BOOL GetExitDecode(int nCurWndID);
	BOOL GetExitCycle(int nCurWndID);
	void SetExitDecode(int nCurWndID);
	void SetExitCycle(int nCurWndID);
	friend void CALLBACK  MessageProcFunc(int nWndID, UINT message, DWORD dwUser);
	friend BOOL CALLBACK  GetParamsFunc(int nWndID, int type, DWORD dwUser);
	friend void CALLBACK SetParamsFunc(int nWndID, int type, DWORD dwUser);
	friend void CALLBACK RectEventFunc(RECT WinRect,CPoint &pointStart,CPoint &pointEnd,DWORD dwUser);
	void DoGetDeviceResoureCfg();
	void  CAsafetyAssistantSystemDlg::GetDecoderCfg();
	void start_haikang();//�����������е�ͨ��
	void StopPlay();//��ָ����ͨ������Ƶֹͣ����
	bool OnBTLeave();//�˳�����������
	void init_all_data();//��ʼ��������Ҫ��ʼ���ı���
	HWND   GetDispHandle(int nNum);
	bool  OnBTLogin(CString strIP,int Port,CString ADMIN,CString PassWord); //��½��ť��ȡ�ø����ؼ��ϵķ�����ip��ַ���˿ںţ��û������������ӷ�������ͬʱ����ǰ����������Ƶͨ������ӽ�ͨ������������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
protected:
	//float speed[2];
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton3();
	CSerialPort m_Port1;  //CSerialPort�����
	CSerialPort m_Port2;  //CSerialPort�����
	CSerialPort m_Port3;  //CSerialPort�����
	int  gprsopen;
	void openchuankou();
	void OpenGPSchuankou();
	void OpenGPRSchuankou();
	void OpenRadarchuankou();
	void CloseGPSchuankou();
	void CloseGPRSchuankou();
	void CloseRadarchuankou();
	DWORD GPS_m_dwCommEvents;
	int GPS_m_nBaud;       //������
	int GPS_m_nCom;         //���ں�
	char GPS_m_cParity;    //У��
	int GPS_m_nDatabits;    //����λ
	int GPS_m_nStopbits;    //ֹͣλ
	DWORD GPRS_m_dwCommEvents;
	int GPRS_m_nBaud;       //������
	int GPRS_m_nCom;         //���ں�
	char GPRS_m_cParity;    //У��
	int GPRS_m_nDatabits;    //����λ
	int GPRS_m_nStopbits;    //ֹͣλ
	DWORD Radar_m_dwCommEvents;
	int Radar_m_nBaud;       //������
	int Radar_m_nCom;         //���ں�
	char Radar_m_cParity;    //У��
	int Radar_m_nDatabits;    //����λ
	int Radar_m_nStopbits;    //ֹͣλ
	void closechuankou();
	afx_msg void OnDestroy();
	int Str2Hex(CString str, char* data);
	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
	char HexChar(char c);
	//CEdit gpsshow;
	CString gps_show;
	CString Radar_show;
	CString gprs_show;
	void OnCbnSelchangeCombo1(int i);
	void OnCbnSelchangeCombo11(int i);
	void OnCbnSelchangeCombo7(int i);
	void OnCbnSelchangeCombo4(int i);
	void OnCbnSelchangeCombo12(int i);
	void OnCbnSelchangeCombo8(int i);
	void OnCbnSelchangeCombo17(int i);
	void OnCbnSelchangeCombo15(int i);
	void OnCbnSelchangeCombo10(int i);
	void OnCbnSelchangeCombo5(int i);
	void OnCbnSelchangeCombo13(int i);
	void OnCbnSelchangeCombo9(int i);
	void OnCbnSelchangeCombo6(int i);
	void OnCbnSelchangeCombo14(int i);
	void OnCbnSelchangeCombo16(int i);
	afx_msg void OnBnClickedOk();
	LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnMyMessage1(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	CString picdizhi;
	CString videodizhi;
	int zhuatu();//ץȡָ��ͨ����ͼ
	void initdata();
	CString OnBUTTONSure(double in_JD1,double in_WD1,double in_JD2,double in_WD2) ;
	CComboBox m_box;
	//afx_msg void OnBnClickedButton7();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//����GPS���� 
	void Get_GPS(char *a );
	void Get_GPS( );
	float zuosudu,zuosudu_old;
	float yousudu,yousudu_old;
	double jingdu_new,jingdu_old;
	double weidu_new,weidu_old;
	//int gps_jisu;
	//float gps_speed;
	int  licheng;
	int firstfindgps;
	afx_msg void OnStnClickedjingdu();
	afx_msg void OnStnClickedgaocheng();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnEnChangeEdit1();
	HBITMAP   hBitmap; 
	HBITMAP   hBitmap_1; // ��ͼƬ
	CRect alarmLamp_rc;
	// bool bFlash; 
	afx_msg void OnCbnSelchangeCombo1();

	//��ͼ
	CWnd* pWnd;
	CRect m_rc;
	CDC* pDC;
	CRect rc;
	CBitmap bitmap;
	void DrawACar(double Pos_x, double Pos_y, double velocity, int ID);
	void DrawBackground();
	// ���ƾ�ʾ��־
	void DrawAlarmLight(COLORREF cColor);
	
	AlarmClass alarmClass;
	double alarmPara[6];
	// Ԥ���㷨
	int Alarm_Algorithm(double alarm_para[6], double v_a, double v_b, double a_b, bool flag, double Sab);
	void ShowAlarmLight(int alarmClass);

	struct Car_Info{
		double PosCarX[32];
		double PosCarY[32];
		double VelocityX[32];
		int ID[32];
		int car_num;
	};
	Car_Info m_carInfo;

	struct MyCar_Motion{
		double vel;
		double accel;
		//DWORD time;
	};
	MyCar_Motion myCar[2];
	
	struct FCar_Motion{
		double pos_x[32];
		double vel[32];
		double accel[32];
		int ID[32]; // forwardCar
		//double time;
	};
	FCar_Motion forwardCar[2];

	// ����Ԥ���㷨
	double nearestCar_Vel;	// �����һ�������ٶ�
	double nearestCar_Accle; // �����һ�����ļ��ٶ�
	bool nearestCar_flag;   // �����һ�����Ƿ����״�ķ�Χ��
	double nearestCar_Distance;  // �����һ�����ľ���

	bool isAlarmfalg[4]; // ���ڹ������Ƶ�ִ�д��򡪡���������ȼ���������
	int time_count[4];   // ���ڹ������Ƶ�ִ��ʱ��
	int alarmLast; // ��һ�εľ����ȼ�

	CFile g_File;

	CDC *pmDC;
};
