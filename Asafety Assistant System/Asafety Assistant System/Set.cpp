// Set.cpp : 实现文件
//

#include "stdafx.h"
#include "Asafety Assistant System.h"
#include "Set.h"
#include "afxdialogex.h"


// CSet 对话框

IMPLEMENT_DYNAMIC(CSet, CDialogEx)

CSet::CSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSet::IDD, pParent)
{
	GPS_m_nBaud=1;       //波特率
	GPS_m_nCom=1;         //串口号
	GPS_m_cParity=1;    //校验
	GPS_m_nDatabits=1;    //数据位
	GPS_m_nStopbits=1;    //停止位
	GPRS_m_nBaud=1;       //波特率
	GPRS_m_nCom=1;         //串口号
	GPRS_m_cParity=1;    //校验
	GPRS_m_nDatabits=1;    //数据位
	GPRS_m_nStopbits=1;    //停止位
	Radar_m_nBaud=1;       //波特率
	Radar_m_nCom=1;         //串口号
	Radar_m_cParity=1;    //校验
	Radar_m_nDatabits=1;    //数据位
	Radar_m_nStopbits=1;    //停止位
}

CSet::~CSet()
{
}

void CSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_IPADDRESS1, m_DvrIPAddr);
	DDX_Control(pDX, IDC_COMBO1, GPS_chuankou);
	DDX_Control(pDX, IDC_COMBO4, GPS_botelv);
	DDX_Control(pDX, IDC_COMBO5, GPS_shujuwei);
	DDX_Control(pDX, IDC_COMBO6, GPS_tingzhiwei);
	DDX_Control(pDX, IDC_COMBO17, GPS_jiaoyanwei);
	DDX_Control(pDX, IDC_COMBO11, GPRS_chuankou);
	DDX_Control(pDX, IDC_COMBO12, GPRS_botelv);
	DDX_Control(pDX, IDC_COMBO13, GPRS_shujuwei);
	DDX_Control(pDX, IDC_COMBO14, GPRS_tingzhiwei);
	DDX_Control(pDX, IDC_COMBO15, GPRS_jiaoyanwei);
	DDX_Control(pDX, IDC_COMBO7, Radar_chuankou);
	DDX_Control(pDX, IDC_COMBO8, Radar_botelv);
	DDX_Control(pDX, IDC_COMBO9, Radar_shujuwei);
	DDX_Control(pDX, IDC_COMBO16, Radar_tingzhiwei);
	DDX_Control(pDX, IDC_COMBO10, Radar_jiaoyanwei);
	DDX_Control(pDX, IDC_COMBO_ALARM, Combox_SetAlarmPara);
}


BEGIN_MESSAGE_MAP(CSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSet::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSet::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO11, &CSet::OnCbnSelchangeCombo11)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CSet::OnCbnSelchangeCombo7)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CSet::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO12, &CSet::OnCbnSelchangeCombo12)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CSet::OnCbnSelchangeCombo8)
	ON_CBN_SELCHANGE(IDC_COMBO17, &CSet::OnCbnSelchangeCombo17)
	ON_CBN_SELCHANGE(IDC_COMBO15, &CSet::OnCbnSelchangeCombo15)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CSet::OnCbnSelchangeCombo10)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CSet::OnCbnSelchangeCombo5)
	ON_CBN_SELCHANGE(IDC_COMBO13, &CSet::OnCbnSelchangeCombo13)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CSet::OnCbnSelchangeCombo9)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CSet::OnCbnSelchangeCombo6)
	ON_CBN_SELCHANGE(IDC_COMBO14, &CSet::OnCbnSelchangeCombo14)
	ON_CBN_SELCHANGE(IDC_COMBO16, &CSet::OnCbnSelchangeCombo16)
	ON_CBN_SELCHANGE(IDC_COMBO_ALARM, &CSet::OnCbnSelchangeComboAlarm)
END_MESSAGE_MAP()


