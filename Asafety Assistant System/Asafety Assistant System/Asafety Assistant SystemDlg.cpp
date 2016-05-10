
// Asafety Assistant SystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Asafety Assistant System.h"
#include "Asafety Assistant SystemDlg.h"
#include "afxdialogex.h"
#include "Set.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SETLUJIN.h"
char* USART1_RX_BUF1={"$GPGGA,121252.000,3937.3032,N,11611.6046,E,1,05,2.0,45.9,M,-5.7,M,,0000*77"};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
HWND g_hWnd = NULL;
char USART1_RX_BUF[256];     //接收缓冲,最大USART_REC_LEN个字节.
int USART2_RX_BUF[256];     //接收缓冲,最大USART_REC_LEN个字节.

int USART1_RX_STA=0;         		//串口1接收状态标记	
int USART2_RX_STA=0;         		//串口2接收状态标记
#define POSYFILTER  2  //显示y方向偏差在左右POSYFITER范围内的数据



//CString filePathName = "carInfo.log";
//
//CFile g_File(filePathName, CFile::modeWrite | CFile::modeCreate); // for test
class Real_Top_DATA//这个类用来当作 节点使用，存放当前点的温度值和时间
{
	public:
		Real_Top_DATA()
		{
			//jingdu="";//车道
		    //weidu="";//车牌号
		    //gaocheng="";//高程
		}
	public:
		//CString jingdu;//经度
		//CString weidu;//纬度
		//CString gaocheng;//高程
		char BUF[256];
};
typedef CList<Real_Top_DATA, Real_Top_DATA&>  Real_Top_Lst;
class Real_T_DATA//这个类用来当作 节点使用，存放当前点的温度值和时间
{
public:
	Real_T_DATA()
	{
		//a[0]=0;
		//a[1]=0;
		//a[2]=0;
		//a[3]=0;
		//a[4]=0;
		//a[5]=0;
		//a[6]=0;
		//a[7]=0;
		//a[8]=0;
		//a[9]=0;
		//a[10]=0;
		memset(a, 0, sizeof(a));
		byte_num = 0;
	}
public:
	int a[256];
	int byte_num;
};
typedef CList<Real_T_DATA, Real_T_DATA&>  Real_T_Lst;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAsafetyAssistantSystemDlg 对话框




CAsafetyAssistantSystemDlg::CAsafetyAssistantSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAsafetyAssistantSystemDlg::IDD, pParent)
	, gps_show(_T(""))
	, Radar_show(_T(""))
	, gprs_show(_T(""))
{

	m_posX = 0;
	m_posY = 0;
	m_posZoom = 0;
	m_CurScreen =0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_lPlayHandle=-1;
	firstfindgps=0;
	//gps_speed = 0;

	memset(&m_carInfo, 0, sizeof(Car_Info));
	memset(myCar, 0, sizeof(myCar));
	memset(forwardCar, 0, sizeof(forwardCar));

	memset(alarmPara, 0, sizeof(alarmPara));
	nearestCar_Accle = 0;
	nearestCar_Distance = 0;
	nearestCar_Vel = 0;
	nearestCar_flag = false;

	memset(isAlarmfalg, 0, sizeof(isAlarmfalg));
	memset(time_count, 0, sizeof(time_count));
	alarmLast = 0;
}

void CAsafetyAssistantSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT3, gpsshow);
	//DDX_Text(pDX, IDC_EDIT3, gps_show);
	//DDX_Text(pDX, IDC_EDIT4, Radar_show);
	DDX_Text(pDX, IDC_EDIT1, gprs_show);
	DDX_Control(pDX, IDC_COMBO1, m_box);
}

