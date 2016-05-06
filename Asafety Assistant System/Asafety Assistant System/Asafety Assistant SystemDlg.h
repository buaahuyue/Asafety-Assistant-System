
// Asafety Assistant SystemDlg.h : 头文件
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
#define WM_USERMESSAGE			    (WM_USER + 100)         //热像仪的状态消息
#define WM_USERMESSAGE1			    (WM_USER + 101)         //热像仪的状态消息
// CAsafetyAssistantSystemDlg 对话框
enum AlarmClass{ SAFE, THREAT, DANGEROUS, GODBLESSYOU};


class CAsafetyAssistantSystemDlg : public CDialogEx
{
// 构造
public:
	CAsafetyAssistantSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ASAFETYASSISTANTSYSTEM_DIALOG };
public:
	BOOL m_bFullScreenMode;        //全屏显示标志
private: 
    WINDOWPLACEMENT m_OldWndPlacement;         //用来保存原窗口位置 
    CRect m_FullScreenRect;        //表示全屏显示时的窗口位置
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
	WORD wDVRPort;//=(WORD)nPort;//获得端口
	char *pchDVRIP;
	char *pchUserName;
	char *pchPassword;
	long m_LoginID;//判断海康的服务器是否登录
	LOCAL_DEVICE_INFO m_struDeviceInfo;//存放当前视频服务器的通道数等信息
	int  PortNum;//当前NVS可用的摄像头数量
	long m_lPlayHandle;
	int camera_EN;
	int GPS_EN;
	int GPRS_EN;
	int Radar_EN;
public:
	void  UpdataScreenPos();//将当前场景对齐到图片控件上
	void  MessageProc(int nWndID, UINT message);//在屏幕上左键或者右键单击的时候，让当前选择的画面红亮显示，即激活当前选中的画面
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
	void start_haikang();//启动海康所有的通道
	void StopPlay();//让指定的通道的视频停止播放
	bool OnBTLeave();//退出服务器连接
	void init_all_data();//初始化所有需要初始化的变量
	HWND   GetDispHandle(int nNum);
	bool  OnBTLogin(CString strIP,int Port,CString ADMIN,CString PassWord); //登陆按钮，取得各个控件上的服务器ip地址，端口号，用户名和密码连接服务器，同时将当前服务器的视频通道数添加进通道数下拉框中
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	//float speed[2];
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton3();
	CSerialPort m_Port1;  //CSerialPort类对象
	CSerialPort m_Port2;  //CSerialPort类对象
	CSerialPort m_Port3;  //CSerialPort类对象
	int  gprsopen;
	void openchuankou();
	void OpenGPSchuankou();
	void OpenGPRSchuankou();
	void OpenRadarchuankou();
	void CloseGPSchuankou();
	void CloseGPRSchuankou();
	void CloseRadarchuankou();
	DWORD GPS_m_dwCommEvents;
	int GPS_m_nBaud;       //波特率
	int GPS_m_nCom;         //串口号
	char GPS_m_cParity;    //校验
	int GPS_m_nDatabits;    //数据位
	int GPS_m_nStopbits;    //停止位
	DWORD GPRS_m_dwCommEvents;
	int GPRS_m_nBaud;       //波特率
	int GPRS_m_nCom;         //串口号
	char GPRS_m_cParity;    //校验
	int GPRS_m_nDatabits;    //数据位
	int GPRS_m_nStopbits;    //停止位
	DWORD Radar_m_dwCommEvents;
	int Radar_m_nBaud;       //波特率
	int Radar_m_nCom;         //串口号
	char Radar_m_cParity;    //校验
	int Radar_m_nDatabits;    //数据位
	int Radar_m_nStopbits;    //停止位
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
	int zhuatu();//抓取指定通道的图
	void initdata();
	CString OnBUTTONSure(double in_JD1,double in_WD1,double in_JD2,double in_WD2) ;
	CComboBox m_box;
	//afx_msg void OnBnClickedButton7();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//解析GPS数据 
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
	HBITMAP   hBitmap_1; // 空图片
	CRect alarmLamp_rc;
	// bool bFlash; 
	afx_msg void OnCbnSelchangeCombo1();

	//绘图
	CWnd* pWnd;
	CRect m_rc;
	CDC* pDC;
	CRect rc;
	CBitmap bitmap;
	void DrawACar(double Pos_x, double Pos_y, double velocity, int ID);
	void DrawBackground();
	// 绘制警示标志
	void DrawAlarmLight(COLORREF cColor);
	
	AlarmClass alarmClass;
	double alarmPara[6];
	// 预警算法
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

	// 用于预警算法
	double nearestCar_Vel;	// 最近的一辆车的速度
	double nearestCar_Accle; // 最近的一辆车的加速度
	bool nearestCar_flag;   // 最近的一辆车是否在雷达的范围内
	double nearestCar_Distance;  // 最近的一辆车的距离

	bool isAlarmfalg[4]; // 用于管理报警灯的执行次序——如果警报等级升级，则
	int time_count[4];   // 用于管理报警灯的执行时间
	int alarmLast; // 上一次的警报等级

	CFile g_File;

	CDC *pmDC;
};