// CSet 消息处理程序
BOOL CSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\" + sFile;//得到xml文件的绝对路劲ipictype1
	CString ip,port,admin,password,gps_chuankou,gps_botelv,gps_shujuwei,gps_tingzhiwei,gps_jiaoyanwei,gprs_chuankou,gprs_botelv,gprs_shujuwei,gprs_tingzhiwei,gprs_jiaoyanwei,radar_chuankou,radar_botelv,radar_shujuwei,radar_tingzhiwei,radar_jiaoyanwei;
	GetPrivateProfileString("ConfigInfo","ServerIP","192.168.1.100",ip.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","ServerPort","5000",     port.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","admin","admin",admin.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","password","12345",     password.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	//GetPrivateProfileString("ConfigInfo","LuJing","D:\\123.txt", lujin.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	SetDlgItemText(IDC_ip,ip);
	SetDlgItemText(IDC_EDIT1,port);
	SetDlgItemText(IDC_EDIT2,admin);
	SetDlgItemText(IDC_EDIT3,password);

	for (int i = 1; i < 13; i++)
	{
		CString s;
		s.Format("%d", i);
		GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM") + s);
	}
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM1"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM2"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM3"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM4"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM5"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM6"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM7"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM8"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM9"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM10"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM11"));
	//GPS_chuankou.InsertString(GPS_chuankou.GetCount(),_T("COM12"));
	
	GetPrivateProfileString("ConfigInfo","gps_chuankou","1",     gps_chuankou.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPS_chuankou.SetCurSel(atoi(gps_chuankou));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("300"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("600"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("1200"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("2400"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("4800"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("9600"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("19200"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("38400"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("43000"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("56000"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("57600"));
	GPS_botelv.InsertString(GPS_botelv.GetCount(),_T("115200"));
	//GPS_botelv.SetCurSel(5);
	GetPrivateProfileString("ConfigInfo","gps_botelv","5",     gps_botelv.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPS_botelv.SetCurSel(atoi(gps_botelv));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("1"));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("2"));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("3"));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("4"));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("5"));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("6"));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("7"));
	GPS_shujuwei.InsertString(GPS_shujuwei.GetCount(),_T("8"));
	//GPS_shujuwei.SetCurSel(7);
	GetPrivateProfileString("ConfigInfo","gps_shujuwei","7",     gps_shujuwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPS_shujuwei.SetCurSel(atoi(gps_shujuwei));
	GPS_tingzhiwei.InsertString(GPS_tingzhiwei.GetCount(),_T("1"));
	GPS_tingzhiwei.InsertString(GPS_tingzhiwei.GetCount(),_T("2"));
	//GPS_tingzhiwei.SetCurSel(0);
	GetPrivateProfileString("ConfigInfo","gps_tingzhiwei","0",     gps_tingzhiwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPS_tingzhiwei.SetCurSel(atoi(gps_tingzhiwei));
	GPS_jiaoyanwei.InsertString(GPS_jiaoyanwei.GetCount(),_T("None"));
	GPS_jiaoyanwei.InsertString(GPS_jiaoyanwei.GetCount(),_T("Odd"));
	GPS_jiaoyanwei.InsertString(GPS_jiaoyanwei.GetCount(),_T("Even"));
	GPS_jiaoyanwei.InsertString(GPS_jiaoyanwei.GetCount(),_T("Mark"));
	GPS_jiaoyanwei.InsertString(GPS_jiaoyanwei.GetCount(),_T("Space"));
	//GPS_jiaoyanwei.SetCurSel(0);
	GetPrivateProfileString("ConfigInfo","gps_jiaoyanwei","0",     gps_jiaoyanwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPS_jiaoyanwei.SetCurSel(atoi(gps_jiaoyanwei));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM1"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM2"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM3"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM4"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM5"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM6"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM7"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM8"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM9"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM10"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM11"));
	GPRS_chuankou.InsertString(GPRS_chuankou.GetCount(),_T("COM12"));
	//GPRS_chuankou.SetCurSel(3);
	GetPrivateProfileString("ConfigInfo","gprs_chuankou","3",     gprs_chuankou.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPRS_chuankou.SetCurSel(atoi(gprs_chuankou));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("300"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("600"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("1200"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("2400"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("4800"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("9600"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("19200"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("38400"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("43000"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("56000"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("57600"));
	GPRS_botelv.InsertString(GPRS_botelv.GetCount(),_T("115200"));
	//GPRS_botelv.SetCurSel(5);
	GetPrivateProfileString("ConfigInfo","gprs_botelv","5",     gprs_botelv.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPRS_botelv.SetCurSel(atoi(gprs_botelv));
	//GPRS_botelv.SetCurSel(GPS_m_nBaud);
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("1"));
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("2"));
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("3"));
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("4"));
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("5"));
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("6"));
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("7"));
	GPRS_shujuwei.InsertString(GPRS_shujuwei.GetCount(),_T("8"));
	//GPRS_shujuwei.SetCurSel(7);
	GetPrivateProfileString("ConfigInfo","gprs_shujuwei","7",     gprs_shujuwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPRS_shujuwei.SetCurSel(atoi(gprs_shujuwei));
	GPRS_tingzhiwei.InsertString(GPRS_tingzhiwei.GetCount(),_T("1"));
	GPRS_tingzhiwei.InsertString(GPRS_tingzhiwei.GetCount(),_T("2"));
	//GPRS_tingzhiwei.SetCurSel(0);
	GetPrivateProfileString("ConfigInfo","gprs_tingzhiwei","0",gprs_tingzhiwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPRS_tingzhiwei.SetCurSel(atoi(gprs_tingzhiwei));
	GPRS_jiaoyanwei.InsertString(GPRS_jiaoyanwei.GetCount(),_T("None"));
	GPRS_jiaoyanwei.InsertString(GPRS_jiaoyanwei.GetCount(),_T("Odd"));
	GPRS_jiaoyanwei.InsertString(GPRS_jiaoyanwei.GetCount(),_T("Even"));
	GPRS_jiaoyanwei.InsertString(GPRS_jiaoyanwei.GetCount(),_T("Mark"));
	GPRS_jiaoyanwei.InsertString(GPRS_jiaoyanwei.GetCount(),_T("Space"));
	//GPRS_jiaoyanwei.SetCurSel(0);
	GetPrivateProfileString("ConfigInfo","gprs_jiaoyanwei","0",     gprs_jiaoyanwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GPRS_jiaoyanwei.SetCurSel(atoi(gprs_jiaoyanwei));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM1"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM2"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM3"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM4"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM5"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM6"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM7"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM8"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM9"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM10"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM11"));
	Radar_chuankou.InsertString(Radar_chuankou.GetCount(),_T("COM12"));
	//Radar_chuankou.SetCurSel(3);
	GetPrivateProfileString("ConfigInfo","radar_chuankou","3",     radar_chuankou.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	Radar_chuankou.SetCurSel(atoi(radar_chuankou));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("300"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("600"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("1200"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("2400"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("4800"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("9600"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("19200"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("38400"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("43000"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("56000"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("57600"));
	Radar_botelv.InsertString(Radar_botelv.GetCount(),_T("115200"));
	//Radar_botelv.SetCurSel(5);
	GetPrivateProfileString("ConfigInfo","radar_botelv","5",     radar_botelv.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	//GPS_botelv.SetCurSel(atoi(gps_botelv));
	Radar_botelv.SetCurSel(atoi(radar_botelv));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("1"));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("2"));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("3"));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("4"));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("5"));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("6"));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("7"));
	Radar_shujuwei.InsertString(Radar_shujuwei.GetCount(),_T("8"));
	//Radar_shujuwei.SetCurSel(7);
	GetPrivateProfileString("ConfigInfo","radar_shujuwei","7",     radar_shujuwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	Radar_shujuwei.SetCurSel(atoi(radar_shujuwei));
	Radar_tingzhiwei.InsertString(Radar_tingzhiwei.GetCount(),_T("1"));
	Radar_tingzhiwei.InsertString(Radar_tingzhiwei.GetCount(),_T("2"));
	//Radar_tingzhiwei.SetCurSel(0);
	GetPrivateProfileString("ConfigInfo","radar_tingzhiwei","0",     radar_tingzhiwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	Radar_tingzhiwei.SetCurSel(atoi(radar_tingzhiwei));
	Radar_jiaoyanwei.InsertString(Radar_jiaoyanwei.GetCount(),_T("None"));
	Radar_jiaoyanwei.InsertString(Radar_jiaoyanwei.GetCount(),_T("Odd"));
	Radar_jiaoyanwei.InsertString(Radar_jiaoyanwei.GetCount(),_T("Even"));
	Radar_jiaoyanwei.InsertString(Radar_jiaoyanwei.GetCount(),_T("Mark"));
	Radar_jiaoyanwei.InsertString(Radar_jiaoyanwei.GetCount(),_T("Space"));
	//Radar_jiaoyanwei.SetCurSel(0);
	GetPrivateProfileString("ConfigInfo","radar_jiaoyanwei","0",     radar_jiaoyanwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	Radar_jiaoyanwei.SetCurSel(atoi(radar_jiaoyanwei));

	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK1);
	CString radio1;
	GetPrivateProfileString("ConfigInfo","radio1","0",     radio1.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	pBtn1->SetCheck(atoi(radio1));

	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_CHECK2);
	CString radio2;
	GetPrivateProfileString("ConfigInfo","radio2","0",     radio2.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	pBtn2->SetCheck(atoi(radio2));

	CButton* pBtn3 = (CButton*)GetDlgItem(IDC_CHECK3);
	CString radio3;
	GetPrivateProfileString("ConfigInfo","radio3","0",     radio3.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	pBtn3->SetCheck(atoi(radio3));

	CButton* pBtn4 = (CButton*)GetDlgItem(IDC_CHECK4);
	CString radio4;
	GetPrivateProfileString("ConfigInfo","radio4","0",     radio4.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	pBtn4->SetCheck(atoi(radio4));

	//// 预警信息设置
	CString m_str[6];
	GetPrivateProfileString("ConfigInfo","AlarmPara0", "1", m_str[0].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara1", "1", m_str[1].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara2", "0.2", m_str[2].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara3", "0.15", m_str[3].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara4", "4", m_str[4].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara5", "5", m_str[5].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);

	SetDlgItemText(IDC_REVISEFACTOR, m_str[0]);
	SetDlgItemText(IDC_TIME1,m_str[1]);
	SetDlgItemText(IDC_TIME2,m_str[2]);
	SetDlgItemText(IDC_TIME3,m_str[3]);
	SetDlgItemText(IDC_SAFEDISTANCE,m_str[4]);
	SetDlgItemText(IDC_ALARMACCEL,m_str[5]);

	//

	Combox_SetAlarmPara.InsertString(Combox_SetAlarmPara.GetCount(),_T("不利条件"));
	Combox_SetAlarmPara.InsertString(Combox_SetAlarmPara.GetCount(),_T("正常条件"));
	Combox_SetAlarmPara.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strDvrIP;
	CString ADMIN; 
	CString PassWord;
	CString nPort; 
    //strDvrIP = GetDvrIP();//取得ip编辑框中的ip地址字符串
	//pchDVRIP = (LPSTR)(LPCSTR)strDvrIP;
	GetDlgItemText(IDC_ip,strDvrIP);
	GetDlgItemText(IDC_EDIT1,nPort);
	GetDlgItemText(IDC_EDIT2,ADMIN);
	GetDlgItemText(IDC_EDIT3,PassWord);

	wDVRPort = atoi( nPort );
	pchUserName=(LPSTR)(LPCSTR)ADMIN;//获得服务器用户名
	pchPassword=(LPSTR)(LPCSTR)PassWord;//获得密码
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\" + sFile;//得到xml文件的绝对路劲ipictype1
	CString gps_chuankou,gps_botelv,gps_shujuwei,gps_tingzhiwei,gps_jiaoyanwei,gprs_chuankou,gprs_botelv,gprs_shujuwei,gprs_tingzhiwei,gprs_jiaoyanwei,radar_chuankou,radar_botelv,radar_shujuwei,radar_tingzhiwei,radar_jiaoyanwei;
	/*
	GetPrivateProfileString("ConfigInfo","ServerIP","192.168.1.100",ip.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","ServerPort","5000",     port.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","admin","admin",admin.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","password","12345",     password.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	*/
	
	::WritePrivateProfileString("ConfigInfo","ServerIP",strDvrIP,sFilePath);
	::WritePrivateProfileString("ConfigInfo","ServerPort",nPort,sFilePath);
	::WritePrivateProfileString("ConfigInfo","admin",ADMIN,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","password",PassWord,sFilePath);//ip
	OnCbnSelchangeCombo4();
	OnCbnSelchangeCombo13();
	OnCbnSelchangeCombo12();
	OnCbnSelchangeCombo8();
	OnCbnSelchangeCombo17();
	OnCbnSelchangeCombo15();
	OnCbnSelchangeCombo10();
	OnCbnSelchangeCombo5();
	OnCbnSelchangeCombo9();
	OnCbnSelchangeCombo6();
	OnCbnSelchangeCombo14();
	OnCbnSelchangeCombo16();
	OnCbnSelchangeCombo1();
	OnCbnSelchangeCombo7();
	OnCbnSelchangeCombo11();
	//GPS_m_nCom=GPS_chuankou.GetCurSel()+1;
	//GPRS_m_nCom=GPRS_chuankou.GetCurSel()+1;
	//Radar_m_nCom=Radar_chuankou.GetCurSel()+1;
	i1=GPS_botelv.GetCurSel();
	i2=GPRS_botelv.GetCurSel();
	i3=Radar_botelv.GetCurSel();
	i4=GPS_jiaoyanwei.GetCurSel();
	i5=GPRS_jiaoyanwei.GetCurSel();
	i6=Radar_jiaoyanwei.GetCurSel();
	i7=GPS_shujuwei.GetCurSel();
	i8=GPRS_shujuwei.GetCurSel();
	i9=Radar_shujuwei.GetCurSel();
	i10=GPS_tingzhiwei.GetCurSel();
	i11=GPRS_tingzhiwei.GetCurSel();
	i12=Radar_tingzhiwei.GetCurSel();
	gps_chuankou.Format("%d",GPS_m_nCom);
	gps_botelv.Format("%d",i1);
	gps_shujuwei.Format("%d",i7);
	gps_tingzhiwei.Format("%d",i10);
	gps_jiaoyanwei.Format("%d",i4);
	::WritePrivateProfileString("ConfigInfo","gps_chuankou",gps_chuankou,sFilePath);
	::WritePrivateProfileString("ConfigInfo","gps_botelv",gps_botelv,sFilePath);
	::WritePrivateProfileString("ConfigInfo","gps_shujuwei",gps_shujuwei,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","gps_tingzhiwei",gps_tingzhiwei,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","gps_jiaoyanwei",gps_jiaoyanwei,sFilePath);

	gprs_chuankou.Format("%d",GPRS_m_nCom);
	gprs_botelv.Format("%d",i2);
	gprs_shujuwei.Format("%d",i8);
	gprs_tingzhiwei.Format("%d",i11);
	gprs_jiaoyanwei.Format("%d",i5);
	::WritePrivateProfileString("ConfigInfo","gprs_chuankou",gprs_chuankou,sFilePath);
	::WritePrivateProfileString("ConfigInfo","gprs_botelv",gprs_botelv,sFilePath);
	::WritePrivateProfileString("ConfigInfo","gprs_shujuwei",gprs_shujuwei,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","gprs_tingzhiwei",gprs_tingzhiwei,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","gprs_jiaoyanwei",gprs_jiaoyanwei,sFilePath);

	radar_chuankou.Format("%d",Radar_m_nCom);
	radar_botelv.Format("%d",i3);
	radar_shujuwei.Format("%d",i9);
	radar_tingzhiwei.Format("%d",i12);
	radar_jiaoyanwei.Format("%d",i6);
	::WritePrivateProfileString("ConfigInfo","radar_chuankou",radar_chuankou,sFilePath);
	::WritePrivateProfileString("ConfigInfo","radar_botelv",radar_botelv,sFilePath);
	::WritePrivateProfileString("ConfigInfo","radar_shujuwei",radar_shujuwei,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","radar_tingzhiwei",radar_tingzhiwei,sFilePath);//ip
	::WritePrivateProfileString("ConfigInfo","radar_jiaoyanwei",radar_jiaoyanwei,sFilePath);

	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK1);
	int state1 = pBtn1->GetCheck();
	CString radio1;
	radio1.Format("%d",state1);
	::WritePrivateProfileString("ConfigInfo","radio1",radio1,sFilePath);

	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_CHECK2);
	int state2 = pBtn2->GetCheck();
	CString radio2;
	radio2.Format("%d",state2);
	::WritePrivateProfileString("ConfigInfo","radio2",radio2,sFilePath);

	CButton* pBtn3 = (CButton*)GetDlgItem(IDC_CHECK3);
	int state3 = pBtn3->GetCheck();
	CString radio3;
	radio3.Format("%d",state3);
	::WritePrivateProfileString("ConfigInfo","radio3",radio3,sFilePath);

	CButton* pBtn4 = (CButton*)GetDlgItem(IDC_CHECK4);
	int state4 = pBtn4->GetCheck();
	CString radio4;
	radio4.Format("%d",state4);
	::WritePrivateProfileString("ConfigInfo","radio4",radio4,sFilePath);
	//当state == 1时表示该复选框没有被选中；
	//当state == 0时表示该复选框被选中；

	// 设置预警参数
	CString str_AlarmP[6];
	GetDlgItemText(IDC_REVISEFACTOR, str_AlarmP[0]);
	GetDlgItemText(IDC_TIME1,str_AlarmP[1]);
	GetDlgItemText(IDC_TIME2,str_AlarmP[2]);
	GetDlgItemText(IDC_TIME3,str_AlarmP[3]);
	GetDlgItemText(IDC_SAFEDISTANCE,str_AlarmP[4]);
	GetDlgItemText(IDC_ALARMACCEL,str_AlarmP[5]);

	for (int i = 0; i < 6 ;i ++)
	{
		CString tmp;
		tmp.Format("%d", i);
		tmp = "AlarmPara" + tmp;
		::WritePrivateProfileString("ConfigInfo", tmp, str_AlarmP[i], sFilePath);
		AlarmP[i] = (atof(str_AlarmP[i].GetBuffer()));		
	}
	


	CDialogEx::OnOK();
}
CString  CSet::GetDvrIP()//获得ip控件上的ip地址
{
	CString strRet="";
	BYTE nField0,nField1,nField2,nField3;
	m_DvrIPAddr.GetAddress(nField0,nField1,nField2,nField3);
	strRet.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	return strRet;
}

void CSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CSet::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	GPS_m_nCom=GPS_chuankou.GetCurSel();
}

void CSet::OnCbnSelchangeCombo11()
{
	// TODO: 在此添加控件通知处理程序代码
	GPRS_m_nCom=GPRS_chuankou.GetCurSel();
}

void CSet::OnCbnSelchangeCombo7()
{
	// TODO: 在此添加控件通知处理程序代码
	Radar_m_nCom=Radar_chuankou.GetCurSel();
}

void CSet::OnCbnSelchangeCombo4()
{
	// TODO: 在此添加控件通知处理程序代码
	i1=GPS_botelv.GetCurSel();
	switch(i1)
	{
	case 0:
		GPS_m_nBaud=300;
		break;
	case 1:
		GPS_m_nBaud=600;
		break;
	case 2:
		GPS_m_nBaud=1200;
		break;
	case 3:
		GPS_m_nBaud=2400;
		break;
	case 4:
		GPS_m_nBaud=4800;
		break;
	case 5:
		GPS_m_nBaud=9600;
		break;
	case 6:
		GPS_m_nBaud=19200;
		break;
	case 7:
		GPS_m_nBaud=38400;
		break;
	case 8:
		GPS_m_nBaud=43000;
		break;
	case 9:
		GPS_m_nBaud=56000;
		break;
	case 10:
		GPS_m_nBaud=57600;
		break;
	case 11:
		GPS_m_nBaud=115200;
		break;
	default:
		break;

	}
	//GPS_m_nBaud=i;
}


void CSet::OnCbnSelchangeCombo12()
{
	// TODO: 在此添加控件通知处理程序代码
	i2=GPRS_botelv.GetCurSel();
	switch(i2)
	{
	case 0:
		GPRS_m_nBaud=300;
		break;
	case 1:
		GPRS_m_nBaud=600;
		break;
	case 2:
		GPRS_m_nBaud=1200;
		break;
	case 3:
		GPRS_m_nBaud=2400;
		break;
	case 4:
		GPRS_m_nBaud=4800;
		break;
	case 5:
		GPRS_m_nBaud=9600;
		break;
	case 6:
		GPRS_m_nBaud=19200;
		break;
	case 7:
		GPRS_m_nBaud=38400;
		break;
	case 8:
		GPRS_m_nBaud=43000;
		break;
	case 9:
		GPRS_m_nBaud=56000;
		break;
	case 10:
		GPRS_m_nBaud=57600;
		break;
	case 11:
		GPRS_m_nBaud=115200;
		break;
	default:
		break;

	}
	//GPRS_m_nBaud=i;
}


void CSet::OnCbnSelchangeCombo8()
{
	// TODO: 在此添加控件通知处理程序代码
	i3=Radar_botelv.GetCurSel();
	switch(i3)
	{
	case 0:
		Radar_m_nBaud=300;
		break;
	case 1:
		Radar_m_nBaud=600;
		break;
	case 2:
		Radar_m_nBaud=1200;
		break;
	case 3:
		Radar_m_nBaud=2400;
		break;
	case 4:
		Radar_m_nBaud=4800;
		break;
	case 5:
		Radar_m_nBaud=9600;
		break;
	case 6:
		Radar_m_nBaud=19200;
		break;
	case 7:
		Radar_m_nBaud=38400;
		break;
	case 8:
		Radar_m_nBaud=43000;
		break;
	case 9:
		Radar_m_nBaud=56000;
		break;
	case 10:
		Radar_m_nBaud=57600;
		break;
	case 11:
		Radar_m_nBaud=115200;
		break;
	default:
		break;

	}
	//Radar_m_nBaud=i;
}


void CSet::OnCbnSelchangeCombo17()
{
	// TODO: 在此添加控件通知处理程序代码
	//char temp;
	i4=GPS_jiaoyanwei.GetCurSel();
	switch(i4)
	{
	case 0:
		GPS_m_cParity='N';
		break;
	case 1:
		GPS_m_cParity='O';
		break;
	case 2:
		GPS_m_cParity='E';
	case 4:
		GPS_m_cParity='M';
		break;
	case 5:
		GPS_m_cParity='S';
		break;
	}
	//GPS_m_cParity=temp;
}

void CSet::OnCbnSelchangeCombo15()
{
	// TODO: 在此添加控件通知处理程序代码
	//char temp;
	i5=GPRS_jiaoyanwei.GetCurSel();
	switch(i5)
	{
	case 0:
		GPRS_m_cParity='N';
		break;
	case 1:
		GPRS_m_cParity='O';
		break;
	case 2:
		GPRS_m_cParity='E';
	case 4:
		GPRS_m_cParity='M';
		break;
	case 5:
		GPRS_m_cParity='S';
		break;
	}
	//GPRS_m_cParity=temp;
}

void CSet::OnCbnSelchangeCombo10()
{
	// TODO: 在此添加控件通知处理程序代码
	//char temp;
	i6=Radar_jiaoyanwei.GetCurSel();
	switch(i6)
	{
	case 0:
		Radar_m_cParity='N';
		break;
	case 1:
		Radar_m_cParity='O';
		break;
	case 2:
		Radar_m_cParity='E';
	case 4:
		Radar_m_cParity='M';
		break;
	case 5:
		Radar_m_cParity='S';
		break;
	}
	//Radar_m_cParity=temp;
}

void CSet::OnCbnSelchangeCombo5()
{
	// TODO: 在此添加控件通知处理程序代码
	i7=GPS_shujuwei.GetCurSel();
	switch(i7)
	{
		case 0:
			GPS_m_nDatabits=1;
			break;
		case 1:
			GPS_m_nDatabits=2;
			break;
		case 2:
			GPS_m_nDatabits=3;
		case 3:
			GPS_m_nDatabits=4;
			break;
		case 4:
			GPS_m_nDatabits=5;
			break;
		case 5:
			GPS_m_nDatabits=6;
		case 6:
			GPS_m_nDatabits=7;
			break;
		case 7:
			GPS_m_nDatabits=8;
		    break;
	}
	//GPS_m_nDatabits=i;
}


void CSet::OnCbnSelchangeCombo13()
{
	// TODO: 在此添加控件通知处理程序代码
	i8=GPRS_shujuwei.GetCurSel();
	switch(i8)
	{
		case 0:
			GPRS_m_nDatabits=1;
			break;
		case 1:
			GPRS_m_nDatabits=2;
			break;
		case 2:
			GPRS_m_nDatabits=3;
		case 3:
			GPRS_m_nDatabits=4;
			break;
		case 4:
			GPRS_m_nDatabits=5;
			break;
		case 5:
			GPRS_m_nDatabits=6;
		case 6:
			GPRS_m_nDatabits=7;
			break;
		case 7:
			GPRS_m_nDatabits=8;
		    break;
	}
	//GPRS_m_nDatabits=i;

}

void CSet::OnCbnSelchangeCombo9()
{
	// TODO: 在此添加控件通知处理程序代码
	i9=Radar_shujuwei.GetCurSel();
	switch(i9)
	{
		case 0:
			Radar_m_nDatabits=1;
			break;
		case 1:
			Radar_m_nDatabits=2;
			break;
		case 2:
			Radar_m_nDatabits=3;
		case 3:
			Radar_m_nDatabits=4;
			break;
		case 4:
			Radar_m_nDatabits=5;
			break;
		case 5:
			Radar_m_nDatabits=6;
		case 6:
			Radar_m_nDatabits=7;
			break;
		case 7:
			Radar_m_nDatabits=8;
		    break;
	}
	//Radar_m_nDatabits=i;
}

void CSet::OnCbnSelchangeCombo6()
{
	// TODO: 在此添加控件通知处理程序代码
	i10=GPS_tingzhiwei.GetCurSel();
	switch(i10)
	{
	case 0:
		GPS_m_nStopbits=1;
		break;
	case 1:
		GPS_m_nStopbits=2;
		break;
	}
	//GPS_m_nStopbits=i;
}

void CSet::OnCbnSelchangeCombo14()
{
	// TODO: 在此添加控件通知处理程序代码
	i11=GPRS_tingzhiwei.GetCurSel();
	switch(i11)
	{
	case 0:
		GPRS_m_nStopbits=1;
		break;
	case 1:
		GPRS_m_nStopbits=2;
		break;
	}
	//GPRS_m_nStopbits=i;
}

void CSet::OnCbnSelchangeCombo16()
{
	// TODO: 在此添加控件通知处理程序代码
	i12=Radar_tingzhiwei.GetCurSel();
	switch(i12)
	{
	case 0:
		Radar_m_nStopbits=1;
		break;
	case 1:
		Radar_m_nStopbits=2;
		break;
	}
	//Radar_m_nStopbits=i;
}


void CSet::OnCbnSelchangeComboAlarm()
{
	// TODO: 在此添加控件通知处理程序代码
	int index_num = Combox_SetAlarmPara.GetCurSel();
	CString m_str1[6] = {"1.1","1.2","0.2","0.15","5","4"};
	CString m_str2[6] = {"1","1","0.2","0.15","4","5"};
	switch(index_num)
	{
	case 0:
		// 预警信息设置		
		SetDlgItemText(IDC_REVISEFACTOR, m_str1[0]);
		SetDlgItemText(IDC_TIME1,m_str1[1]);
		SetDlgItemText(IDC_TIME2,m_str1[2]);
		SetDlgItemText(IDC_TIME3,m_str1[3]);
		SetDlgItemText(IDC_SAFEDISTANCE,m_str1[4]);
		SetDlgItemText(IDC_ALARMACCEL,m_str1[5]);
		break;
	case 1:		
		SetDlgItemText(IDC_REVISEFACTOR, m_str2[0]);
		SetDlgItemText(IDC_TIME1,m_str2[1]);
		SetDlgItemText(IDC_TIME2,m_str2[2]);
		SetDlgItemText(IDC_TIME3,m_str2[3]);
		SetDlgItemText(IDC_SAFEDISTANCE,m_str2[4]);
		SetDlgItemText(IDC_ALARMACCEL,m_str2[5]);
		break;
	}
}