BEGIN_MESSAGE_MAP(CAsafetyAssistantSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE( WM_USERMESSAGE, OnMyMessage)//添加
	ON_MESSAGE( WM_USERMESSAGE1, OnMyMessage1)//添加
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_BN_CLICKED(IDC_BUTTON5, &CAsafetyAssistantSystemDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CAsafetyAssistantSystemDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON3, &CAsafetyAssistantSystemDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CAsafetyAssistantSystemDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CAsafetyAssistantSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CAsafetyAssistantSystemDlg::OnBnClickedButton4)
	
	//ON_BN_CLICKED(IDC_BUTTON7, &CAsafetyAssistantSystemDlg::OnBnClickedButton7)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_jingdu, &CAsafetyAssistantSystemDlg::OnStnClickedjingdu)
	ON_STN_CLICKED(IDC_gaocheng, &CAsafetyAssistantSystemDlg::OnStnClickedgaocheng)
	ON_BN_CLICKED(IDC_BUTTON7, &CAsafetyAssistantSystemDlg::OnBnClickedButton7)
	ON_EN_CHANGE(IDC_EDIT1, &CAsafetyAssistantSystemDlg::OnEnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CAsafetyAssistantSystemDlg 消息处理程序
void CAsafetyAssistantSystemDlg::FullScreenModeOn() 
{ 
    GetWindowPlacement( &m_OldWndPlacement); 
    CRect WindowRect; 
    GetWindowRect(&WindowRect); 
    CRect ClientRect; 
    RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect); 
    ClientToScreen(&ClientRect);

    // 获取屏幕的分辨率

    int nFullWidth=GetSystemMetrics(SM_CXSCREEN); 
    int nFullHeight=GetSystemMetrics(SM_CYSCREEN); 
    
    //将除控制条外的客户区全屏显示到从(0,0)到(nFullWidth, nFullHeight)区域, 
    //将(0,0)和(nFullWidth, nFullHeight)两个点外扩充原窗口和除控制条之外的 客户区位置间的差值, 就得到全屏显示的窗口位置 
    m_FullScreenRect.left = WindowRect.left - ClientRect.left; 
    m_FullScreenRect.top = WindowRect.top - ClientRect.top; 
    m_FullScreenRect.right = WindowRect.right - ClientRect.right + nFullWidth; 
    m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight; 
    m_bFullScreenMode = TRUE; 
    
    //设置全屏显示标志为 TRUE 
    //进入全屏显示状态 
    WINDOWPLACEMENT wndpl; 
    wndpl.length=sizeof(WINDOWPLACEMENT); 
    wndpl.flags=0; 
    wndpl.showCmd=SW_SHOWNORMAL; 
    wndpl.rcNormalPosition=m_FullScreenRect; 
    SetWindowPlacement(&wndpl);

    m_bFullScreenMode=true; 
	licheng=0;
}

void CAsafetyAssistantSystemDlg::FullScreenModeOff() 
{ 
    ShowWindow(SW_HIDE); 
    SetWindowPlacement(&m_OldWndPlacement); 
    m_bFullScreenMode=false; 
}




BOOL CAsafetyAssistantSystemDlg::OnInitDialog()
{
	skinppLoadSkin(_T("xp_corona.ssk"));
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//FullScreenModeOn() ;//全屏显示函数
	//ShowWindow(SW_SHOWMAXIMIZED);
	GPS_m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	GPRS_m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	Radar_m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	// TODO: 在此添加额外的初始化代码
	SetWindowText(_T("行车辅助安全预警软件"));
		//如下几句是创建一个蓝色屏幕
	m_ptzScreen.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,1981);
	init_all_data();//初始化所有需要初始化的变量
	UpdataScreenPos();
	m_ptzScreen.ShowWindow(SW_SHOW);
	m_ptzScreen.SetCallBack(MessageProcFunc,(DWORD)this,GetParamsFunc,(DWORD)this,SetParamsFunc,(DWORD)this,RectEventFunc,(DWORD)this);
	m_ptzScreen.SetShowPlayWin(SPLIT1,0);
	if(FALSE==NET_DVR_Init())//TRUE表示成功，FALSE表示失败
	{
		 AfxMessageBox("海康sdk初始化失败!");
	}
	g_hWnd = this->m_hWnd;
    initdata();
	
	m_box.InsertString(m_box.GetCount(),_T("前方有雾"));
	m_box.InsertString(m_box.GetCount(),_T("前方浓雾"));
	m_box.InsertString(m_box.GetCount(),_T("当前小雨天气"));
	m_box.InsertString(m_box.GetCount(),_T("当前中雨天气"));
	m_box.InsertString(m_box.GetCount(),_T("当前大到暴雨天气"));
	m_box.InsertString(m_box.GetCount(),_T("当前小雪天气"));
	m_box.InsertString(m_box.GetCount(),_T("当前中雪天气"));
	m_box.InsertString(m_box.GetCount(),_T("当前大到暴雪天气"));
	m_box.InsertString(m_box.GetCount(),_T("出现沙尘天气"));
	m_box.InsertString(m_box.GetCount(),_T("出现严重沙尘暴天气"));
	m_box.InsertString(m_box.GetCount(),_T("出现强对流雷暴天气"));
	m_box.InsertString(m_box.GetCount(),_T("当前风力较大"));
	m_box.InsertString(m_box.GetCount(),_T("当前风力很大"));
	m_box.InsertString(m_box.GetCount(),_T("路面潮湿"));
	m_box.InsertString(m_box.GetCount(),_T("路面有积水现象"));
	m_box.InsertString(m_box.GetCount(),_T("路面开始有积雪"));
	m_box.InsertString(m_box.GetCount(),_T("路面积雪小于3cm"));
	m_box.InsertString(m_box.GetCount(),_T("路面积雪大于3cm"));
	m_box.InsertString(m_box.GetCount(),_T("当前为冻雨天气"));
	m_box.InsertString(m_box.GetCount(),_T("路面局部出现结冰"));
	m_box.InsertString(m_box.GetCount(),_T("路面出现大部分凝冻结冰"));
	m_box.InsertString(m_box.GetCount(),_T("前方路面出现塌陷"));
	m_box.InsertString(m_box.GetCount(),_T("前方路基出现坍塌"));
	m_box.InsertString(m_box.GetCount(),_T("前方边坡出现滑坡"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通事故"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通事故 正在处理中"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通事故 有人员伤亡"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通事故 拥堵严重"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通事故 单方向交通中断"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通流量大"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通流量大 出现拥堵"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通流量大 出现严重拥堵"));
	m_box.InsertString(m_box.GetCount(),_T("严重拥堵 正在疏散缓解"));
	m_box.InsertString(m_box.GetCount(),_T("严重拥堵 预计持续15分钟以上"));
	m_box.InsertString(m_box.GetCount(),_T("严重拥堵 预计持续30分钟以上"));
	m_box.InsertString(m_box.GetCount(),_T("严重拥堵 预计持续60分钟以上"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通管制"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通管制 正采取强制分流措施"));
	m_box.InsertString(m_box.GetCount(),_T("前方交通管制 勤务车辆通行"));
	m_box.InsertString(m_box.GetCount(),_T("该路段施工养护作业"));
	m_box.InsertString(m_box.GetCount(),_T("该路段施工养护作业  交通拥堵"));

	m_box.SetCurSel(1);
	CWnd*   pEdit;   
	pEdit   =   GetDlgItem(IDC_EDIT1);   
	 ASSERT(pEdit   !=   NULL);   
    CFont font;
	font.CreateFont(
    20,                        // nHeight
    2,                        // nWidth
    0,                        // nEscapement
    0,                        // nOrientation
    FW_NORMAL,                // nWeight
    FALSE,                    // bItalic
    FALSE,                    // bUnderline
    0,                        // cStrikeOut
    ANSI_CHARSET,              // nCharSet
    OUT_DEFAULT_PRECIS,        // nOutPrecision
    CLIP_DEFAULT_PRECIS,      // nClipPrecision
    DEFAULT_QUALITY,          // nQuality
    DEFAULT_PITCH | FF_SWISS,
    _T("Arial")                    // nPitchAndFamily
    ); 
	 //HFONT   hFont   =   (HFONT)::GetStockObject(1);   
	//CFont*   pFont   =   CFont::FromHandle(hFont);   
	pEdit->SetFont(&font); 
	// 方法二：加载资源图片  
  
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(),   MAKEINTRESOURCE(IDB_BITMAP_TEST)); // IDB_BITMAP_TEST为资源图片ID 
	//((CButton *)GetDlgItem(IDC_BUTTON8))->SetBitmap(hBitmap); 
	//HBITMAP hBitmap;   
    //关联图片ID  
    hBitmap =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BITMAP_TEST),  IMAGE_BITMAP,0,0,  LR_LOADMAP3DCOLORS);    
	hBitmap_1 =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BITMAP_WIHTE),  IMAGE_BITMAP,0,0,  LR_LOADMAP3DCOLORS);    
    //获得指向静态控件的指针    
    //CStatic *pStatic=(CStatic *)GetDlgItem(IDC_STATIC111);//控件ID     
    //设置静态控件的样式，使其可以使用位图，并试位标显示使居中    
    //pStatic->ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);     
    //设置静态控件显示位图    
    //pStatic->SetBitmap(hBitmap);

	

	//图片控件
	pWnd = GetDlgItem(IDC_STATIC_DRAW);
	pWnd->GetClientRect(&rc);
	pDC = pWnd->GetDC();
	pWnd->GetWindowRect(&m_rc);
	ScreenToClient(m_rc);

	bitmap.LoadBitmap(IDB_BITMAP_ROAD);


	(GetDlgItem(IDC_STATIC_ALARMLIGHT))->GetWindowRect(&alarmLamp_rc);
	ScreenToClient(alarmLamp_rc);

	// for test
	CString filePathName = "-carInfo.log";
	CTime m_time = CTime::GetCurrentTime();
	CString str = m_time.Format("%c");
	str.Replace('/', '-');
	str.Replace(':', '-');
	filePathName = str + filePathName;

	g_File.Open(filePathName, CFile::modeWrite | CFile::modeCreate); // for test
	pWnd = GetDlgItem(IDC_STATIC_ALARMLIGHT);
	pmDC = pWnd->GetDC();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CAsafetyAssistantSystemDlg::initdata()
{
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\";//得到xml文件的绝对路劲ipictype1
	CString lujing;
	GetPrivateProfileString("ConfigInfo","tplujin","C:\\Users\\Administrator\\Desktop\\tanyiming\\photo",lujing.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	picdizhi=lujing;
	lujing.ReleaseBuffer();
	GetPrivateProfileString("ConfigInfo","videolujin","C:\\Users\\Administrator\\Desktop\\tanyiming\\video",lujing.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	videodizhi=lujing;
	lujing.ReleaseBuffer();
}
void CAsafetyAssistantSystemDlg::init_all_data()//初始化所有需要初始化的变量
{
	m_CurScreen =0;
	m_posX = 0;
	m_posY = 0;
	m_posZoom = 0;

}
void  CAsafetyAssistantSystemDlg::UpdataScreenPos()//将当前场景对齐到图片控件上
{
	//GetDlgItem(IDC_SCREEN_WINDOW)->GetClientRect(&m_clientRect);
	 GetDlgItem(IDC_SCREEN_WINDOW)->GetWindowRect(&m_clientRect);   //取得控件的全屏坐标， m_edit为一个Edit控件
    this->ScreenToClient(&m_clientRect);        //转成客户窗口坐标，this指向控件所在的窗口。
	m_screenRect = m_clientRect;
	m_ptzScreen.MoveWindow(m_screenRect);
	//////////////
	//CRect rect;
   

}
void  CAsafetyAssistantSystemDlg::MessageProc(int nWndID, UINT message)//在屏幕上左键或者右键单击的时候，让当前选择的画面红亮显示，即激活当前选中的画面
{
	switch(message)
	{
	case WM_LBUTTONDOWN:
	
		{
			m_CurScreen =nWndID;
//			m_comboDispNum.SetCurSel(m_CurScreen);
			//SetCurWindId(nWndID);
			//UpdateCurScreenInfo();
		}
		break;
	case WM_RBUTTONDOWN:
		break;
	default:
		break;
	}
	
}
BOOL  CAsafetyAssistantSystemDlg::GetParams(int nWndID, int type)
{
	BOOL bRet = FALSE;
	
	if (type == 0)
	{
		bRet = GetExitDecode(nWndID);
	}
	else if (type == 1)
	{
		bRet = GetExitCycle(nWndID);
	}
	
	return bRet;
}
void  CAsafetyAssistantSystemDlg::SetParams(int nWndID, int type)
{
	if (type == 0)
	{
		SetExitDecode(nWndID);
	}
	else if (type == 1)
	{
		SetExitCycle(nWndID);
	}

}
BOOL  CAsafetyAssistantSystemDlg::GetExitDecode(int nCurWndID)
{
	BOOL bRet = FALSE;
	if (nCurWndID<0 || nCurWndID>=16)
	{
		return bRet;
	}
	
	BOOL bIsTimeOut = m_cs.Lock();
	if(bIsTimeOut)
	{
		bRet = m_bWndExitDecode[nCurWndID];
		m_cs.Unlock();
	}
	
	return bRet;
}
BOOL  CAsafetyAssistantSystemDlg::GetExitCycle(int nCurWndID)
{
	BOOL bRet = FALSE;
	if (nCurWndID<0 || nCurWndID>=16)
	{
		return bRet;
	}
	
	bRet = m_bWndExitCycle[nCurWndID];
	
	return bRet;
}
void  CAsafetyAssistantSystemDlg::SetExitDecode(int nCurWndID)
{
	if (nCurWndID<0 || nCurWndID>=16)
	{
		return;
	}
	
	BOOL bIsTimeOut = m_cs.Lock();
	if(bIsTimeOut)
	{
		m_bWndExitDecode[nCurWndID] = !m_bWndExitDecode[nCurWndID];
		m_cs.Unlock();
	}
}
void  CAsafetyAssistantSystemDlg::SetExitCycle(int nCurWndID)
{
	if (nCurWndID<0 || nCurWndID>=16)
	{
		return;
	}
	
	m_bWndExitCycle[nCurWndID] = !m_bWndExitCycle[nCurWndID];
}
void CALLBACK  MessageProcFunc(int nWndID, UINT message, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return;
	}
	
	CAsafetyAssistantSystemDlg *dlg = (CAsafetyAssistantSystemDlg *)dwUser;
	dlg->MessageProc(nWndID, message);

}
BOOL CALLBACK  GetParamsFunc(int nWndID, int type, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return FALSE;
	}
	CAsafetyAssistantSystemDlg *dlg = (CAsafetyAssistantSystemDlg *)dwUser;
	return dlg->GetParams(nWndID, type);
}
void CALLBACK SetParamsFunc(int nWndID, int type, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return;
	}
	
	CAsafetyAssistantSystemDlg *dlg = (CAsafetyAssistantSystemDlg *)dwUser;
	dlg->SetParams(nWndID, type);
}
/************************************************************************/
/* Callback function                                                      */
/************************************************************************/
void CALLBACK RectEventFunc(RECT WinRect,CPoint &pointStart,CPoint &pointEnd,DWORD dwUser)
{
	CAsafetyAssistantSystemDlg *dlg =(CAsafetyAssistantSystemDlg *)dwUser;
	CPoint Origin;
	CPoint SendPoint;
	
	Origin.x = ( WinRect.left + WinRect.right ) / 2;
	Origin.y = ( WinRect.top + WinRect.bottom ) / 2;
	
	int dx = (pointStart.x + pointEnd.x)/2;
	int dy = (pointStart.y + pointEnd.y)/2;
	
	int width  = WinRect.right - WinRect.left;
	int height = WinRect.bottom - WinRect.top; 
	
	SendPoint.x = ( dx - Origin.x) * 8192 * 2 / width;
	SendPoint.y = ( dy - Origin.y) * 8192 * 2  / height ;
	
	int width2 = pointEnd.x - pointStart.x;
	int height2 = pointEnd.y - pointStart.y;
	int multiple = 0;

	if ( height2 !=0 && width2!=0)
	{
		multiple = (width * height) / (width2 * height2);
	}
	
	dlg->m_posX = SendPoint.x;
	dlg->m_posY = SendPoint.y;
	dlg->m_posZoom = multiple;
	dlg->UpdateData(FALSE);
	//dlg->PtzExtControl(DH_EXTPTZ_FASTGOTO);
}
void CAsafetyAssistantSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAsafetyAssistantSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	//绘制界面
	DrawBackground();

	for (int i=0; i < m_carInfo.car_num; i++)
	{
		//if (abs(m_carInfo.PosCarY[i]) < 2)
		{
			DrawACar(m_carInfo.PosCarX[i], m_carInfo.PosCarY[i], m_carInfo.VelocityX[i], m_carInfo.ID[i]);
			
		}		
	}
	//for (int i=0; i < 7; i++)
	//{
	//	//if (abs(m_carInfo.PosCarY[i]) < 2)
	//	{
	//		DrawACar(i*10, (i - 2)*2, i*10, i);
	//		
	//	}		
	//}	// for test
	memset(&m_carInfo, 0, sizeof(m_carInfo));
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAsafetyAssistantSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CAsafetyAssistantSystemDlg::DoGetDeviceResoureCfg()
{
	NET_DVR_IPPARACFG IpAccessCfg;
	memset(&IpAccessCfg,0,sizeof(IpAccessCfg));	//清0
	DWORD  dwReturned;

	m_struDeviceInfo.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_IPPARACFG,0,&IpAccessCfg,sizeof(NET_DVR_IPPARACFG),&dwReturned);

	int i;
    if(!m_struDeviceInfo.bIPRet)   //不支持ip接入,9000以下设备不支持禁用模拟通道
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf_s(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;  //通道号
				m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
				
			}
			else
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf_s(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
			}
		}
	}
	else        //支持IP接入，9000设备
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //模拟通道
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf_s(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;
				if(IpAccessCfg.byAnalogChanEnable[i])
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
				}
				else
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				}
				
			}
			else//clear the state of other channel
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf_s(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
			}		
		}

		//数字通道
		for(i=0; i<MAX_IP_CHANNEL; i++)
		{
			if(IpAccessCfg.struIPChanInfo[i].byEnable)  //ip通道在线
			{
				m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = TRUE;
                m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = IpAccessCfg.struIPChanInfo[i].byChannel;
				sprintf_s(m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].chChanName,"IP Camera %d",IpAccessCfg.struIPChanInfo[i].byChannel);

			}
			else
			{
               m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = FALSE;
			    m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = -1;
			}
		}
		
		
	} //支持IP接入，9000设备

}
void  CAsafetyAssistantSystemDlg::GetDecoderCfg()//解码器信息也放在m_struDeviceInfo1.struChanInfo[i].struDecodercfg中存放
{
     NET_DVR_DECODERCFG_V30 DecoderCfg;//用来存放解码器信息
	 DWORD  dwReturned;
	 BOOL bRet;
		
      
	 //获取通道解码器信息
	 for(int i=0; i<MAX_CHANNUM_V30; i++)
	 {
		 if(m_struDeviceInfo.struChanInfo[i].bEnable)//如果通道有效
		 {
			 memset(&DecoderCfg,0,sizeof(NET_DVR_DECODERCFG_V30));//清空
			 bRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_DECODERCFG_V30 , \
				 m_struDeviceInfo.struChanInfo[i].iChanIndex,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30),&dwReturned);
			 if(!bRet)
			 {
				 TRACE("Get DecderCfg failed,Chan:%d\n",m_struDeviceInfo.struChanInfo[i].iChanIndex);
				 continue;
			 }
	
			 memcpy(&m_struDeviceInfo.struChanInfo[i].struDecodercfg,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30));
		 }
		 
	 }

}
bool  CAsafetyAssistantSystemDlg::OnBTLogin(CString strIP,int Port,CString ADMIN,CString PassWord) //登陆按钮，取得各个控件上的服务器ip地址，端口号，用户名和密码连接服务器，同时将当前服务器的视频通道数添加进通道数下拉框中
{//如果当前服务器没有连接任何摄像头，此时服务器就当录像回放器使用，用来播放录像，此时通道数下显示Multi_Preview（多路回放）
			int err = 0;	//Storage the possible error return value.
			
			pchDVRIP = (LPSTR)(LPCSTR)strIP;
			WORD wDVRPort=(WORD)Port;//获得端口
			char *pchUserName=(LPSTR)(LPCSTR)ADMIN;//获得服务器用户名
			char *pchPassword=(LPSTR)(LPCSTR)PassWord;//获得密码
			NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;//存放的摄像头信息比较少
	        memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));

	        LONG lLoginID = NET_DVR_Login_V30(pchDVRIP, Port, pchUserName,pchPassword,&DeviceInfoTmp);
			if(-1 != lLoginID)//如果成功连接上服务器
			{
				 m_LoginID = lLoginID;//将当前服务器的登陆id存放起来，通过这个就可以判断当前是否成功登陆
				 m_struDeviceInfo.lLoginID = lLoginID;//获取到登陆id
				 m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;//模拟通道数量
				 m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;//数字摄像头的数量
				 m_struDeviceInfo.iStartChan  = DeviceInfoTmp.byStartChan;//起始地址
				 DoGetDeviceResoureCfg();  //获取设备资源信息,
				 //这样服务器最新的信息放在了m_struDeviceInfo1结构体中存放
				// m_nChannelCount = m_struDeviceInfo[hh].iDeviceChanNum+m_struDeviceInfo[hh].iIPChanNum;//取得当前服务器的模拟通道总数，这个时候需要注意，有的视频服务器接的是模拟摄像头，但是也有的视频服务器接的是数字摄像头
				//如果当前视频服务器是数字视频服务器的话，模拟通道数自然为0， m_nChannelCount的值就为0
				 int nIndex = 0;
				 int j=-1;
					for(int i=0;i< MAX_IP_DEVICE_V40;i++)//对每一路通道进行操作，让每一路通道的名字出现在复选框中
				    {//m_nChannelCount中存放的是当前视频服务器的通道数，如果没有连接上值就为0，连接上之后就为实际的通道数量
					  if(m_struDeviceInfo.struChanInfo[i].bEnable)  //通道有效，插入到通道下拉框中
					  {
						  j++;
					  }
				    }//当前视频服务器有几个通道，就在这个通道数下拉框中添加几个项，从0开始
					PortNum=j+1;
					//CString a;
					//a.Format("通道数%d",PortNum);
					//MessageBox(a);
					if(PortNum<=0)
					{
						MessageBox("当前视频服务器没有可用的摄像头，请检查后重试");
						return 0;
					}
					GetDecoderCfg();//解码器信息也放在m_struDeviceInfo1.struChanInfo[i].struDecodercfg中存放
					return 1;
			}
			else//连接服务器失败
			{
				MessageBox("海康视频服务器连接失败，请重新连接!\n");
		        return 0;
			}
}
bool  CAsafetyAssistantSystemDlg::OnBTLeave() //单机了退出按钮，要求断开 视频服务器连接
{
	if(-1 != m_LoginID)//如果当前是登陆状态
		{
			BOOL bSuccess =NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); //退出登录
			if(bSuccess)//如果成功退出服务器
			{
				 m_LoginID = 0;
				 m_struDeviceInfo.lLoginID = -1;
				 return 1;
			}
		    else
			{
				  MessageBox("退出失败，请检查是否成功连接到海康视频服务器!!");
				  m_struDeviceInfo.lLoginID = -1;
				  return 0;
			}
			Invalidate();//刷新
			return 1;
		}
	else 
		return 1;
}
HWND   CAsafetyAssistantSystemDlg::GetDispHandle(int nNum)//根据参数的值，返回1--9这九个播放视频的静态文本框中其中一个的句柄
{
	m_CurScreen = nNum;//取得当前选中的画面
	m_ptzScreen.SetActiveWnd(m_CurScreen,TRUE);//当前画面被激活
	HWND hWnd=0;
	hWnd = ((CWnd *)(m_ptzScreen.GetPage(m_CurScreen)))->m_hWnd;
	return hWnd;
	switch(nNum) {
	case 1:
		hWnd = GetDlgItem(IDC_SCREEN_01)->m_hWnd;
		break;
	case 2:
		hWnd = GetDlgItem(IDC_SCREEN_02)->m_hWnd;
		break;
	case 3:
		hWnd = GetDlgItem(IDC_SCREEN_03)->m_hWnd;
		break;
	case 4:
		hWnd = GetDlgItem(IDC_SCREEN_04)->m_hWnd;
		break;
	case 5:
		hWnd = GetDlgItem(IDC_SCREEN_05)->m_hWnd;
		break;
	case 6:
		hWnd = GetDlgItem(IDC_SCREEN_06)->m_hWnd;
		break;
	case 7:
		hWnd = GetDlgItem(IDC_SCREEN_07)->m_hWnd;
		break;
	case 8:
		hWnd = GetDlgItem(IDC_SCREEN_08)->m_hWnd;
		break;
	case 9:
		hWnd = GetDlgItem(IDC_SCREEN_09)->m_hWnd;
		break;
	default:
		break;
	}
	return hWnd;
}
void CAsafetyAssistantSystemDlg::StopPlay()//让指定的通道的视频停止播放
{
	if(m_lPlayHandle != -1)
	{	
		NET_DVR_StopRealPlay(m_lPlayHandle);//停止播放指定的通道
		m_lPlayHandle=-1;
	}
}
void CAsafetyAssistantSystemDlg::start_haikang()//传递进去的是通道数,要显示的屏幕。设置让指定的通道视频流在制定的窗口上显示
{
	int i=MAX_IP_DEVICE_V40;
	int j=-1;
	while(i--)//对每一路通道进行操作，让每一路通道的名字出现在复选框中
	{//m_nChannelCount中存放的是当前视频服务器的通道数，如果没有连接上值就为0，连接上之后就为实际的通道数量
		if(m_struDeviceInfo.struChanInfo[MAX_IP_DEVICE_V40-i].bEnable)  //通道有效，插入到通道下拉框中
		{
			j++;
			if(j==1)
			{
				break;
			}
		}
	}
	//CString aaa;
	//aaa.Format("实际通道数%d",j);//这里j=0
	//MessageBox(aaa);
	NET_DVR_CLIENTINFO ClientInfo;
	HWND a=GetDispHandle(0);//取得当前静态文本框的句柄，也就是1--9那九个播放视频的控件中的某一个的句柄
	ClientInfo.hPlayWnd     = a;//取得当前图片控件的句柄。此时有sdk实现解码，直接将解码后的视频进行显示了，如果这里传递null，
	ClientInfo.lChannel     = MAX_IP_DEVICE_V40-j+1;//1;//m_struDeviceInfo1.struChanInfo[iChanIndex].iChanIndex;//取得当前通道号
	ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
	//setshowmode(ClientInfo.lChannel,"dongshuo23456789");
	if(-1== m_lPlayHandle)//如果当前没有开启预览的话，就开启预览，否则的话，直接退出。
	{
		m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);
	}
	else
		return;
	if(-1 == m_lPlayHandle)
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format("播放出错，错误代码%d",err);
		MessageBox(m_csErr);
	}
}
void CAsafetyAssistantSystemDlg::OnBnClickedButton5()//连接摄像机
{
	
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\" + sFile;//得到xml文件的绝对路劲ipictype1	
	CString cameraen;
	GetPrivateProfileString("ConfigInfo","radio1","0",cameraen.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	camera_EN=atoi(cameraen);
	CString gpsen;
	GetPrivateProfileString("ConfigInfo","radio2","0",gpsen.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GPS_EN=atoi(gpsen);
	CString gprsen;
	GetPrivateProfileString("ConfigInfo","radio3","0",gprsen.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GPRS_EN=atoi(gprsen);
	CString radaren;
	GetPrivateProfileString("ConfigInfo","radio4","0",radaren.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	Radar_EN=atoi(radaren);
	////////////////////////////////////
	CString serverIP,serverPort,ADMIN,PassWord;
	CString gps_chuankou,gps_botelv,gps_shujuwei,gps_tingzhiwei,gps_jiaoyanwei,gprs_chuankou,gprs_botelv,gprs_shujuwei,gprs_tingzhiwei,gprs_jiaoyanwei,radar_chuankou,radar_botelv,radar_shujuwei,radar_tingzhiwei,radar_jiaoyanwei;
	GetPrivateProfileString("ConfigInfo","ServerIP","192.168.1.64",serverIP.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","ServerPort","5000",serverPort.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GetPrivateProfileString("ConfigInfo","admin","admin",ADMIN.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//用户名
	GetPrivateProfileString("ConfigInfo","password","gonglusuo123456",PassWord.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//密码
	GetPrivateProfileString("ConfigInfo","gps_chuankou","1",     gps_chuankou.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gps_botelv","5",     gps_botelv.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gps_shujuwei","7",     gps_shujuwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gps_tingzhiwei","0",     gps_tingzhiwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gps_jiaoyanwei","0",     gps_jiaoyanwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gprs_chuankou","3",     gprs_chuankou.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gprs_botelv","5",     gprs_botelv.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gprs_shujuwei","7",     gprs_shujuwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gprs_tingzhiwei","0",gprs_tingzhiwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","gprs_jiaoyanwei","0",     gprs_jiaoyanwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","radar_chuankou","3",     radar_chuankou.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","radar_botelv","5",     radar_botelv.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","radar_shujuwei","7",     radar_shujuwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","radar_tingzhiwei","0",     radar_tingzhiwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","radar_jiaoyanwei","0",     radar_jiaoyanwei.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	OnCbnSelchangeCombo4(atoi(gps_botelv));
	
	OnCbnSelchangeCombo12(atoi(gprs_botelv));
	OnCbnSelchangeCombo8(atoi(radar_botelv));
	OnCbnSelchangeCombo17(atoi(gps_jiaoyanwei));
	OnCbnSelchangeCombo15(atoi(gprs_jiaoyanwei));
	OnCbnSelchangeCombo10(atoi(radar_jiaoyanwei));
	OnCbnSelchangeCombo5(atoi(gps_shujuwei));
	OnCbnSelchangeCombo13(atoi(gprs_shujuwei));
	OnCbnSelchangeCombo9(atoi(radar_shujuwei));
	OnCbnSelchangeCombo6(atoi(gps_tingzhiwei));
	OnCbnSelchangeCombo14(atoi(gprs_tingzhiwei));
	OnCbnSelchangeCombo16(atoi(radar_tingzhiwei));
	OnCbnSelchangeCombo1(atoi(gps_chuankou));
	OnCbnSelchangeCombo7(atoi(radar_chuankou));
	OnCbnSelchangeCombo11(atoi(gprs_chuankou));
	//pchDVRIP = (LPSTR)(LPCSTR)serverIP;
	//wDVRPort=(WORD)atoi(serverPort);//获得端口
	//pchUserName=(LPSTR)(LPCSTR)ADMIN;//获得服务器用户名
	//pchPassword=(LPSTR)(LPCSTR)PassWord;//获得密码
	
	////////////////////////////////////
	CString  s;
	GetDlgItemText(IDC_BUTTON5,s);
	if(0 == s.Compare("连接设备")) //相同
	{		
		if(camera_EN==1)
		{
			if(1==OnBTLogin(serverIP,atoi(serverPort),ADMIN,PassWord))
			{
				SetDlgItemText(IDC_BUTTON5,_T("断开连接"));
				start_haikang();//启动海康所有的通道
			}
			else
			{
				SetDlgItemText(IDC_BUTTON5,_T("连接设备"));
				StopPlay();//让指定的通道的视频停止播放
			}
		}
		
		if(GPS_EN==1)
		{
			OpenGPSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("断开连接"));
		}

		
		if(GPRS_EN==1)
		{
			OpenGPRSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("断开连接"));
		}

		
		if(Radar_EN==1)
		{
			OpenRadarchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("断开连接"));
		}
	}
	else
	{
		
		if(camera_EN==1)
		{
			StopPlay();
			OnBTLeave();//退出服务器连接
			SetDlgItemText(IDC_BUTTON5,_T("连接设备"));
		}
		if(GPS_EN==1)
		{
			CloseGPSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("连接设备"));
		}	
		if(GPRS_EN==1)
		{
			CloseGPRSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("连接设备"));
		}		
		if(Radar_EN==1)
		{
			CloseRadarchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("连接设备"));
		}
	}
	CString tmp_AlarmPara[6];
	// 预警参数
	GetPrivateProfileString("ConfigInfo","AlarmPara0", "1", tmp_AlarmPara[0].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara1", "1", tmp_AlarmPara[1].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara2", "0.2", tmp_AlarmPara[2].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara3", "0.15", tmp_AlarmPara[3].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara4", "4", tmp_AlarmPara[4].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);
	GetPrivateProfileString("ConfigInfo","AlarmPara5", "5", tmp_AlarmPara[5].GetBuffer(MAX_PATH),MAX_PATH,sFilePath);

	for (int i = 0; i < 6;i++)
	{
		float test =  atof(tmp_AlarmPara[i]);
		alarmPara[i] = atof(tmp_AlarmPara[i]);
		
	}
	//KillTimer(TIMER2);
	//ShowAlarmLight(DANGEROUS);
	//::Beep(4000, 1000);
	//double a = 100.0 / 3.6;
	//double b = 80.0 / 3.6;
	//Alarm_Algorithm(alarmPara, a, b, -4, true, 10);
}
void CAsafetyAssistantSystemDlg::OnDestroy() 
{
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\" + sFile;//得到xml文件的绝对路劲ipictype1	
	CString cameraen;
	GetPrivateProfileString("ConfigInfo","radio1","0",cameraen.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	camera_EN=atoi(cameraen);
	CString gpsen;
	GetPrivateProfileString("ConfigInfo","radio2","0",gpsen.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GPS_EN=atoi(gpsen);
	CString gprsen;
	GetPrivateProfileString("ConfigInfo","radio3","0",gprsen.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	GPRS_EN=atoi(gprsen);
	CString radaren;
	GetPrivateProfileString("ConfigInfo","radio4","0",radaren.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	Radar_EN=atoi(radaren);
	CString  s;
	GetDlgItemText(IDC_BUTTON5,s);
	if(0 != s.Compare("连接设备")) //相同
	{
		if(camera_EN==1)
		{
			StopPlay();
			OnBTLeave();//退出服务器连接
			SetDlgItemText(IDC_BUTTON5,_T("连接设备"));
		}
	}
	if(GPS_EN==1)
	{
		CloseGPSchuankou();
	}	
	if(GPRS_EN==1)
	{
		CloseGPRSchuankou();
	}		
	if(Radar_EN==1)
	{
		CloseRadarchuankou();
	}
	NET_DVR_Cleanup();
	CDialog::OnDestroy();
	//m_ReceiveData.Empty();  //清空接收数据字符串
}
void CAsafetyAssistantSystemDlg::closechuankou()
{
	
	
	
}
void CAsafetyAssistantSystemDlg::CloseGPSchuankou()
{
	m_Port1.ClosePort();//关闭串口
	jingdu_new=0;
		jingdu_old=0;
		weidu_new=0;
		weidu_old=0;
		firstfindgps=0;
}
void CAsafetyAssistantSystemDlg::CloseGPRSchuankou()
{
	m_Port2.ClosePort();//关闭串口
	gprsopen=0;
}
void CAsafetyAssistantSystemDlg::CloseRadarchuankou()
{
	m_Port3.ClosePort();//关闭串口
	GetDlgItem(IDC_zuoqiansudu)->SetWindowText("未启动");
	GetDlgItem(IDC_youqianchesu)->SetWindowText("未启动");
}
void CAsafetyAssistantSystemDlg::OpenGPSchuankou()
{
	if (m_Port1.InitPort(this, GPS_m_nCom+1, GPS_m_nBaud,GPS_m_cParity,GPS_m_nDatabits,GPS_m_nStopbits,GPS_m_dwCommEvents,512))
	{
		m_Port1.StartMonitoring();
		// gps_jisu=0;
		jingdu_new=0;
		jingdu_old=0;
		weidu_new=0;
		weidu_old=0;
		firstfindgps=1;
		licheng=0;
	}
	else
	{
		AfxMessageBox("没有发现GPS串口或被占用");
	}
}
void CAsafetyAssistantSystemDlg::OpenGPRSchuankou()
{
	if (m_Port2.InitPort(this, GPRS_m_nCom+1, GPRS_m_nBaud,GPRS_m_cParity,GPRS_m_nDatabits,GPRS_m_nStopbits,GPRS_m_dwCommEvents,512))
	{
		m_Port2.StartMonitoring();
		gprsopen=1;
	}
	else
	{
		AfxMessageBox("没有发现GPRS串口或被占用");
	}
}
void CAsafetyAssistantSystemDlg::OpenRadarchuankou()
{
	if (m_Port3.InitPort(this, Radar_m_nCom+1, Radar_m_nBaud,Radar_m_cParity,Radar_m_nDatabits,Radar_m_nStopbits,Radar_m_dwCommEvents,512))
	{
		m_Port3.StartMonitoring();
		GetDlgItem(IDC_zuoqiansudu)->SetWindowText("启动");
		GetDlgItem(IDC_youqianchesu)->SetWindowText("启动");
	}
	else
	{
		AfxMessageBox("没有发现雷达串口或被占用");
	}
}
void CAsafetyAssistantSystemDlg::openchuankou()
{
	// TODO: 在此添加控件通知处理程序代码		

	//m_Port1.ClosePort();//关闭串口
	
}

void CAsafetyAssistantSystemDlg::OnBnClickedButton6()//参数设置
{
	// TODO: 在此添加控件通知处理程序代码
	CSet  cDlg;
	//CPasswordDialog cDlg;//建立对象  
	if(cDlg.DoModal()==IDOK)//用户按下确定  
	{  
        wDVRPort=cDlg.wDVRPort;//=(WORD)nPort;//获得端口
		pchDVRIP=cDlg.pchDVRIP;
		pchUserName=cDlg.pchUserName;
		pchPassword=cDlg.pchPassword;

		GPS_m_nBaud=cDlg.GPS_m_nBaud;       //波特率
		GPS_m_nCom=cDlg.GPS_m_nCom;         //串口号
		GPS_m_cParity=cDlg.GPS_m_cParity;    //校验
		GPS_m_nDatabits=cDlg.GPS_m_nDatabits;    //数据位
		GPS_m_nStopbits=cDlg.GPS_m_nStopbits;    //停止位

		GPRS_m_nBaud=cDlg.GPRS_m_nBaud;       //波特率
		GPRS_m_nCom=cDlg.GPRS_m_nCom;         //串口号
		GPRS_m_cParity=cDlg.GPRS_m_cParity;    //校验
		GPRS_m_nDatabits=cDlg.GPRS_m_nDatabits;    //数据位
		GPRS_m_nStopbits=cDlg.GPRS_m_nStopbits;    //停止位

		Radar_m_nBaud=cDlg.Radar_m_nBaud;       //波特率
		Radar_m_nCom=cDlg.Radar_m_nCom;         //串口号
		Radar_m_cParity=cDlg.Radar_m_cParity;    //校验
		Radar_m_nDatabits=cDlg.Radar_m_nDatabits;    //数据位
		Radar_m_nStopbits=cDlg.Radar_m_nStopbits;    //停止位
		CString sFile = "config.ini";
		char chPath[MAX_PATH];
		GetModuleFileName(NULL,chPath,MAX_PATH);
		CString sDirectoryPath = chPath;
		int nCount = sDirectoryPath.ReverseFind('\\');
		sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
		CString sFilePath = sDirectoryPath + "\\" + sFile;//得到xml文件的绝对路劲ipictype1
		if(GPRS_m_nCom==GPS_m_nCom)
		{
			GPRS_m_nCom=-1;
			MessageBox("GPRS设备串口号不能与GPS设备相同，请重新设置");
			CString gprs_chuankou,radar_chuankou;
			gprs_chuankou.Format("%d",GPRS_m_nCom);
			::WritePrivateProfileString("ConfigInfo","gprs_chuankou",gprs_chuankou,sFilePath);
			return;
		}
		if((GPRS_m_nCom==Radar_m_nCom)||(GPS_m_nCom==Radar_m_nCom))
		{
			Radar_m_nCom=-2;
			GPRS_m_nCom=-1;
			CString gprs_chuankou,radar_chuankou;
			gprs_chuankou.Format("%d",GPRS_m_nCom);
			radar_chuankou.Format("%d",Radar_m_nCom);

			::WritePrivateProfileString("ConfigInfo","radar_chuankou",radar_chuankou,sFilePath);
			::WritePrivateProfileString("ConfigInfo","gprs_chuankou",gprs_chuankou,sFilePath);
			MessageBox("雷达设备串口号不能与GPS等设备相同，请重新设置");
			return;
		}

		// 预警参数获取
		memcpy(alarmPara, cDlg.AlarmP, sizeof(alarmPara));
	} 
}



int rxdatacount=0;  //该变量用于接收字符计数
CString str_old,str_new;
LONG CAsafetyAssistantSystemDlg::OnCommunication(WPARAM ch, LPARAM port)
{
	if (port <= 0 || port > 12)
		return -1;
	if(port==(GPS_m_nCom+1))
	{
		//CString str;
		//str.Format("%c ",ch);
		//int x=atoi(str);
		/*if(ch=='\r')
		{
			//::SendMessage(g_hWnd, WM_USERMESSAGE, (DWORD)cheliangchangdu, NULL);//将检测到的车辆温度个数送到主窗口显示
			if (g_hWnd != INVALID_HANDLE_VALUE)
			{
				::SendMessage(g_hWnd, WM_USERMESSAGE, NULL,NULL);
			}
		}
		str.Format("%c ",ch);
		gps_show += str;
		SetDlgItemText(IDC_STATIC_jingdu,gps_show );*/
		if((USART1_RX_STA&0x8000)==0)//接收未完成 当还没有接收到一帧数据的时候
		 {						
			if(USART1_RX_STA<6)	//如果接收到的数据长度小于6，表示还处在接收数据头的阶段，数据头为$GPGGA  $GPRMC
			{
				if(USART1_RX_STA==0) //判断第一个字节是否为$
				{ //长度为0的时候，表示刚开始接收
					if(ch==0x24) // 如果接收到的第一个字符为$0x24
					{	
						USART1_RX_BUF[USART1_RX_STA]=ch ;//将这个字符放入到接收数组中存放
						USART1_RX_STA++;
					}
					else //如果一开始接收到的字符不是$就表示接收到的数据帧有错误，重新开始计数				
					 USART1_RX_STA=0;
					
				}else //如果已经接受到了第一个数据头			
				if(USART1_RX_STA==1) //如果之前只接收到一个字符的话
				{
					if(ch==0x47)  //如果接收到的第二个字符为G
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ; //将第二个字符G存入到字符数组中
						USART1_RX_STA++;
					}else
	          		USART1_RX_STA=0;					
				}else 
				if(USART1_RX_STA==2)
				{
					if(ch==0x50) //如果接收到的第三个字符是P
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
	        		}else
					USART1_RX_STA=0;
					
				}else 
				if(USART1_RX_STA==3) //接收到的第4个字节可能是G R V
				{
					if(ch=='R')  //如果接收到的第4个字符是G 0x47
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					/*else if(ch==0x52)  //如果接收到的第4个字符是R	 这个是模式2
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}else if(ch==0x56)  //如果接收到的第4个字符是V	  ，这个是模式3
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}*/
					else
					  USART1_RX_STA=0;
					
				}else 
				if(USART1_RX_STA==4)	//接收到的第五个字节可能是G M T S			
				{
					if(ch=='M')	//如果接收到的第5个字符是	G0x47
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					/*else  if(ch==0x4D)	//如果接收到的第5个字符是	M
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}else if(ch==0x54)	//如果接收到的第5个字符是	T
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;						
					}else if(ch==0x53)	//如果接收到的第5个字符是	S
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					*/else
					  USART1_RX_STA=0;
					
				}else 
				if(USART1_RX_STA==5)  //接收到的第6个字节可能是A C G V 
				{
					if(ch=='C') //如果接收到的第6个字符是A0x41
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					/*else if(ch==0x43) //如果接收到的第6个字符是C
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					} else if(ch==0x47) //如果接收到的第6个字符是G
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}else if(ch==0x56) //如果接收到的第6个字符是V
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}*/else
					  USART1_RX_STA=0;					
				} 
			}
			else //如下用来接收数据头外的内容
			 {
					//回车换行的ascll码分别为13,10，十六进制表示就是0x0d，0x0a
					if(USART1_RX_STA&0x4000)//接收到了0x0d 也就是接收到了回车按键
					{
						if(ch!=0x0a)  USART1_RX_STA=0;//接收错误,重新开始
						else 
						{	
						     USART1_RX_STA &= 0x8000;	//接收完成了
							 //::SendMessage(g_hWnd, WM_USERMESSAGE, (DWORD)cheliangchangdu, NULL);//将检测到的车辆温度个数送到主窗口显示
							if (g_hWnd != INVALID_HANDLE_VALUE)
							{
								Real_Top_DATA  d;
								//for(int i=0;i<(USART1_RX_STA&0X3FFF);i++)
									//d.BUF[i]=USART1_RX_BUF[i];
								strcpy(d.BUF, USART1_RX_BUF);
								//d.gaocheng.Format("%s",USART1_RX_BUF);
								//for(int xx=0;xx<11;xx++)
                                  //d.gaochenga[xx]=USART1_RX_BUF[xx];
								::SendMessage(g_hWnd,WM_USERMESSAGE, (WPARAM)&d,NULL);//relatedID.GetBuffer(20)
								//relatedID.ReleaseBuffer();
								//::SendMessage(g_hWnd, WM_USERMESSAGE, NULL,NULL);
							}
						}	
					}
					else
					{
						if(ch==0x0d) //如果当前接收到了回车换行符，就表示接收到了数据尾
						{
							USART1_RX_STA|=0x4000;//就受到0x0d就将标志位置1								
							//如果当前接收到的是回车，但是不是一gprmc结尾的，直接不处理这部分，也不记录
						}
						else
						{
							USART1_RX_BUF[USART1_RX_STA&0X3FFF]=ch;
							USART1_RX_STA++;
							if(USART1_RX_STA>(255-1))
								USART1_RX_STA=0;//接收数据错误,重新开始接收	  
						}		 
					}
				}
		   }  		 
	}
	else if(port==(1+GPRS_m_nCom))
	{
		rxdatacount++;
		if(rxdatacount>1)
		{
			rxdatacount=0;
			str_old.Format("%c ",ch);
			CString str;
			str.Format("%c%c",str_new,str_old);
			gprs_show += str;
		}
		else
		{
			str_new.Format("%c ",ch);
		}
		SetDlgItemText(IDC_EDIT1,gprs_show);
		UpdateData(FALSE);
	}
	else  if(port==(1+Radar_m_nCom))//雷达数据
	{
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{			
			if(USART2_RX_STA<3)	//如果数据长度小于3，表示接收到的是数据头，数据头为0x05，0x10,0x08
			{
				if(USART2_RX_STA==0)  //刚开始接收数据的话
				{
					if(ch==0x05) //接收到的第一个字节为0x05，
					{
						nearestCar_flag = false;	// 放在此处有逻辑风险，最好修改接受逻辑
						USART2_RX_BUF[USART2_RX_STA]=ch ;
						USART2_RX_STA++;
					}else
					  USART2_RX_STA=0;
					
  			    }else if(USART2_RX_STA==1)//接收到第二个字符的话，如果是0x10就表示正确
				{
					if(ch==0x10)
					{
						USART2_RX_BUF[USART2_RX_STA]=ch ;
						USART2_RX_STA++;
					}
					else if(ch==0x11)
					{
						USART2_RX_BUF[USART2_RX_STA]=ch ;
						USART2_RX_STA++;
					}
					else if(ch==0x12)
					{
						USART2_RX_BUF[USART2_RX_STA]=ch ;
						USART2_RX_STA++;
					}else
					  USART2_RX_STA=0;					
				}
				else if(USART2_RX_STA==2)//接收到第3个字符的话，如果是0x08就表示正确
				{
					if(ch==0x08)
					{
						USART2_RX_BUF[USART2_RX_STA]=ch ;
						USART2_RX_STA++;						
					}else
					  USART2_RX_STA=0;					
				}
			}
			else //进入接收数据部分
			{
				if(ch==0xED)//(ch==0xBB) //如果接收到数据尾
				{
					USART2_RX_BUF[USART2_RX_STA]=ch ;
					USART2_RX_STA++;
					if(USART2_RX_BUF[USART2_RX_STA - 2] == 0xEC)
						if(USART2_RX_BUF[USART2_RX_STA - 3] == 0xEB)
							if(USART2_RX_BUF[USART2_RX_STA - 4] == 0xEA)
							{
								Real_T_DATA d;
								for(int xx=0;xx<USART2_RX_STA;xx++)
									d.a[xx]=USART2_RX_BUF[xx];
								d.byte_num = USART2_RX_STA;
								USART2_RX_STA&=0x8000;								
								::SendMessage(g_hWnd,WM_USERMESSAGE1, (WPARAM)&d,NULL);
								
							}
					//USART2_RX_BUF[USART2_RX_STA]=ch ;
					//USART2_RX_STA++;
					//if(	USART2_RX_STA==11)			
					//	USART2_RX_STA|=0x8000;	
					//Real_T_DATA  d;
					//for(int xx=0;xx<11;xx++)
					//	d.a[xx]=USART2_RX_BUF[xx];
					//::SendMessage(g_hWnd,WM_USERMESSAGE1, (WPARAM)&d,NULL);
				}
				else
				{
					USART2_RX_BUF[USART2_RX_STA]=ch ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(253))
						USART2_RX_STA=0;//接收数据错误,重新开始接收	
				}
			}
        }//if((USART2_RX_STA&0x8000)==0)//接收未完成
	}
	//若设置了“自动清空”，则达到50行后，自动清空接收编辑框中显示的数据
	//如果没有“自动清空”，数据行达到400后，也自动清空
	//因为数据过多，影响接收速度，显示是最费CPU时间的操作

	//如果选择了"十六进制显示"，则显示十六进制值
	
	//以下是将接收的字符加在字符串的最后，这里费时很多
	//但考虑到数据需要保存成文件，所以没有用List Control
	//m_ctrlReceiveData.SetSel(0, -1);
	//m_ReceiveData+=str;
	return 0;
}

//将一个字符串作为十六进制串转化为一个字节数组，字节间可用空格分隔，
//返回转换后的字节数组长度，同时字节数组长度自动设置。
int CAsafetyAssistantSystemDlg::Str2Hex(CString str, char* data)
{
	int t,t1;
	int rlen=0,len=str.GetLength();
	//data.SetSize(len/2);
	for(int i=0;i<len;)
	{
		char l,h=str[i];
		if(h==' ')
		{
			i++;
			continue;
		}
		i++;
		if(i>=len)
			break;
		l=str[i];
		t=HexChar(h);
		t1=HexChar(l);
		if((t==16)||(t1==16))
			break;
		else 
			t=t*16+t1;
		i++;
		data[rlen]=(char)t;
		rlen++;
	}
	return rlen;

}
char CAsafetyAssistantSystemDlg::HexChar(char c)
{
	if((c>='0')&&(c<='9'))
		return c-0x30;
	else if((c>='A')&&(c<='F'))
		return c-'A'+10;
	else if((c>='a')&&(c<='f'))
		return c-'a'+10;
	else 
		return 0x10;
}

void CAsafetyAssistantSystemDlg::OnBnClickedOk()//发送
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	CString  s,s1,s2,s3;
	GetDlgItemText(IDC_jingdu,s1);
	GetDlgItemText(IDC_weidu,s2);
	GetDlgItemText(IDC_EDIT2,s3);
	s.Format("%s%s%s",s1,s2,s3);
	if(1==gprsopen)
		m_Port2.WriteToPort(s);  //

}
LRESULT CAsafetyAssistantSystemDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)////当一辆车通过热像仪之后会发送一个一共采集到该车的温度值的个数回来，可以用来判断是否有车挡在热像仪前面
{
	//CString str;
	//int a=(DWORD)wParam;
	//CString str((TCHAR*)wParam); 
	Real_Top_DATA*  d;
	d=(Real_Top_DATA*)wParam;
	Get_GPS(d->BUF );
	USART1_RX_STA=0; 
    return 1;
}
//解析GPS数据 
void CAsafetyAssistantSystemDlg::Get_GPS(char *a)
{
	char timeutc[10];        
	char WD[12]={0};            //纬度
	char WD_a=0;            //纬度方向
	char JD[12]={0};            //经度	  
	char JD_a=0;            //经度方向
	int lock;            //定位状态
	char use_sat[12]={0};        //使用的卫星数
	char high[12];        //高度
	char strspeed[12] = {0}; //速度
	
	for(int x=0;x<12;x++)
	{
		WD[x]=0;
		JD[x]=0;
		high[x]=0;
	}
	//memset(WD, 0, strlen(WD) );
	//memset(JD, 0, strlen(JD) );
	//memset(high,0, strlen(high) );
	///////////////////////
	  int byte_counter=0;//用来对接收到的数据进行计数
	  int byte_num=0;
	  int seg_counter=0;//用来计算段，用逗号隔开的段
	  char tmp;
	  byte_counter=0;
	  seg_counter=0;
	  byte_num=0;
	  do
	  {
		 tmp=a[byte_counter];//将串口缓冲区中的数据放在临时变量中存放

		 if(byte_counter==80) break;//如果长度超过80，直接退出循环
		 else byte_counter++;
		 if(tmp==',') //如果遇到逗号
		 {
			 seg_counter++; byte_num=0;
		 }
		 else//当前字符不是逗号，那就是一段数据
		 {
			 switch(seg_counter)//对数据进行分段
			 {
                case 0:	//$GPRMC
					byte_num=0;							  
					break;
			 	case 1:	//第一个逗号后是实时时间，utc时间
					timeutc[byte_num]=tmp;
					if(byte_num<9)			 //处理
					{
						byte_num++;
					}
					else byte_num=0;							  
					break;
				case 3:                      //经度
		            WD[byte_num]=tmp;		           
				    //if(byte_num<10)
		            {
						byte_num++;
		            }
					//else byte_num=0;
		            break;
		        case 4:                      //纬度方向处理
		            WD_a=tmp;
					//if(byte_num<1)
		            {
						byte_num++;
		            }
					//else 
						//byte_num=0;
		            break;
		        case 5:                                //经度处理
		            JD[byte_num]=tmp;		           
				    //if(byte_num<9)
		            {
						byte_num++;
		            }
					//else byte_num=0;
		            break;
		        case 6: 
					JD_a=tmp;
//					byte_num=0;                               //经度方向处理
//		            if(byte_num<1)
//		            {
//		                JD_a=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;						   
		        case 2:
		            lock=tmp;				                //定位判断,定位质量指示，V=定位无效，A=定位有效
//					byte_num=0;
//		            if(byte_num<1)
//		            {
//		                lock=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;
		        case 8:                                //地面航向
 		           	use_sat[byte_num]=tmp;
					byte_num++;
					//high_len=0;
/*		            if(byte_num==0)
		            {
					use_sat[1]='0';
					use_sat[0]=tmp;
					byte_num++;
					}
					if(byte_num==1)
		            {
					use_sat[1]=use_sat[0];
					use_sat[0]=tmp;
					}	   */
			        break;								
		        case 7:                                //地面速率
					strspeed[byte_num]=tmp;
					byte_num++;
					//high_len++;
/*					high[byte_num]=tmp;
					if(high_end==1)
					{		
						byte_num=0;
						high_end=0;
					}
					else 
					{	
						byte_num++;
						high_len=byte_num;
					}
					if(tmp=='.') high_end=1;	 */
		            break;
				default:
					break;
			}
		}
//		printf("run gpgga get /n");	
	  }while(USART1_RX_BUF[byte_counter]!='*');	
	 //gps_jisu++;
	 int JD_ddd = 0;
	 float JD_mm = 0;
	 int WD_dd = 0;
	 float WD_mm = 0;
	 //double jingdudata = 0;
	 //double weidudata = 0;
	 float speed;
	 CString  jingdudata_temp,weidudata_temp,temp1,temp2,temp3;
	// // SetDlgItemText(IDC_BUTTON5,_T("断开连接"));
	 JD_ddd = (JD[0] - '0')*100 + (JD[1] - '0')*10 + (JD[2] - '0');	//dd
	 JD_mm = atof((JD+3));	//mm.mmmm
	 CString s, tmp_dd, tmp_mm;
	 tmp_dd.Format("%d", JD_ddd); //度
	 tmp_mm.Format("%.2f", JD_mm); //分
	 s = tmp_dd + "°" + tmp_mm + "′";
	 SetDlgItemText(IDC_jingdu,s);

	 s.Format("%c",JD_a);
	 SetDlgItemText(IDC_jingdufangwei,s);

	 WD_dd = (WD[0] - '0')*10 + (WD[1] - '0');	//dd
	 WD_mm = atof((WD+2));	//mm.mmmm
	 //CString s, tmp_dd, tmp_mm;
	 tmp_dd.Format("%d", WD_dd); //度
	 tmp_mm.Format("%.2f", WD_mm); //分
	 s = tmp_dd + "°" + tmp_mm + "′";
	 SetDlgItemText(IDC_weidu,s);
	 s.Format("%c",WD_a);
	 SetDlgItemText(IDC_weidufangwei,s);
	 char test[10] = "20";
	 speed = atof(strspeed);	// 单位：海里
	 speed *= 1.852;		// 转换为Km/h		 
		 
	s.Format("%.2f",speed);
	GetDlgItem(IDC_sudu)->SetWindowText(s+"Km/h");
	//myCar[1].time = GetTickCount(); // 获取系统时间
	myCar[1].vel = speed;
	// 处理加速度信息
	myCar[1].accel = (myCar[1].vel - myCar[0].vel) / ((double)( 200.0 )/1000*3.6 );  // 采样时间间隔
	s.Format("%.2f", myCar[1].accel);
	SetDlgItemText(IDC_EDIT_ACCE, s + "m/s^2");
	memcpy(&(myCar[0]) , &(myCar[1]), sizeof(MyCar_Motion));


	// 按照GPS的处理周期来处理结果
    int tmp_alarm = Alarm_Algorithm(alarmPara, speed / 3.6, nearestCar_Vel, nearestCar_Accle, nearestCar_flag, nearestCar_Distance);
	ShowAlarmLight(tmp_alarm);	// 每次执行一个
}
void CAsafetyAssistantSystemDlg::Get_GPS( )
{
	char timeutc[10];        
	char WD[12]={""};            //纬度
	char WD_a=0;            //纬度方向
	char JD[12]={""};            //经度	  
	char JD_a=0;            //经度方向
	int lock;            //定位状态
	char use_sat[2]={""};        //使用的卫星数
	char high[12];        //高度
	for(int x=0;x<12;x++)
	{
		WD[x]=0;
		JD[x]=0;
		high[x]=0;
	}
	//memset(WD, 0, strlen(WD) );
	//memset(JD, 0, strlen(JD) );
	//memset(high,0, strlen(high) );
	///////////////////////
	  int byte_counter=0;//用来对接收到的数据进行计数
	  int byte_num=0;
	  int seg_counter=0;//用来计算段，用逗号隔开的段
	  char tmp;
	  byte_counter=0;
	  seg_counter=0;
	  byte_num=0;
	  do
	  {
		 tmp=USART1_RX_BUF[byte_counter];//将串口缓冲区中的数据放在临时变量中存放

		 if(byte_counter==80) break;//如果长度超过80，直接退出循环
		 else byte_counter++;
		 if(tmp==',') //如果遇到逗号
		 {
			 seg_counter++; byte_num=0;
		 }
		 else//当前字符不是逗号，那就是一段数据
		 {
			 switch(seg_counter)//对数据进行分段
			 {
                case 0:	//第一个逗号后是实时时间，utc时间
					byte_num=0;							  
					break;
			 	case 1:	//第一个逗号后是实时时间，utc时间
					timeutc[byte_num]=tmp;
					if(byte_num<9)			 //处理
					{
						byte_num++;
					}
					else byte_num=0;							  
					break;
				case 2:                               //纬度处理
		            WD[byte_num]=tmp;		           
				    if(byte_num<8)
		            {
						byte_num++;
		            }
					else byte_num=0;
		            break;
		        case 3:                                //纬度方向处理
		            WD_a=tmp;
					if(byte_num<1)
		            {
						byte_num++;
		            }
					else 
						byte_num=0;
		            break;
		        case 4:                                //经度处理
		            JD[byte_num]=tmp;		           
				    if(byte_num<9)
		            {
						byte_num++;
		            }
					else byte_num=0;
		            break;
		        case 5: 
					JD_a=tmp;
//					byte_num=0;                               //经度方向处理
//		            if(byte_num<1)
//		            {
//		                JD_a=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;						   
		        case 6:
		            lock=tmp;				                //定位判断,定位质量指示，0=定位无效，1=定位有效
//					byte_num=0;
//		            if(byte_num<1)
//		            {
//		                lock=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;
		        case 7:                                //定位使用的卫星数
 		           	use_sat[byte_num]=tmp;
					byte_num++;
					//high_len=0;
/*		            if(byte_num==0)
		            {
					use_sat[1]='0';
					use_sat[0]=tmp;
					byte_num++;
					}
					if(byte_num==1)
		            {
					use_sat[1]=use_sat[0];
					use_sat[0]=tmp;
					}	   */
			        break;								
		        case 9:                                //高度处理
					high[byte_num]=tmp;
					byte_num++;
					//high_len++;
/*					high[byte_num]=tmp;
					if(high_end==1)
					{		
						byte_num=0;
						high_end=0;
					}
					else 
					{	
						byte_num++;
						high_len=byte_num;
					}
					if(tmp=='.') high_end=1;	 */
		            break;
				default:
					break;
			}
		}
//		printf("run gpgga get /n");	
	  }while(USART1_RX_BUF[byte_counter]!='*');	
	 // SetDlgItemText(IDC_BUTTON5,_T("断开连接"));
	  CString s;
	  s.Format("%s",JD);
	  SetDlgItemText(IDC_jingdu,s);
	  //east 东(69) west 西 (87)
	  //north 北(78)south 南 (83)
	  //if(JD_a==69)
	  s.Format("%c",JD_a);
	  SetDlgItemText(IDC_jingdufangwei,s);
	  s.Format("%s",WD);
	  SetDlgItemText(IDC_weidu,s);
	  s.Format("%c",WD_a);
	  SetDlgItemText(IDC_weidufangwei,s);
	 // s.Format("%s",high);
	 // SetDlgItemText(IDC_gaocheng,s);

  
}
LRESULT CAsafetyAssistantSystemDlg::OnMyMessage1(WPARAM wParam, LPARAM lParam)////当一辆车通过热像仪之后会发送一个一共采集到该车的温度值的个数回来，可以用来判断是否有车挡在热像仪前面
{
	Real_T_DATA* d;
	d=(Real_T_DATA*)wParam;

	int tmp_count = 0;
	tmp_count = d->byte_num / 11; // 计算共几组车辆信息数据
	int tmp_exam_count = 0;

	int count_nearest = 0;
	double nearest_distance = 10000;
	double nearest_vel = 0;
	double nearest_accel = 0;
	CString m_str;

	int *raw_speed_x = new int [tmp_count];
	int *raw_pos_x = new int[tmp_count];
	int *raw_pos_y = new int[tmp_count];
	double *speed_x = new double[tmp_count];
	double *pos_x = new double[tmp_count]; 
	double *pos_y = new double[tmp_count];	
	int *raw_ID = new int[tmp_count];
	//double *accel = new double[tmp_count];
	bool tmpflag = false;
	//double tmp_time = forwardCar[1].time;
	memset(&(forwardCar[1]), 0, sizeof(FCar_Motion));  //将当前车辆信息归零
	//forwardCar[1].time = tmp_time;// 获取当前时间	
	for (int i = 0; i < tmp_count; i++)
	{
		if (d->a[11*i] == 0x05)
		{
			if (d->a[11*i+1] == 0x10 + i)
			{
				if (d->a[11*i+2] == 0x08)
				{
					//[数据格式]：
					//--05 10 08 00 4D 01 1E 7B FB 23 5E  (目标信息)
					//--0000 0000 0100 1101 0000 0001 0001 1110 0111 1011 1111 1011 0010 0011 0101 1110
					//--转换为二进制后截取相应的位数如下:
					//--000(3位) 0000001(7位) 00110(5位) 10000000100(11位) 01111001111(11位) 0111111101100(13位) 1000110101111(13位) 0(1位)
					//--下面的括号内为二进制 转换为 十进制值 
					//--000(0) 0000001(1) 00110(6)
					//--10000000100(1028) 计算车速(Y方向的车速不用管)
					//--01111001111(975)  计算车速 X_Speed=(975-1024)*0.1 = -4.9(m/s) = -17(km/h)
					//--0111111101100(4076)  计算车道位置 Y_Pos=(4076-4096)*0.064 = -1.28 
					//--1000110101111(4527)  计算距离位置 X_Pos=(4527-4096)*0.064 = 27.58		   

					//第五字节第三位开始到第六字节第5位结束为速度，共11位
					raw_ID[i] = ((d->a[11*i+3]&0x1F)<<2) + ((d->a[11*i+4]&0xC0)>>6);	// 第四字节的低5位 和 第五字节的高2位 拼接
					raw_speed_x[i] = ((d->a[11*i+6]&0x3F)<<5) + ((d->a[11*i+7]&0xF8)>>3);
					raw_pos_y[i] = ((d->a[11*i+7]&0x07)<<10) + ((d->a[11*i+8])<<2) + ((d->a[11*i+9])>>6);
					raw_pos_x[i] = ((d->a[11*i+9]&0x3F)<<7) + ((d->a[11*i+10])>>1);
					//double test = 0; // for test
					speed_x[i] = (raw_speed_x[i] - 1024) * 0.1 * 3.6;
					//test = speed_x[i];
					pos_x[i] = (raw_pos_x[i] - 4096) * 0.064;
					//test = pos_x[i];
					pos_y[i] = (raw_pos_y[i] - 4096) * 0.064;	// y方向取负号和绘图中的符号保持一致。
					//test = pos_y[i];
					tmp_exam_count++;
					// 记录当前每辆车的位置、速度信息，并计算处加速度信息
					forwardCar[1].pos_x[i] = pos_x[i];
					forwardCar[1].vel[i] = speed_x[i];
					//double test = (forwardCar[1].time - forwardCar[0].time) / 1000 * 3.6;
					//forwardCar[1].accel[i] = (forwardCar[1].vel[i] - forwardCar[0].vel[i]) / ((forwardCar[1].time - forwardCar[0].time) / 1000 * 3.6);	// 计算出每个车的加速度
					forwardCar[1].accel[i] = (forwardCar[1].vel[i] - forwardCar[0].vel[i]) / ( (20.0) / 1000 * 3.6);	// 计算出每个车的加速度
					forwardCar[1].ID[i] = raw_ID[i];
				}
			}
		}
	} // 处理这所有车辆信息数据	
	int tmp_nearest_num = 0;
	for (int i = 0; i < tmp_count; i++)
	{
		if(abs(pos_y[i]) < 5 )
		{
			if(nearest_distance > pos_x[i])
			{
				count_nearest++; 
				// tmp_nearest_num = i;
				if ( forwardCar[1].vel[i] != 0 && forwardCar[0].vel[i] != 0)
				{
					nearest_distance = forwardCar[1].pos_x[i];
					nearest_vel = forwardCar[1].vel[i];
					nearest_accel = forwardCar[1].accel[i];
					tmpflag = true;
				} // 如果获取的前一个速度和当前速度数据均不为零
			}
		}
	} // 获取y方向左右4m内的车辆数
	
	if ( count_nearest == 0 )
	{
		nearest_distance = 0;
		nearest_vel = 0;
		nearest_accel = 0;
		tmpflag = false;
	} // 如果没有y方向左右4m内的车辆信息，显示数据清零

	memcpy(&forwardCar[0] , &forwardCar[1], sizeof(FCar_Motion));	// 将当前数据赋值到上一次数据

	// 显示y方向在4m范围内，x方向距离最近的一辆车辆，显示出来
	m_str.Format("%.2f", nearest_distance);
	SetDlgItemText(IDC_youqianchesu,m_str + "m");
	m_str.Format("%.2f", nearest_vel);
	SetDlgItemText(IDC_zuoqiansudu,m_str + "km/h");
	m_str.Format("%.2f", nearest_accel);
	SetDlgItemText(IDC_EDIT_FACCE,m_str + "m/s^2");

	//////////////////////////////////////////////////////////////////////////
	// 将前车的参数赋值给全局变量
	nearestCar_Accle = nearest_accel;
	nearestCar_Distance = nearest_distance;
	nearestCar_Vel = nearest_vel /3.6;
	nearestCar_flag = tmpflag;
	//////////////////////////////////////////////////////////////////////////

	if (tmp_exam_count == tmp_count) 
	{
		m_carInfo.car_num = tmp_count;
		memcpy(m_carInfo.PosCarX, pos_x, tmp_count*sizeof(double));	// 雷达坐标和图像坐标不一致, X = Y
		memcpy(m_carInfo.PosCarY, pos_y, tmp_count*sizeof(double));
		memcpy(m_carInfo.VelocityX, speed_x, tmp_count*sizeof(double));
		memcpy(m_carInfo.ID, raw_ID, tmp_count*sizeof(int));

		InvalidateRect(m_rc);	// 刷新界面信息
	}// 检查采集到的数据和计算的数据是否一致

	delete [] raw_speed_x;
	delete [] raw_pos_x;
	delete [] raw_pos_y;
	delete [] speed_x;
	delete [] pos_x;
	delete [] pos_y;

	USART2_RX_STA=0;//接收数据错误,重新开始接收
	return 0;
}
void CAsafetyAssistantSystemDlg::OnBnClickedButton1()//截屏
{
	// TODO: 在此添加控件通知处理程序代码
	zhuatu();//抓取指定通道的图u 
}

void CAsafetyAssistantSystemDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString  s;
	GetDlgItemText(IDC_BUTTON3,s);
	if(0 == s.Compare("开始录制视频")) //相同
	{
		char videoName[256] = {0};
		SYSTEMTIME stUTC;  
		::GetLocalTime(&stUTC);  
		//显示时间的间隔。   
		sprintf_s(videoName,"%s\\%04d%02d%02d%02d%02d%02d%02d.mp4",videodizhi, stUTC.wYear,stUTC.wMonth,stUTC.wDay, \
				stUTC.wHour, stUTC.wMinute,stUTC.wSecond,stUTC.wMilliseconds);
		if (NET_DVR_SaveRealData(m_struDeviceInfo.lLoginID,videoName))
		{
			SetDlgItemText(IDC_BUTTON3,_T("停止录制视频"));
		}
	}
	else
	{
		
		if (NET_DVR_StopSaveRealData(m_struDeviceInfo.lLoginID))
		{
			SetDlgItemText(IDC_BUTTON3,_T("开始录制视频"));
		}
		
	}
}
void CAsafetyAssistantSystemDlg::OnBnClickedButton4()//路径设置
{
	// TODO: 在此添加控件通知处理程序代码
	CSETLUJIN td;  
	if(IDOK==td.DoModal())
	{
		picdizhi=td.GETSaveList();;//用来存放当前图像识别后上传过来的路径
		videodizhi=td.GETSaveList1();;//用来存放当前图像识别后上传过来的路径
		CString sFile = "config.ini";
		char chPath[MAX_PATH];
		GetModuleFileName(NULL,chPath,MAX_PATH);
		CString sDirectoryPath = chPath;
		int nCount = sDirectoryPath.ReverseFind('\\');
		sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
		CString sFilePath = sDirectoryPath + "\\" + sFile;
		::WritePrivateProfileString("ConfigInfo","tplujin",picdizhi,sFilePath);
		::WritePrivateProfileString("ConfigInfo","videolujin",videodizhi,sFilePath);
	}
}
int CAsafetyAssistantSystemDlg::zhuatu()//抓取指定通道的图
{
	if(m_lPlayHandle == -1)
	{
		return 0;
	}
	char PicName[256] = {0};
	SYSTEMTIME stUTC;  
    ::GetLocalTime(&stUTC);  
    //显示时间的间隔。   
	sprintf_s(PicName,"%s\\%04d%02d%02d%02d%02d%02d%02d.jpg",picdizhi, stUTC.wYear,stUTC.wMonth,stUTC.wDay, \
				stUTC.wHour, stUTC.wMinute,stUTC.wSecond,stUTC.wMilliseconds);
	NET_DVR_JPEGPARA JpgPara = {0};
	////////////////////////////////////////
	if(NET_DVR_CaptureJPEGPicture(m_struDeviceInfo.lLoginID, 1, &JpgPara, PicName))
	{
	}
	else
		return 0;
	return 1;
}
void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo1(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	GPS_m_nCom=i;
}

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo11(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	GPRS_m_nCom=i;
}

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo7(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	Radar_m_nCom=i;
}

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo4(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i1=i;
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


void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo12(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i2=i;
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


void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo8(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i3=i;
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


void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo17(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	//char temp;
	int i4=i;
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

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo15(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	//char temp;
	int i5=i;
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

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo10(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	//char temp;
	int i6=i;
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

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo5(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i7=i;
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


void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo13(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i8=i;
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

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo9(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i9=i;
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

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo6(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i10=i;
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

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo14(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i11=i;
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

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo16(int i)
{
	// TODO: 在此添加控件通知处理程序代码
	int i12=i;
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


CString CAsafetyAssistantSystemDlg::OnBUTTONSure(double in_JD1,double in_WD1,double in_JD2,double in_WD2) //这个函数可以得到距离
{
	double x,y,out; 
	double PI=3.14159265; 
	double R=6.371229*1e6; 	
	x=(in_JD2-in_JD1)*PI*R*cos( ((in_WD1+in_WD2)/2) *PI/180)/180; 
	y=(in_WD2-in_WD1)*PI*R/180; 
	out=hypot(x,y); 
	out/=1000;
	CString m_Dis="";
	licheng+=out;
	//m_Dis.Format("%d",licheng);
	//GetDlgItem(IDC_licheng)->SetWindowText(m_Dis+"Km");
	double pp=(out*100/5)/3.6;
	m_Dis.Format("%f",pp);
	GetDlgItem(IDC_sudu)->SetWindowText(m_Dis+"Km/h");
	return  m_Dis;	
}

//void CAsafetyAssistantSystemDlg::OnBnClickedButton7()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString i;
//	i=OnBUTTONSure(45,45,50,50);
//	Get_GPS( );
//
//} 


void CAsafetyAssistantSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
   {
       case TIMER1:
      { 
            CString i,j;
			i=OnBUTTONSure(45,45,50,50);
			j.Format("%s km",i);
			SetDlgItemText(IDC_sudu,j);
            break;
      }
      case TIMER2:
      {
		  static bool bFlash = false;
		  time_count[alarmLast]++;  // 时间计数器
		  if(!bFlash)
		  {
			  switch(alarmLast)
			  {					
			  case SAFE:
				  //InvalidateRect(alarmLamp_rc);
				  DrawAlarmLight(RGB(0, 255, 0));     // 绿色
				  break;
			  case THREAT:
				  if (time_count[THREAT] >= 5)
				  {
					  InvalidateRect(alarmLamp_rc);
					  KillTimer(TIMER2);
					  time_count[THREAT] = 0;
					  isAlarmfalg[THREAT] = false;
					  bFlash = false;
					  return ;
				  }
				  DrawAlarmLight(RGB(255, 255, 0));	// 黄色RGB(255,255,0)
				  break;
			  case DANGEROUS:
				  if (time_count[DANGEROUS] >= 10)
				  {
					  InvalidateRect(alarmLamp_rc);
					  KillTimer(TIMER2);
					  time_count[DANGEROUS] = 0;
					  isAlarmfalg[DANGEROUS] = false;
					  bFlash = false;
					  return ;
				  }
				  DrawAlarmLight(RGB(255, 0, 0));		// 红色
				  break;
			  case GODBLESSYOU:
				  if (time_count[GODBLESSYOU] >= 40)
				  {
					  InvalidateRect(alarmLamp_rc);
					  KillTimer(TIMER2);
					  time_count[GODBLESSYOU] = 0;
					  isAlarmfalg[GODBLESSYOU] = false;
					  bFlash = false;
					  return ;
				  }
				  DrawAlarmLight(RGB(255, 0, 0));		// 红色
				  break;
			  default:
				  break;	// error
			  }				
			  bFlash = true;				
		  }
		  else
		  {
			  if (alarmLast != SAFE)
			  {
				  InvalidateRect(alarmLamp_rc);					
			  }		
			  bFlash = false;
		  }
		  break;
      }
      default:
            break;
   }
	CDialogEx::OnTimer(nIDEvent);
}


void CAsafetyAssistantSystemDlg::OnStnClickedjingdu()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CAsafetyAssistantSystemDlg::OnStnClickedgaocheng()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CAsafetyAssistantSystemDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT1,"");
}


void CAsafetyAssistantSystemDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tmp;
	m_box.GetLBText(m_box.GetCurSel(), tmp);
	SetDlgItemText(IDC_EDIT2,  tmp);
}

void CAsafetyAssistantSystemDlg::DrawBackground() 
{
	// 背景图显示方法 下面代码
	//CDC MemDC;
	//BITMAP bmp;
	//CBitmap *pOldBitmap;

	//bitmap.GetObject(sizeof(BITMAP), &bmp);
	//MemDC.CreateCompatibleDC(pDC);
	//pOldBitmap = (CBitmap *)(MemDC.SelectObject(&bitmap));

	//pDC->SetStretchBltMode(COLORONCOLOR);
	//pDC->StretchBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	//MemDC.SelectObject(pOldBitmap);
	//MemDC.DeleteDC(); 

	// 原方案
	CPen cu[2];
	CString m_str;

	cu[0].CreatePen(PS_DOT, 1, RGB(0,0,0));//绘制背景
	CPen* oldPen = pDC->SelectObject(&cu[0]);

	const int road_num = 5; // 在界面中绘制5条车道
	//绘制车道线
	for (int i=1; i < road_num; i++)
	{
		// if(i == 1 || i == 5)
		{
			pDC->MoveTo(i * rc.Width()/road_num, 0);
			pDC->LineTo( i * rc.Width()/road_num, rc.Height());
		}
	}
	for (int i=1; i < 10; i++)
	{
		pDC->MoveTo(0, i * rc.Height()/10);
		pDC->LineTo(rc.Width(), i * rc.Height()/10);  // 绘制100mm长，每隔10m画一条

		m_str.Format("%dm", (10-i)*10);
		CFont m_font;
		m_font.CreatePointFont(rc.Height()/4, "宋体");
		CFont* m_pOldFont = (CFont*)pDC->SelectObject(&m_font);
		pDC->TextOutA(rc.Width()/1.3 - 2 , i * rc.Height()/10 - 7, m_str);
		pDC->SelectObject(m_pOldFont);
	}
	pDC->SelectObject(oldPen);
	//CString m_str;
	//for (int i=1; i < 8; i++)
	//{
	//	m_str.Format("%dm", (8-i)*10);
	//	CFont m_font;
	//	m_font.CreatePointFont(rc.Height()/4, "宋体");
	//	CFont* m_pOldFont = (CFont*)pDC->SelectObject(&m_font);
	//	pDC->TextOutA(rc.Width()/1.3 - 2 , i * rc.Height()/8.2 - 7, m_str);
	//	pDC->SelectObject(m_pOldFont);
	//}	
}

void CAsafetyAssistantSystemDlg::DrawACar(double Pos_x, double Pos_y, double velocity, int ID)
{
	int init_x, init_y;
	float k = 0.3;
	CString m_str;
	CPen m_pen[2];
	CBrush cu;

	double tmp = 0;
	tmp = Pos_y;
	Pos_y = Pos_x;
	Pos_x = -tmp;	// 雷达坐标系转换为图像坐标系
	const int length = 100;

	cu.CreateSolidBrush(RGB(255,0,0));//绘制汽车
	CBrush* oldBrush = pDC->SelectObject(&cu);

	m_pen[0].CreatePen(PS_SOLID, 3, RGB(255,0,0));
	CPen* oldPen = pDC->SelectObject(&m_pen[0]);	

	init_x = rc.Width()/2 - rc.Height()/length + rc.Height()*Pos_x/length;	// 	计算有侧向偏差时车身左侧的位置。 rc.Height()/100为单位长度对应的像素点距离，半个车身的宽度为1m
	init_y = rc.Height() - rc.Height()*Pos_y/length;	// 起始点
	// 绘制汽车图块
	pDC->Rectangle(init_x, init_y, init_x + rc.Height()*2/length, init_y - rc.Height()*4/length); // 车身长4m，宽2m
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	m_pen[1].CreatePen(PS_SOLID, 3, RGB(0,0,255));
	oldPen = pDC->SelectObject(&m_pen[1]);
	// 画出速度线
	pDC->MoveTo(init_x + rc.Height()/length, init_y - rc.Height()*4/length);
	pDC->LineTo(init_x + rc.Height()/length, init_y - rc.Height()*4/length - k*velocity);
	pDC->SelectObject(oldPen);

	m_str.Format("%.1fkm/h", velocity);
	CFont m_font;
	m_font.CreatePointFont(rc.Height()/6, "黑体");
	CFont* m_pOldFont = (CFont*)pDC->SelectObject(&m_font);

	//pDC->TextOutA(init_x , init_y - rc.Height()*4/100 - 18, m_str);
	m_str.Format("%d", ID);
	//pDC->TextOutA(init_x , init_y - rc.Height()*3/100 + 10, "ID:" + m_str);

	pDC->SelectObject(m_pOldFont);
}

void CAsafetyAssistantSystemDlg::DrawAlarmLight(COLORREF cColor)
{
	CStatic *pStatic=(CStatic *)GetDlgItem(IDC_STATIC_ALARMLIGHT);//控件ID 
	
	CRect tmp_rect;
	GetDlgItem(IDC_STATIC_ALARMLIGHT)->GetClientRect(&tmp_rect);
	const int endge_size = 10;

	if (pmDC != NULL)
	{
		CBrush m_cu;
		CPen m_pen;
		m_cu.CreateSolidBrush(cColor);//绘制警示标志
		CBrush* oldBrush = pmDC->SelectObject(&m_cu);
		m_pen.CreatePen(PS_SOLID, 3, cColor);
		CPen* oldPen = pmDC->SelectObject(&m_pen);
		pmDC->Ellipse(tmp_rect.Width()/2-tmp_rect.Height()/2 + endge_size, endge_size, tmp_rect.Width()/2+tmp_rect.Height()/2 - endge_size, tmp_rect.Height() - endge_size);
		pmDC->SelectObject(oldBrush);
		pmDC->SelectObject(oldPen);
	}	
}

/*
*  预警算法函数:Alarm_Algorithm();
*  参数：alarm_para[6]：固定的参数。v_a：本车速度。v_b:前车速度。a_b:前车加速度。flag:是否有前车。Sab:前车距离。
*  返回值: 0：SAFE, 1:THREAT, 2:DANGEROUS, 3:GODBLESSYOU, -1, 没有前车。
*  
*/

int CAsafetyAssistantSystemDlg::Alarm_Algorithm(double alarm_para[6], double v_a, double v_b, double a_b, bool flag, double Sab)
{
	double Sp = -1;
	double Sr = -1;
	double Se1 = -1;
	double Se2 = -1;
	int rtn = 1;
	// 预警公式
	if(abs(a_b) > 0.01)
	{
		double factor_a = (alarm_para[1] + alarm_para[2] + alarm_para[3]/2)*(alarm_para[1] + alarm_para[2] + alarm_para[3]/2)/2;
		double factor_b = (alarm_para[1] + alarm_para[2] + alarm_para[3]/2);

		Sp = alarm_para[0] * (v_a * (alarm_para[1] + alarm_para[2] + alarm_para[3]/2) + v_a*v_a/(2*alarm_para[5])) + alarm_para[4];

		Sr = Sp - v_b*v_b/(2*alarm_para[5]);

		Se1 = Sr + v_b*v_b/(2*a_b);

		Se2 = Sp - (v_b - v_a*a_b/alarm_para[5]) * factor_b	+ a_b*factor_a - v_a * v_b / alarm_para[5] 
		         + a_b*v_a*v_a/(2* alarm_para[5] * alarm_para[5]);

		// 预警流程
		if (flag) 
		{
			if (a_b < 0)
			{
				if (Sab > Sp)
				{

					rtn = SAFE;
				}
				else if (Sab > Sr && Sab <= Sp)
				{
					rtn = THREAT;
				}
				else if (Sab > Se1 && Sab <= Sr)
				{
					rtn = DANGEROUS;
				}
				else
				{
					rtn = GODBLESSYOU;
				}
			}
			else
			{
				if (Sab > Sp)
				{
					rtn = SAFE;
				}
				else if (Sab > Sr && Sab <= Sp)
				{
					rtn = THREAT;
				}
				else if (Sab > Se2 && Sab <= Sr)
				{
					rtn = DANGEROUS;
				}
				else
				{
					rtn = GODBLESSYOU;
				}
			}
		} // 如果发现目标
		else
			rtn = -1;
	}
	else
	{
		rtn = -1;
	}

	
	//CArchive g_ar(&g_File,CArchive::store);  // for test

	CString str;
	str.Format("%lf %lf %lf %d %lf %d", v_a ,v_b, a_b, flag, Sab, rtn);
	str = str + "\r\n";
	g_File.Write(str, str.GetLength());


	return rtn;
}

/*
*  显示预警结果:ShowAlarmLight();
*  参数：预警结果。
*  函数功能：根据预警结果在界面中显示预警结果。
*  
*/

void CAsafetyAssistantSystemDlg::ShowAlarmLight(int alarmClass)
{
	// 根据每次预警的结果来选择方案
	if (alarmClass < alarmLast && isAlarmfalg[alarmLast] && alarmClass == -1)
	{
		// do nothing
	}// 如果新的警报等级小于正在上一次的警报等级,并且上一次的警报仍在执行
	else if(alarmLast == alarmClass)
	{
		time_count[alarmClass] = 0;
		if(!isAlarmfalg[alarmClass])
		{
			KillTimer(TIMER2);
			switch(alarmClass)
			{
			case SAFE:					
				SetTimer(TIMER2, 1000, NULL);
				break;
			case THREAT: 
				SetTimer(TIMER2, 500, NULL); // 500ms，执行2S，计数器累计4次
				break;
			case DANGEROUS:
				SetTimer(TIMER2, 200, NULL); // 200ms间隔，执行5S，计数器累计25次
				break;
			case GODBLESSYOU:
				SetTimer(TIMER2, 50, NULL); // 200ms间隔，执行5S，计数器累计100次
				break;
			default:
				break;
			}
		}
	}
	else
	{
		alarmLast = alarmClass;  // 保留当前的警报等级

		memset(isAlarmfalg, 0, sizeof(isAlarmfalg));
		memset(time_count, 0, sizeof(time_count));	// 所有计数器清零
		isAlarmfalg[alarmClass] = true;

		KillTimer(TIMER2);
		switch(alarmClass)
		{
		case SAFE:					
			SetTimer(TIMER2, 10, NULL);
			break;
		case THREAT: 
			SetTimer(TIMER2, 500, NULL); // 500ms，执行2S，计数器累计4次
			break;
		case DANGEROUS:
			SetTimer(TIMER2, 200, NULL); // 200ms间隔，执行5S，计数器累计25次
			break;
		case GODBLESSYOU:
			SetTimer(TIMER2, 50, NULL); // 200ms间隔，执行5S，计数器累计100次
			break;
		default:
			break;
		}
	} 
}