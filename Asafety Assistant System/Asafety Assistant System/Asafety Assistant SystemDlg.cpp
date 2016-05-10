
// Asafety Assistant SystemDlg.cpp : ʵ���ļ�
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
HWND g_hWnd = NULL;
char USART1_RX_BUF[256];     //���ջ���,���USART_REC_LEN���ֽ�.
int USART2_RX_BUF[256];     //���ջ���,���USART_REC_LEN���ֽ�.

int USART1_RX_STA=0;         		//����1����״̬���	
int USART2_RX_STA=0;         		//����2����״̬���
#define POSYFILTER  2  //��ʾy����ƫ��������POSYFITER��Χ�ڵ�����



//CString filePathName = "carInfo.log";
//
//CFile g_File(filePathName, CFile::modeWrite | CFile::modeCreate); // for test
class Real_Top_DATA//������������� �ڵ�ʹ�ã���ŵ�ǰ����¶�ֵ��ʱ��
{
	public:
		Real_Top_DATA()
		{
			//jingdu="";//����
		    //weidu="";//���ƺ�
		    //gaocheng="";//�߳�
		}
	public:
		//CString jingdu;//����
		//CString weidu;//γ��
		//CString gaocheng;//�߳�
		char BUF[256];
};
typedef CList<Real_Top_DATA, Real_Top_DATA&>  Real_Top_Lst;
class Real_T_DATA//������������� �ڵ�ʹ�ã���ŵ�ǰ����¶�ֵ��ʱ��
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

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAsafetyAssistantSystemDlg �Ի���




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
	ON_MESSAGE( WM_USERMESSAGE, OnMyMessage)//���
	ON_MESSAGE( WM_USERMESSAGE1, OnMyMessage1)//���
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


// CAsafetyAssistantSystemDlg ��Ϣ�������
void CAsafetyAssistantSystemDlg::FullScreenModeOn() 
{ 
    GetWindowPlacement( &m_OldWndPlacement); 
    CRect WindowRect; 
    GetWindowRect(&WindowRect); 
    CRect ClientRect; 
    RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect); 
    ClientToScreen(&ClientRect);

    // ��ȡ��Ļ�ķֱ���

    int nFullWidth=GetSystemMetrics(SM_CXSCREEN); 
    int nFullHeight=GetSystemMetrics(SM_CYSCREEN); 
    
    //������������Ŀͻ���ȫ����ʾ����(0,0)��(nFullWidth, nFullHeight)����, 
    //��(0,0)��(nFullWidth, nFullHeight)������������ԭ���ںͳ�������֮��� �ͻ���λ�ü�Ĳ�ֵ, �͵õ�ȫ����ʾ�Ĵ���λ�� 
    m_FullScreenRect.left = WindowRect.left - ClientRect.left; 
    m_FullScreenRect.top = WindowRect.top - ClientRect.top; 
    m_FullScreenRect.right = WindowRect.right - ClientRect.right + nFullWidth; 
    m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + nFullHeight; 
    m_bFullScreenMode = TRUE; 
    
    //����ȫ����ʾ��־Ϊ TRUE 
    //����ȫ����ʾ״̬ 
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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	//FullScreenModeOn() ;//ȫ����ʾ����
	//ShowWindow(SW_SHOWMAXIMIZED);
	GPS_m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	GPRS_m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	Radar_m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetWindowText(_T("�г�������ȫԤ�����"));
		//���¼����Ǵ���һ����ɫ��Ļ
	m_ptzScreen.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,1981);
	init_all_data();//��ʼ��������Ҫ��ʼ���ı���
	UpdataScreenPos();
	m_ptzScreen.ShowWindow(SW_SHOW);
	m_ptzScreen.SetCallBack(MessageProcFunc,(DWORD)this,GetParamsFunc,(DWORD)this,SetParamsFunc,(DWORD)this,RectEventFunc,(DWORD)this);
	m_ptzScreen.SetShowPlayWin(SPLIT1,0);
	if(FALSE==NET_DVR_Init())//TRUE��ʾ�ɹ���FALSE��ʾʧ��
	{
		 AfxMessageBox("����sdk��ʼ��ʧ��!");
	}
	g_hWnd = this->m_hWnd;
    initdata();
	
	m_box.InsertString(m_box.GetCount(),_T("ǰ������"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ��Ũ��"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰС������"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰ��������"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰ�󵽱�������"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰСѩ����"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰ��ѩ����"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰ�󵽱�ѩ����"));
	m_box.InsertString(m_box.GetCount(),_T("����ɳ������"));
	m_box.InsertString(m_box.GetCount(),_T("��������ɳ��������"));
	m_box.InsertString(m_box.GetCount(),_T("����ǿ�����ױ�����"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰ�����ϴ�"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰ�����ܴ�"));
	m_box.InsertString(m_box.GetCount(),_T("·�泱ʪ"));
	m_box.InsertString(m_box.GetCount(),_T("·���л�ˮ����"));
	m_box.InsertString(m_box.GetCount(),_T("·�濪ʼ�л�ѩ"));
	m_box.InsertString(m_box.GetCount(),_T("·���ѩС��3cm"));
	m_box.InsertString(m_box.GetCount(),_T("·���ѩ����3cm"));
	m_box.InsertString(m_box.GetCount(),_T("��ǰΪ��������"));
	m_box.InsertString(m_box.GetCount(),_T("·��ֲ����ֽ��"));
	m_box.InsertString(m_box.GetCount(),_T("·����ִ󲿷��������"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ��·���������"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ��·������̮��"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ�����³��ֻ���"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ�¹�"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ�¹� ���ڴ�����"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ�¹� ����Ա����"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ�¹� ӵ������"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ�¹� ������ͨ�ж�"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ������"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ������ ����ӵ��"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ������ ��������ӵ��"));
	m_box.InsertString(m_box.GetCount(),_T("����ӵ�� ������ɢ����"));
	m_box.InsertString(m_box.GetCount(),_T("����ӵ�� Ԥ�Ƴ���15��������"));
	m_box.InsertString(m_box.GetCount(),_T("����ӵ�� Ԥ�Ƴ���30��������"));
	m_box.InsertString(m_box.GetCount(),_T("����ӵ�� Ԥ�Ƴ���60��������"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ����"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ���� ����ȡǿ�Ʒ�����ʩ"));
	m_box.InsertString(m_box.GetCount(),_T("ǰ����ͨ���� ������ͨ��"));
	m_box.InsertString(m_box.GetCount(),_T("��·��ʩ��������ҵ"));
	m_box.InsertString(m_box.GetCount(),_T("��·��ʩ��������ҵ  ��ͨӵ��"));

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
	// ��������������ԴͼƬ  
  
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(),   MAKEINTRESOURCE(IDB_BITMAP_TEST)); // IDB_BITMAP_TESTΪ��ԴͼƬID 
	//((CButton *)GetDlgItem(IDC_BUTTON8))->SetBitmap(hBitmap); 
	//HBITMAP hBitmap;   
    //����ͼƬID  
    hBitmap =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BITMAP_TEST),  IMAGE_BITMAP,0,0,  LR_LOADMAP3DCOLORS);    
	hBitmap_1 =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BITMAP_WIHTE),  IMAGE_BITMAP,0,0,  LR_LOADMAP3DCOLORS);    
    //���ָ��̬�ؼ���ָ��    
    //CStatic *pStatic=(CStatic *)GetDlgItem(IDC_STATIC111);//�ؼ�ID     
    //���þ�̬�ؼ�����ʽ��ʹ�����ʹ��λͼ������λ����ʾʹ����    
    //pStatic->ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);     
    //���þ�̬�ؼ���ʾλͼ    
    //pStatic->SetBitmap(hBitmap);

	

	//ͼƬ�ؼ�
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CAsafetyAssistantSystemDlg::initdata()
{
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\";//�õ�xml�ļ��ľ���·��ipictype1
	CString lujing;
	GetPrivateProfileString("ConfigInfo","tplujin","C:\\Users\\Administrator\\Desktop\\tanyiming\\photo",lujing.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	picdizhi=lujing;
	lujing.ReleaseBuffer();
	GetPrivateProfileString("ConfigInfo","videolujin","C:\\Users\\Administrator\\Desktop\\tanyiming\\video",lujing.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//ip
	videodizhi=lujing;
	lujing.ReleaseBuffer();
}
void CAsafetyAssistantSystemDlg::init_all_data()//��ʼ��������Ҫ��ʼ���ı���
{
	m_CurScreen =0;
	m_posX = 0;
	m_posY = 0;
	m_posZoom = 0;

}
void  CAsafetyAssistantSystemDlg::UpdataScreenPos()//����ǰ�������뵽ͼƬ�ؼ���
{
	//GetDlgItem(IDC_SCREEN_WINDOW)->GetClientRect(&m_clientRect);
	 GetDlgItem(IDC_SCREEN_WINDOW)->GetWindowRect(&m_clientRect);   //ȡ�ÿؼ���ȫ�����꣬ m_editΪһ��Edit�ؼ�
    this->ScreenToClient(&m_clientRect);        //ת�ɿͻ��������꣬thisָ��ؼ����ڵĴ��ڡ�
	m_screenRect = m_clientRect;
	m_ptzScreen.MoveWindow(m_screenRect);
	//////////////
	//CRect rect;
   

}
void  CAsafetyAssistantSystemDlg::MessageProc(int nWndID, UINT message)//����Ļ����������Ҽ�������ʱ���õ�ǰѡ��Ļ��������ʾ�������ǰѡ�еĻ���
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAsafetyAssistantSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	//���ƽ���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAsafetyAssistantSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CAsafetyAssistantSystemDlg::DoGetDeviceResoureCfg()
{
	NET_DVR_IPPARACFG IpAccessCfg;
	memset(&IpAccessCfg,0,sizeof(IpAccessCfg));	//��0
	DWORD  dwReturned;

	m_struDeviceInfo.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_IPPARACFG,0,&IpAccessCfg,sizeof(NET_DVR_IPPARACFG),&dwReturned);

	int i;
    if(!m_struDeviceInfo.bIPRet)   //��֧��ip����,9000�����豸��֧�ֽ���ģ��ͨ��
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf_s(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;  //ͨ����
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
	else        //֧��IP���룬9000�豸
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //ģ��ͨ��
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

		//����ͨ��
		for(i=0; i<MAX_IP_CHANNEL; i++)
		{
			if(IpAccessCfg.struIPChanInfo[i].byEnable)  //ipͨ������
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
		
		
	} //֧��IP���룬9000�豸

}
void  CAsafetyAssistantSystemDlg::GetDecoderCfg()//��������ϢҲ����m_struDeviceInfo1.struChanInfo[i].struDecodercfg�д��
{
     NET_DVR_DECODERCFG_V30 DecoderCfg;//������Ž�������Ϣ
	 DWORD  dwReturned;
	 BOOL bRet;
		
      
	 //��ȡͨ����������Ϣ
	 for(int i=0; i<MAX_CHANNUM_V30; i++)
	 {
		 if(m_struDeviceInfo.struChanInfo[i].bEnable)//���ͨ����Ч
		 {
			 memset(&DecoderCfg,0,sizeof(NET_DVR_DECODERCFG_V30));//���
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
bool  CAsafetyAssistantSystemDlg::OnBTLogin(CString strIP,int Port,CString ADMIN,CString PassWord) //��½��ť��ȡ�ø����ؼ��ϵķ�����ip��ַ���˿ںţ��û������������ӷ�������ͬʱ����ǰ����������Ƶͨ������ӽ�ͨ������������
{//�����ǰ������û�������κ�����ͷ����ʱ�������͵�¼��ط���ʹ�ã���������¼�񣬴�ʱͨ��������ʾMulti_Preview����·�طţ�
			int err = 0;	//Storage the possible error return value.
			
			pchDVRIP = (LPSTR)(LPCSTR)strIP;
			WORD wDVRPort=(WORD)Port;//��ö˿�
			char *pchUserName=(LPSTR)(LPCSTR)ADMIN;//��÷������û���
			char *pchPassword=(LPSTR)(LPCSTR)PassWord;//�������
			NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;//��ŵ�����ͷ��Ϣ�Ƚ���
	        memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));

	        LONG lLoginID = NET_DVR_Login_V30(pchDVRIP, Port, pchUserName,pchPassword,&DeviceInfoTmp);
			if(-1 != lLoginID)//����ɹ������Ϸ�����
			{
				 m_LoginID = lLoginID;//����ǰ�������ĵ�½id���������ͨ������Ϳ����жϵ�ǰ�Ƿ�ɹ���½
				 m_struDeviceInfo.lLoginID = lLoginID;//��ȡ����½id
				 m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;//ģ��ͨ������
				 m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;//��������ͷ������
				 m_struDeviceInfo.iStartChan  = DeviceInfoTmp.byStartChan;//��ʼ��ַ
				 DoGetDeviceResoureCfg();  //��ȡ�豸��Դ��Ϣ,
				 //�������������µ���Ϣ������m_struDeviceInfo1�ṹ���д��
				// m_nChannelCount = m_struDeviceInfo[hh].iDeviceChanNum+m_struDeviceInfo[hh].iIPChanNum;//ȡ�õ�ǰ��������ģ��ͨ�����������ʱ����Ҫע�⣬�е���Ƶ�������ӵ���ģ������ͷ������Ҳ�е���Ƶ�������ӵ�����������ͷ
				//�����ǰ��Ƶ��������������Ƶ�������Ļ���ģ��ͨ������ȻΪ0�� m_nChannelCount��ֵ��Ϊ0
				 int nIndex = 0;
				 int j=-1;
					for(int i=0;i< MAX_IP_DEVICE_V40;i++)//��ÿһ·ͨ�����в�������ÿһ·ͨ�������ֳ����ڸ�ѡ����
				    {//m_nChannelCount�д�ŵ��ǵ�ǰ��Ƶ��������ͨ���������û��������ֵ��Ϊ0��������֮���Ϊʵ�ʵ�ͨ������
					  if(m_struDeviceInfo.struChanInfo[i].bEnable)  //ͨ����Ч�����뵽ͨ����������
					  {
						  j++;
					  }
				    }//��ǰ��Ƶ�������м���ͨ�����������ͨ��������������Ӽ������0��ʼ
					PortNum=j+1;
					//CString a;
					//a.Format("ͨ����%d",PortNum);
					//MessageBox(a);
					if(PortNum<=0)
					{
						MessageBox("��ǰ��Ƶ������û�п��õ�����ͷ�����������");
						return 0;
					}
					GetDecoderCfg();//��������ϢҲ����m_struDeviceInfo1.struChanInfo[i].struDecodercfg�д��
					return 1;
			}
			else//���ӷ�����ʧ��
			{
				MessageBox("������Ƶ����������ʧ�ܣ�����������!\n");
		        return 0;
			}
}
bool  CAsafetyAssistantSystemDlg::OnBTLeave() //�������˳���ť��Ҫ��Ͽ� ��Ƶ����������
{
	if(-1 != m_LoginID)//�����ǰ�ǵ�½״̬
		{
			BOOL bSuccess =NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); //�˳���¼
			if(bSuccess)//����ɹ��˳�������
			{
				 m_LoginID = 0;
				 m_struDeviceInfo.lLoginID = -1;
				 return 1;
			}
		    else
			{
				  MessageBox("�˳�ʧ�ܣ������Ƿ�ɹ����ӵ�������Ƶ������!!");
				  m_struDeviceInfo.lLoginID = -1;
				  return 0;
			}
			Invalidate();//ˢ��
			return 1;
		}
	else 
		return 1;
}
HWND   CAsafetyAssistantSystemDlg::GetDispHandle(int nNum)//���ݲ�����ֵ������1--9��Ÿ�������Ƶ�ľ�̬�ı���������һ���ľ��
{
	m_CurScreen = nNum;//ȡ�õ�ǰѡ�еĻ���
	m_ptzScreen.SetActiveWnd(m_CurScreen,TRUE);//��ǰ���汻����
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
void CAsafetyAssistantSystemDlg::StopPlay()//��ָ����ͨ������Ƶֹͣ����
{
	if(m_lPlayHandle != -1)
	{	
		NET_DVR_StopRealPlay(m_lPlayHandle);//ֹͣ����ָ����ͨ��
		m_lPlayHandle=-1;
	}
}
void CAsafetyAssistantSystemDlg::start_haikang()//���ݽ�ȥ����ͨ����,Ҫ��ʾ����Ļ��������ָ����ͨ����Ƶ�����ƶ��Ĵ�������ʾ
{
	int i=MAX_IP_DEVICE_V40;
	int j=-1;
	while(i--)//��ÿһ·ͨ�����в�������ÿһ·ͨ�������ֳ����ڸ�ѡ����
	{//m_nChannelCount�д�ŵ��ǵ�ǰ��Ƶ��������ͨ���������û��������ֵ��Ϊ0��������֮���Ϊʵ�ʵ�ͨ������
		if(m_struDeviceInfo.struChanInfo[MAX_IP_DEVICE_V40-i].bEnable)  //ͨ����Ч�����뵽ͨ����������
		{
			j++;
			if(j==1)
			{
				break;
			}
		}
	}
	//CString aaa;
	//aaa.Format("ʵ��ͨ����%d",j);//����j=0
	//MessageBox(aaa);
	NET_DVR_CLIENTINFO ClientInfo;
	HWND a=GetDispHandle(0);//ȡ�õ�ǰ��̬�ı���ľ����Ҳ����1--9�ǾŸ�������Ƶ�Ŀؼ��е�ĳһ���ľ��
	ClientInfo.hPlayWnd     = a;//ȡ�õ�ǰͼƬ�ؼ��ľ������ʱ��sdkʵ�ֽ��룬ֱ�ӽ���������Ƶ������ʾ�ˣ�������ﴫ��null��
	ClientInfo.lChannel     = MAX_IP_DEVICE_V40-j+1;//1;//m_struDeviceInfo1.struChanInfo[iChanIndex].iChanIndex;//ȡ�õ�ǰͨ����
	ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
	//setshowmode(ClientInfo.lChannel,"dongshuo23456789");
	if(-1== m_lPlayHandle)//�����ǰû�п���Ԥ���Ļ����Ϳ���Ԥ��������Ļ���ֱ���˳���
	{
		m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);
	}
	else
		return;
	if(-1 == m_lPlayHandle)
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format("���ų����������%d",err);
		MessageBox(m_csErr);
	}
}
void CAsafetyAssistantSystemDlg::OnBnClickedButton5()//���������
{
	
	CString sFile = "config.ini";
	char chPath[MAX_PATH];
	GetModuleFileName(NULL,chPath,MAX_PATH);
	CString sDirectoryPath = chPath;
	int nCount = sDirectoryPath.ReverseFind('\\');
	sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
	CString sFilePath = sDirectoryPath + "\\" + sFile;//�õ�xml�ļ��ľ���·��ipictype1	
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
	GetPrivateProfileString("ConfigInfo","admin","admin",ADMIN.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//�û���
	GetPrivateProfileString("ConfigInfo","password","gonglusuo123456",PassWord.GetBuffer(MAX_PATH),MAX_PATH,sFilePath);//����
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
	//wDVRPort=(WORD)atoi(serverPort);//��ö˿�
	//pchUserName=(LPSTR)(LPCSTR)ADMIN;//��÷������û���
	//pchPassword=(LPSTR)(LPCSTR)PassWord;//�������
	
	////////////////////////////////////
	CString  s;
	GetDlgItemText(IDC_BUTTON5,s);
	if(0 == s.Compare("�����豸")) //��ͬ
	{		
		if(camera_EN==1)
		{
			if(1==OnBTLogin(serverIP,atoi(serverPort),ADMIN,PassWord))
			{
				SetDlgItemText(IDC_BUTTON5,_T("�Ͽ�����"));
				start_haikang();//�����������е�ͨ��
			}
			else
			{
				SetDlgItemText(IDC_BUTTON5,_T("�����豸"));
				StopPlay();//��ָ����ͨ������Ƶֹͣ����
			}
		}
		
		if(GPS_EN==1)
		{
			OpenGPSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("�Ͽ�����"));
		}

		
		if(GPRS_EN==1)
		{
			OpenGPRSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("�Ͽ�����"));
		}

		
		if(Radar_EN==1)
		{
			OpenRadarchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("�Ͽ�����"));
		}
	}
	else
	{
		
		if(camera_EN==1)
		{
			StopPlay();
			OnBTLeave();//�˳�����������
			SetDlgItemText(IDC_BUTTON5,_T("�����豸"));
		}
		if(GPS_EN==1)
		{
			CloseGPSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("�����豸"));
		}	
		if(GPRS_EN==1)
		{
			CloseGPRSchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("�����豸"));
		}		
		if(Radar_EN==1)
		{
			CloseRadarchuankou();
			SetDlgItemText(IDC_BUTTON5,_T("�����豸"));
		}
	}
	CString tmp_AlarmPara[6];
	// Ԥ������
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
	CString sFilePath = sDirectoryPath + "\\" + sFile;//�õ�xml�ļ��ľ���·��ipictype1	
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
	if(0 != s.Compare("�����豸")) //��ͬ
	{
		if(camera_EN==1)
		{
			StopPlay();
			OnBTLeave();//�˳�����������
			SetDlgItemText(IDC_BUTTON5,_T("�����豸"));
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
	//m_ReceiveData.Empty();  //��ս��������ַ���
}
void CAsafetyAssistantSystemDlg::closechuankou()
{
	
	
	
}
void CAsafetyAssistantSystemDlg::CloseGPSchuankou()
{
	m_Port1.ClosePort();//�رմ���
	jingdu_new=0;
		jingdu_old=0;
		weidu_new=0;
		weidu_old=0;
		firstfindgps=0;
}
void CAsafetyAssistantSystemDlg::CloseGPRSchuankou()
{
	m_Port2.ClosePort();//�رմ���
	gprsopen=0;
}
void CAsafetyAssistantSystemDlg::CloseRadarchuankou()
{
	m_Port3.ClosePort();//�رմ���
	GetDlgItem(IDC_zuoqiansudu)->SetWindowText("δ����");
	GetDlgItem(IDC_youqianchesu)->SetWindowText("δ����");
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
		AfxMessageBox("û�з���GPS���ڻ�ռ��");
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
		AfxMessageBox("û�з���GPRS���ڻ�ռ��");
	}
}
void CAsafetyAssistantSystemDlg::OpenRadarchuankou()
{
	if (m_Port3.InitPort(this, Radar_m_nCom+1, Radar_m_nBaud,Radar_m_cParity,Radar_m_nDatabits,Radar_m_nStopbits,Radar_m_dwCommEvents,512))
	{
		m_Port3.StartMonitoring();
		GetDlgItem(IDC_zuoqiansudu)->SetWindowText("����");
		GetDlgItem(IDC_youqianchesu)->SetWindowText("����");
	}
	else
	{
		AfxMessageBox("û�з����״ﴮ�ڻ�ռ��");
	}
}
void CAsafetyAssistantSystemDlg::openchuankou()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������		

	//m_Port1.ClosePort();//�رմ���
	
}

void CAsafetyAssistantSystemDlg::OnBnClickedButton6()//��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSet  cDlg;
	//CPasswordDialog cDlg;//��������  
	if(cDlg.DoModal()==IDOK)//�û�����ȷ��  
	{  
        wDVRPort=cDlg.wDVRPort;//=(WORD)nPort;//��ö˿�
		pchDVRIP=cDlg.pchDVRIP;
		pchUserName=cDlg.pchUserName;
		pchPassword=cDlg.pchPassword;

		GPS_m_nBaud=cDlg.GPS_m_nBaud;       //������
		GPS_m_nCom=cDlg.GPS_m_nCom;         //���ں�
		GPS_m_cParity=cDlg.GPS_m_cParity;    //У��
		GPS_m_nDatabits=cDlg.GPS_m_nDatabits;    //����λ
		GPS_m_nStopbits=cDlg.GPS_m_nStopbits;    //ֹͣλ

		GPRS_m_nBaud=cDlg.GPRS_m_nBaud;       //������
		GPRS_m_nCom=cDlg.GPRS_m_nCom;         //���ں�
		GPRS_m_cParity=cDlg.GPRS_m_cParity;    //У��
		GPRS_m_nDatabits=cDlg.GPRS_m_nDatabits;    //����λ
		GPRS_m_nStopbits=cDlg.GPRS_m_nStopbits;    //ֹͣλ

		Radar_m_nBaud=cDlg.Radar_m_nBaud;       //������
		Radar_m_nCom=cDlg.Radar_m_nCom;         //���ں�
		Radar_m_cParity=cDlg.Radar_m_cParity;    //У��
		Radar_m_nDatabits=cDlg.Radar_m_nDatabits;    //����λ
		Radar_m_nStopbits=cDlg.Radar_m_nStopbits;    //ֹͣλ
		CString sFile = "config.ini";
		char chPath[MAX_PATH];
		GetModuleFileName(NULL,chPath,MAX_PATH);
		CString sDirectoryPath = chPath;
		int nCount = sDirectoryPath.ReverseFind('\\');
		sDirectoryPath = sDirectoryPath.Mid(0, nCount + 1);
		CString sFilePath = sDirectoryPath + "\\" + sFile;//�õ�xml�ļ��ľ���·��ipictype1
		if(GPRS_m_nCom==GPS_m_nCom)
		{
			GPRS_m_nCom=-1;
			MessageBox("GPRS�豸���ںŲ�����GPS�豸��ͬ������������");
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
			MessageBox("�״��豸���ںŲ�����GPS���豸��ͬ������������");
			return;
		}

		// Ԥ��������ȡ
		memcpy(alarmPara, cDlg.AlarmP, sizeof(alarmPara));
	} 
}



int rxdatacount=0;  //�ñ������ڽ����ַ�����
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
			//::SendMessage(g_hWnd, WM_USERMESSAGE, (DWORD)cheliangchangdu, NULL);//����⵽�ĳ����¶ȸ����͵���������ʾ
			if (g_hWnd != INVALID_HANDLE_VALUE)
			{
				::SendMessage(g_hWnd, WM_USERMESSAGE, NULL,NULL);
			}
		}
		str.Format("%c ",ch);
		gps_show += str;
		SetDlgItemText(IDC_STATIC_jingdu,gps_show );*/
		if((USART1_RX_STA&0x8000)==0)//����δ��� ����û�н��յ�һ֡���ݵ�ʱ��
		 {						
			if(USART1_RX_STA<6)	//������յ������ݳ���С��6����ʾ�����ڽ�������ͷ�Ľ׶Σ�����ͷΪ$GPGGA  $GPRMC
			{
				if(USART1_RX_STA==0) //�жϵ�һ���ֽ��Ƿ�Ϊ$
				{ //����Ϊ0��ʱ�򣬱�ʾ�տ�ʼ����
					if(ch==0x24) // ������յ��ĵ�һ���ַ�Ϊ$0x24
					{	
						USART1_RX_BUF[USART1_RX_STA]=ch ;//������ַ����뵽���������д��
						USART1_RX_STA++;
					}
					else //���һ��ʼ���յ����ַ�����$�ͱ�ʾ���յ�������֡�д������¿�ʼ����				
					 USART1_RX_STA=0;
					
				}else //����Ѿ����ܵ��˵�һ������ͷ			
				if(USART1_RX_STA==1) //���֮ǰֻ���յ�һ���ַ��Ļ�
				{
					if(ch==0x47)  //������յ��ĵڶ����ַ�ΪG
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ; //���ڶ����ַ�G���뵽�ַ�������
						USART1_RX_STA++;
					}else
	          		USART1_RX_STA=0;					
				}else 
				if(USART1_RX_STA==2)
				{
					if(ch==0x50) //������յ��ĵ������ַ���P
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
	        		}else
					USART1_RX_STA=0;
					
				}else 
				if(USART1_RX_STA==3) //���յ��ĵ�4���ֽڿ�����G R V
				{
					if(ch=='R')  //������յ��ĵ�4���ַ���G 0x47
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					/*else if(ch==0x52)  //������յ��ĵ�4���ַ���R	 �����ģʽ2
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}else if(ch==0x56)  //������յ��ĵ�4���ַ���V	  �������ģʽ3
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}*/
					else
					  USART1_RX_STA=0;
					
				}else 
				if(USART1_RX_STA==4)	//���յ��ĵ�����ֽڿ�����G M T S			
				{
					if(ch=='M')	//������յ��ĵ�5���ַ���	G0x47
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					/*else  if(ch==0x4D)	//������յ��ĵ�5���ַ���	M
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}else if(ch==0x54)	//������յ��ĵ�5���ַ���	T
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;						
					}else if(ch==0x53)	//������յ��ĵ�5���ַ���	S
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					*/else
					  USART1_RX_STA=0;
					
				}else 
				if(USART1_RX_STA==5)  //���յ��ĵ�6���ֽڿ�����A C G V 
				{
					if(ch=='C') //������յ��ĵ�6���ַ���A0x41
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}
					/*else if(ch==0x43) //������յ��ĵ�6���ַ���C
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					} else if(ch==0x47) //������յ��ĵ�6���ַ���G
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}else if(ch==0x56) //������յ��ĵ�6���ַ���V
					{
						USART1_RX_BUF[USART1_RX_STA]=ch ;
						USART1_RX_STA++;
					}*/else
					  USART1_RX_STA=0;					
				} 
			}
			else //����������������ͷ�������
			 {
					//�س����е�ascll��ֱ�Ϊ13,10��ʮ�����Ʊ�ʾ����0x0d��0x0a
					if(USART1_RX_STA&0x4000)//���յ���0x0d Ҳ���ǽ��յ��˻س�����
					{
						if(ch!=0x0a)  USART1_RX_STA=0;//���մ���,���¿�ʼ
						else 
						{	
						     USART1_RX_STA &= 0x8000;	//���������
							 //::SendMessage(g_hWnd, WM_USERMESSAGE, (DWORD)cheliangchangdu, NULL);//����⵽�ĳ����¶ȸ����͵���������ʾ
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
						if(ch==0x0d) //�����ǰ���յ��˻س����з����ͱ�ʾ���յ�������β
						{
							USART1_RX_STA|=0x4000;//���ܵ�0x0d�ͽ���־λ��1								
							//�����ǰ���յ����ǻس������ǲ���һgprmc��β�ģ�ֱ�Ӳ������ⲿ�֣�Ҳ����¼
						}
						else
						{
							USART1_RX_BUF[USART1_RX_STA&0X3FFF]=ch;
							USART1_RX_STA++;
							if(USART1_RX_STA>(255-1))
								USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
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
	else  if(port==(1+Radar_m_nCom))//�״�����
	{
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{			
			if(USART2_RX_STA<3)	//������ݳ���С��3����ʾ���յ���������ͷ������ͷΪ0x05��0x10,0x08
			{
				if(USART2_RX_STA==0)  //�տ�ʼ�������ݵĻ�
				{
					if(ch==0x05) //���յ��ĵ�һ���ֽ�Ϊ0x05��
					{
						nearestCar_flag = false;	// ���ڴ˴����߼����գ�����޸Ľ����߼�
						USART2_RX_BUF[USART2_RX_STA]=ch ;
						USART2_RX_STA++;
					}else
					  USART2_RX_STA=0;
					
  			    }else if(USART2_RX_STA==1)//���յ��ڶ����ַ��Ļ��������0x10�ͱ�ʾ��ȷ
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
				else if(USART2_RX_STA==2)//���յ���3���ַ��Ļ��������0x08�ͱ�ʾ��ȷ
				{
					if(ch==0x08)
					{
						USART2_RX_BUF[USART2_RX_STA]=ch ;
						USART2_RX_STA++;						
					}else
					  USART2_RX_STA=0;					
				}
			}
			else //����������ݲ���
			{
				if(ch==0xED)//(ch==0xBB) //������յ�����β
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
						USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	
				}
			}
        }//if((USART2_RX_STA&0x8000)==0)//����δ���
	}
	//�������ˡ��Զ���ա�����ﵽ50�к��Զ���ս��ձ༭������ʾ������
	//���û�С��Զ���ա��������дﵽ400��Ҳ�Զ����
	//��Ϊ���ݹ��࣬Ӱ������ٶȣ���ʾ�����CPUʱ��Ĳ���

	//���ѡ����"ʮ��������ʾ"������ʾʮ������ֵ
	
	//�����ǽ����յ��ַ������ַ�������������ʱ�ܶ�
	//�����ǵ�������Ҫ������ļ�������û����List Control
	//m_ctrlReceiveData.SetSel(0, -1);
	//m_ReceiveData+=str;
	return 0;
}

//��һ���ַ�����Ϊʮ�����ƴ�ת��Ϊһ���ֽ����飬�ֽڼ���ÿո�ָ���
//����ת������ֽ����鳤�ȣ�ͬʱ�ֽ����鳤���Զ����á�
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

void CAsafetyAssistantSystemDlg::OnBnClickedOk()//����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	CString  s,s1,s2,s3;
	GetDlgItemText(IDC_jingdu,s1);
	GetDlgItemText(IDC_weidu,s2);
	GetDlgItemText(IDC_EDIT2,s3);
	s.Format("%s%s%s",s1,s2,s3);
	if(1==gprsopen)
		m_Port2.WriteToPort(s);  //

}
LRESULT CAsafetyAssistantSystemDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)////��һ����ͨ��������֮��ᷢ��һ��һ���ɼ����ó����¶�ֵ�ĸ������������������ж��Ƿ��г�����������ǰ��
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
//����GPS���� 
void CAsafetyAssistantSystemDlg::Get_GPS(char *a)
{
	char timeutc[10];        
	char WD[12]={0};            //γ��
	char WD_a=0;            //γ�ȷ���
	char JD[12]={0};            //����	  
	char JD_a=0;            //���ȷ���
	int lock;            //��λ״̬
	char use_sat[12]={0};        //ʹ�õ�������
	char high[12];        //�߶�
	char strspeed[12] = {0}; //�ٶ�
	
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
	  int byte_counter=0;//�����Խ��յ������ݽ��м���
	  int byte_num=0;
	  int seg_counter=0;//��������Σ��ö��Ÿ����Ķ�
	  char tmp;
	  byte_counter=0;
	  seg_counter=0;
	  byte_num=0;
	  do
	  {
		 tmp=a[byte_counter];//�����ڻ������е����ݷ�����ʱ�����д��

		 if(byte_counter==80) break;//������ȳ���80��ֱ���˳�ѭ��
		 else byte_counter++;
		 if(tmp==',') //�����������
		 {
			 seg_counter++; byte_num=0;
		 }
		 else//��ǰ�ַ����Ƕ��ţ��Ǿ���һ������
		 {
			 switch(seg_counter)//�����ݽ��зֶ�
			 {
                case 0:	//$GPRMC
					byte_num=0;							  
					break;
			 	case 1:	//��һ�����ź���ʵʱʱ�䣬utcʱ��
					timeutc[byte_num]=tmp;
					if(byte_num<9)			 //����
					{
						byte_num++;
					}
					else byte_num=0;							  
					break;
				case 3:                      //����
		            WD[byte_num]=tmp;		           
				    //if(byte_num<10)
		            {
						byte_num++;
		            }
					//else byte_num=0;
		            break;
		        case 4:                      //γ�ȷ�����
		            WD_a=tmp;
					//if(byte_num<1)
		            {
						byte_num++;
		            }
					//else 
						//byte_num=0;
		            break;
		        case 5:                                //���ȴ���
		            JD[byte_num]=tmp;		           
				    //if(byte_num<9)
		            {
						byte_num++;
		            }
					//else byte_num=0;
		            break;
		        case 6: 
					JD_a=tmp;
//					byte_num=0;                               //���ȷ�����
//		            if(byte_num<1)
//		            {
//		                JD_a=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;						   
		        case 2:
		            lock=tmp;				                //��λ�ж�,��λ����ָʾ��V=��λ��Ч��A=��λ��Ч
//					byte_num=0;
//		            if(byte_num<1)
//		            {
//		                lock=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;
		        case 8:                                //���溽��
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
		        case 7:                                //��������
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
	// // SetDlgItemText(IDC_BUTTON5,_T("�Ͽ�����"));
	 JD_ddd = (JD[0] - '0')*100 + (JD[1] - '0')*10 + (JD[2] - '0');	//dd
	 JD_mm = atof((JD+3));	//mm.mmmm
	 CString s, tmp_dd, tmp_mm;
	 tmp_dd.Format("%d", JD_ddd); //��
	 tmp_mm.Format("%.2f", JD_mm); //��
	 s = tmp_dd + "��" + tmp_mm + "��";
	 SetDlgItemText(IDC_jingdu,s);

	 s.Format("%c",JD_a);
	 SetDlgItemText(IDC_jingdufangwei,s);

	 WD_dd = (WD[0] - '0')*10 + (WD[1] - '0');	//dd
	 WD_mm = atof((WD+2));	//mm.mmmm
	 //CString s, tmp_dd, tmp_mm;
	 tmp_dd.Format("%d", WD_dd); //��
	 tmp_mm.Format("%.2f", WD_mm); //��
	 s = tmp_dd + "��" + tmp_mm + "��";
	 SetDlgItemText(IDC_weidu,s);
	 s.Format("%c",WD_a);
	 SetDlgItemText(IDC_weidufangwei,s);
	 char test[10] = "20";
	 speed = atof(strspeed);	// ��λ������
	 speed *= 1.852;		// ת��ΪKm/h		 
		 
	s.Format("%.2f",speed);
	GetDlgItem(IDC_sudu)->SetWindowText(s+"Km/h");
	//myCar[1].time = GetTickCount(); // ��ȡϵͳʱ��
	myCar[1].vel = speed;
	// ������ٶ���Ϣ
	myCar[1].accel = (myCar[1].vel - myCar[0].vel) / ((double)( 200.0 )/1000*3.6 );  // ����ʱ����
	s.Format("%.2f", myCar[1].accel);
	SetDlgItemText(IDC_EDIT_ACCE, s + "m/s^2");
	memcpy(&(myCar[0]) , &(myCar[1]), sizeof(MyCar_Motion));


	// ����GPS�Ĵ���������������
    int tmp_alarm = Alarm_Algorithm(alarmPara, speed / 3.6, nearestCar_Vel, nearestCar_Accle, nearestCar_flag, nearestCar_Distance);
	ShowAlarmLight(tmp_alarm);	// ÿ��ִ��һ��
}
void CAsafetyAssistantSystemDlg::Get_GPS( )
{
	char timeutc[10];        
	char WD[12]={""};            //γ��
	char WD_a=0;            //γ�ȷ���
	char JD[12]={""};            //����	  
	char JD_a=0;            //���ȷ���
	int lock;            //��λ״̬
	char use_sat[2]={""};        //ʹ�õ�������
	char high[12];        //�߶�
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
	  int byte_counter=0;//�����Խ��յ������ݽ��м���
	  int byte_num=0;
	  int seg_counter=0;//��������Σ��ö��Ÿ����Ķ�
	  char tmp;
	  byte_counter=0;
	  seg_counter=0;
	  byte_num=0;
	  do
	  {
		 tmp=USART1_RX_BUF[byte_counter];//�����ڻ������е����ݷ�����ʱ�����д��

		 if(byte_counter==80) break;//������ȳ���80��ֱ���˳�ѭ��
		 else byte_counter++;
		 if(tmp==',') //�����������
		 {
			 seg_counter++; byte_num=0;
		 }
		 else//��ǰ�ַ����Ƕ��ţ��Ǿ���һ������
		 {
			 switch(seg_counter)//�����ݽ��зֶ�
			 {
                case 0:	//��һ�����ź���ʵʱʱ�䣬utcʱ��
					byte_num=0;							  
					break;
			 	case 1:	//��һ�����ź���ʵʱʱ�䣬utcʱ��
					timeutc[byte_num]=tmp;
					if(byte_num<9)			 //����
					{
						byte_num++;
					}
					else byte_num=0;							  
					break;
				case 2:                               //γ�ȴ���
		            WD[byte_num]=tmp;		           
				    if(byte_num<8)
		            {
						byte_num++;
		            }
					else byte_num=0;
		            break;
		        case 3:                                //γ�ȷ�����
		            WD_a=tmp;
					if(byte_num<1)
		            {
						byte_num++;
		            }
					else 
						byte_num=0;
		            break;
		        case 4:                                //���ȴ���
		            JD[byte_num]=tmp;		           
				    if(byte_num<9)
		            {
						byte_num++;
		            }
					else byte_num=0;
		            break;
		        case 5: 
					JD_a=tmp;
//					byte_num=0;                               //���ȷ�����
//		            if(byte_num<1)
//		            {
//		                JD_a=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;						   
		        case 6:
		            lock=tmp;				                //��λ�ж�,��λ����ָʾ��0=��λ��Ч��1=��λ��Ч
//					byte_num=0;
//		            if(byte_num<1)
//		            {
//		                lock=tmp;
//						byte_num++;
//		            }
//					else byte_num=0;
		            break;
		        case 7:                                //��λʹ�õ�������
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
		        case 9:                                //�߶ȴ���
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
	 // SetDlgItemText(IDC_BUTTON5,_T("�Ͽ�����"));
	  CString s;
	  s.Format("%s",JD);
	  SetDlgItemText(IDC_jingdu,s);
	  //east ��(69) west �� (87)
	  //north ��(78)south �� (83)
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
LRESULT CAsafetyAssistantSystemDlg::OnMyMessage1(WPARAM wParam, LPARAM lParam)////��һ����ͨ��������֮��ᷢ��һ��һ���ɼ����ó����¶�ֵ�ĸ������������������ж��Ƿ��г�����������ǰ��
{
	Real_T_DATA* d;
	d=(Real_T_DATA*)wParam;

	int tmp_count = 0;
	tmp_count = d->byte_num / 11; // ���㹲���鳵����Ϣ����
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
	memset(&(forwardCar[1]), 0, sizeof(FCar_Motion));  //����ǰ������Ϣ����
	//forwardCar[1].time = tmp_time;// ��ȡ��ǰʱ��	
	for (int i = 0; i < tmp_count; i++)
	{
		if (d->a[11*i] == 0x05)
		{
			if (d->a[11*i+1] == 0x10 + i)
			{
				if (d->a[11*i+2] == 0x08)
				{
					//[���ݸ�ʽ]��
					//--05 10 08 00 4D 01 1E 7B FB 23 5E  (Ŀ����Ϣ)
					//--0000 0000 0100 1101 0000 0001 0001 1110 0111 1011 1111 1011 0010 0011 0101 1110
					//--ת��Ϊ�����ƺ��ȡ��Ӧ��λ������:
					//--000(3λ) 0000001(7λ) 00110(5λ) 10000000100(11λ) 01111001111(11λ) 0111111101100(13λ) 1000110101111(13λ) 0(1λ)
					//--�����������Ϊ������ ת��Ϊ ʮ����ֵ 
					//--000(0) 0000001(1) 00110(6)
					//--10000000100(1028) ���㳵��(Y����ĳ��ٲ��ù�)
					//--01111001111(975)  ���㳵�� X_Speed=(975-1024)*0.1 = -4.9(m/s) = -17(km/h)
					//--0111111101100(4076)  ���㳵��λ�� Y_Pos=(4076-4096)*0.064 = -1.28 
					//--1000110101111(4527)  �������λ�� X_Pos=(4527-4096)*0.064 = 27.58		   

					//�����ֽڵ���λ��ʼ�������ֽڵ�5λ����Ϊ�ٶȣ���11λ
					raw_ID[i] = ((d->a[11*i+3]&0x1F)<<2) + ((d->a[11*i+4]&0xC0)>>6);	// �����ֽڵĵ�5λ �� �����ֽڵĸ�2λ ƴ��
					raw_speed_x[i] = ((d->a[11*i+6]&0x3F)<<5) + ((d->a[11*i+7]&0xF8)>>3);
					raw_pos_y[i] = ((d->a[11*i+7]&0x07)<<10) + ((d->a[11*i+8])<<2) + ((d->a[11*i+9])>>6);
					raw_pos_x[i] = ((d->a[11*i+9]&0x3F)<<7) + ((d->a[11*i+10])>>1);
					//double test = 0; // for test
					speed_x[i] = (raw_speed_x[i] - 1024) * 0.1 * 3.6;
					//test = speed_x[i];
					pos_x[i] = (raw_pos_x[i] - 4096) * 0.064;
					//test = pos_x[i];
					pos_y[i] = (raw_pos_y[i] - 4096) * 0.064;	// y����ȡ���źͻ�ͼ�еķ��ű���һ�¡�
					//test = pos_y[i];
					tmp_exam_count++;
					// ��¼��ǰÿ������λ�á��ٶ���Ϣ�������㴦���ٶ���Ϣ
					forwardCar[1].pos_x[i] = pos_x[i];
					forwardCar[1].vel[i] = speed_x[i];
					//double test = (forwardCar[1].time - forwardCar[0].time) / 1000 * 3.6;
					//forwardCar[1].accel[i] = (forwardCar[1].vel[i] - forwardCar[0].vel[i]) / ((forwardCar[1].time - forwardCar[0].time) / 1000 * 3.6);	// �����ÿ�����ļ��ٶ�
					forwardCar[1].accel[i] = (forwardCar[1].vel[i] - forwardCar[0].vel[i]) / ( (20.0) / 1000 * 3.6);	// �����ÿ�����ļ��ٶ�
					forwardCar[1].ID[i] = raw_ID[i];
				}
			}
		}
	} // ���������г�����Ϣ����	
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
				} // �����ȡ��ǰһ���ٶȺ͵�ǰ�ٶ����ݾ���Ϊ��
			}
		}
	} // ��ȡy��������4m�ڵĳ�����
	
	if ( count_nearest == 0 )
	{
		nearest_distance = 0;
		nearest_vel = 0;
		nearest_accel = 0;
		tmpflag = false;
	} // ���û��y��������4m�ڵĳ�����Ϣ����ʾ��������

	memcpy(&forwardCar[0] , &forwardCar[1], sizeof(FCar_Motion));	// ����ǰ���ݸ�ֵ����һ������

	// ��ʾy������4m��Χ�ڣ�x������������һ����������ʾ����
	m_str.Format("%.2f", nearest_distance);
	SetDlgItemText(IDC_youqianchesu,m_str + "m");
	m_str.Format("%.2f", nearest_vel);
	SetDlgItemText(IDC_zuoqiansudu,m_str + "km/h");
	m_str.Format("%.2f", nearest_accel);
	SetDlgItemText(IDC_EDIT_FACCE,m_str + "m/s^2");

	//////////////////////////////////////////////////////////////////////////
	// ��ǰ���Ĳ�����ֵ��ȫ�ֱ���
	nearestCar_Accle = nearest_accel;
	nearestCar_Distance = nearest_distance;
	nearestCar_Vel = nearest_vel /3.6;
	nearestCar_flag = tmpflag;
	//////////////////////////////////////////////////////////////////////////

	if (tmp_exam_count == tmp_count) 
	{
		m_carInfo.car_num = tmp_count;
		memcpy(m_carInfo.PosCarX, pos_x, tmp_count*sizeof(double));	// �״������ͼ�����겻һ��, X = Y
		memcpy(m_carInfo.PosCarY, pos_y, tmp_count*sizeof(double));
		memcpy(m_carInfo.VelocityX, speed_x, tmp_count*sizeof(double));
		memcpy(m_carInfo.ID, raw_ID, tmp_count*sizeof(int));

		InvalidateRect(m_rc);	// ˢ�½�����Ϣ
	}// ���ɼ��������ݺͼ���������Ƿ�һ��

	delete [] raw_speed_x;
	delete [] raw_pos_x;
	delete [] raw_pos_y;
	delete [] speed_x;
	delete [] pos_x;
	delete [] pos_y;

	USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����
	return 0;
}
void CAsafetyAssistantSystemDlg::OnBnClickedButton1()//����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	zhuatu();//ץȡָ��ͨ����ͼu 
}

void CAsafetyAssistantSystemDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString  s;
	GetDlgItemText(IDC_BUTTON3,s);
	if(0 == s.Compare("��ʼ¼����Ƶ")) //��ͬ
	{
		char videoName[256] = {0};
		SYSTEMTIME stUTC;  
		::GetLocalTime(&stUTC);  
		//��ʾʱ��ļ����   
		sprintf_s(videoName,"%s\\%04d%02d%02d%02d%02d%02d%02d.mp4",videodizhi, stUTC.wYear,stUTC.wMonth,stUTC.wDay, \
				stUTC.wHour, stUTC.wMinute,stUTC.wSecond,stUTC.wMilliseconds);
		if (NET_DVR_SaveRealData(m_struDeviceInfo.lLoginID,videoName))
		{
			SetDlgItemText(IDC_BUTTON3,_T("ֹͣ¼����Ƶ"));
		}
	}
	else
	{
		
		if (NET_DVR_StopSaveRealData(m_struDeviceInfo.lLoginID))
		{
			SetDlgItemText(IDC_BUTTON3,_T("��ʼ¼����Ƶ"));
		}
		
	}
}
void CAsafetyAssistantSystemDlg::OnBnClickedButton4()//·������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSETLUJIN td;  
	if(IDOK==td.DoModal())
	{
		picdizhi=td.GETSaveList();;//������ŵ�ǰͼ��ʶ����ϴ�������·��
		videodizhi=td.GETSaveList1();;//������ŵ�ǰͼ��ʶ����ϴ�������·��
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
int CAsafetyAssistantSystemDlg::zhuatu()//ץȡָ��ͨ����ͼ
{
	if(m_lPlayHandle == -1)
	{
		return 0;
	}
	char PicName[256] = {0};
	SYSTEMTIME stUTC;  
    ::GetLocalTime(&stUTC);  
    //��ʾʱ��ļ����   
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GPS_m_nCom=i;
}

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo11(int i)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GPRS_m_nCom=i;
}

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo7(int i)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Radar_m_nCom=i;
}

void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo4(int i)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


CString CAsafetyAssistantSystemDlg::OnBUTTONSure(double in_JD1,double in_WD1,double in_JD2,double in_WD2) //����������Եõ�����
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString i;
//	i=OnBUTTONSure(45,45,50,50);
//	Get_GPS( );
//
//} 


void CAsafetyAssistantSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		  time_count[alarmLast]++;  // ʱ�������
		  if(!bFlash)
		  {
			  switch(alarmLast)
			  {					
			  case SAFE:
				  //InvalidateRect(alarmLamp_rc);
				  DrawAlarmLight(RGB(0, 255, 0));     // ��ɫ
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
				  DrawAlarmLight(RGB(255, 255, 0));	// ��ɫRGB(255,255,0)
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
				  DrawAlarmLight(RGB(255, 0, 0));		// ��ɫ
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
				  DrawAlarmLight(RGB(255, 0, 0));		// ��ɫ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAsafetyAssistantSystemDlg::OnStnClickedgaocheng()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAsafetyAssistantSystemDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT1,"");
}


void CAsafetyAssistantSystemDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CAsafetyAssistantSystemDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tmp;
	m_box.GetLBText(m_box.GetCurSel(), tmp);
	SetDlgItemText(IDC_EDIT2,  tmp);
}

void CAsafetyAssistantSystemDlg::DrawBackground() 
{
	// ����ͼ��ʾ���� �������
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

	// ԭ����
	CPen cu[2];
	CString m_str;

	cu[0].CreatePen(PS_DOT, 1, RGB(0,0,0));//���Ʊ���
	CPen* oldPen = pDC->SelectObject(&cu[0]);

	const int road_num = 5; // �ڽ����л���5������
	//���Ƴ�����
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
		pDC->LineTo(rc.Width(), i * rc.Height()/10);  // ����100mm����ÿ��10m��һ��

		m_str.Format("%dm", (10-i)*10);
		CFont m_font;
		m_font.CreatePointFont(rc.Height()/4, "����");
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
	//	m_font.CreatePointFont(rc.Height()/4, "����");
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
	Pos_x = -tmp;	// �״�����ϵת��Ϊͼ������ϵ
	const int length = 100;

	cu.CreateSolidBrush(RGB(255,0,0));//��������
	CBrush* oldBrush = pDC->SelectObject(&cu);

	m_pen[0].CreatePen(PS_SOLID, 3, RGB(255,0,0));
	CPen* oldPen = pDC->SelectObject(&m_pen[0]);	

	init_x = rc.Width()/2 - rc.Height()/length + rc.Height()*Pos_x/length;	// 	�����в���ƫ��ʱ��������λ�á� rc.Height()/100Ϊ��λ���ȶ�Ӧ�����ص���룬�������Ŀ��Ϊ1m
	init_y = rc.Height() - rc.Height()*Pos_y/length;	// ��ʼ��
	// ��������ͼ��
	pDC->Rectangle(init_x, init_y, init_x + rc.Height()*2/length, init_y - rc.Height()*4/length); // ����4m����2m
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	m_pen[1].CreatePen(PS_SOLID, 3, RGB(0,0,255));
	oldPen = pDC->SelectObject(&m_pen[1]);
	// �����ٶ���
	pDC->MoveTo(init_x + rc.Height()/length, init_y - rc.Height()*4/length);
	pDC->LineTo(init_x + rc.Height()/length, init_y - rc.Height()*4/length - k*velocity);
	pDC->SelectObject(oldPen);

	m_str.Format("%.1fkm/h", velocity);
	CFont m_font;
	m_font.CreatePointFont(rc.Height()/6, "����");
	CFont* m_pOldFont = (CFont*)pDC->SelectObject(&m_font);

	//pDC->TextOutA(init_x , init_y - rc.Height()*4/100 - 18, m_str);
	m_str.Format("%d", ID);
	//pDC->TextOutA(init_x , init_y - rc.Height()*3/100 + 10, "ID:" + m_str);

	pDC->SelectObject(m_pOldFont);
}

void CAsafetyAssistantSystemDlg::DrawAlarmLight(COLORREF cColor)
{
	CStatic *pStatic=(CStatic *)GetDlgItem(IDC_STATIC_ALARMLIGHT);//�ؼ�ID 
	
	CRect tmp_rect;
	GetDlgItem(IDC_STATIC_ALARMLIGHT)->GetClientRect(&tmp_rect);
	const int endge_size = 10;

	if (pmDC != NULL)
	{
		CBrush m_cu;
		CPen m_pen;
		m_cu.CreateSolidBrush(cColor);//���ƾ�ʾ��־
		CBrush* oldBrush = pmDC->SelectObject(&m_cu);
		m_pen.CreatePen(PS_SOLID, 3, cColor);
		CPen* oldPen = pmDC->SelectObject(&m_pen);
		pmDC->Ellipse(tmp_rect.Width()/2-tmp_rect.Height()/2 + endge_size, endge_size, tmp_rect.Width()/2+tmp_rect.Height()/2 - endge_size, tmp_rect.Height() - endge_size);
		pmDC->SelectObject(oldBrush);
		pmDC->SelectObject(oldPen);
	}	
}

/*
*  Ԥ���㷨����:Alarm_Algorithm();
*  ������alarm_para[6]���̶��Ĳ�����v_a�������ٶȡ�v_b:ǰ���ٶȡ�a_b:ǰ�����ٶȡ�flag:�Ƿ���ǰ����Sab:ǰ�����롣
*  ����ֵ: 0��SAFE, 1:THREAT, 2:DANGEROUS, 3:GODBLESSYOU, -1, û��ǰ����
*  
*/

int CAsafetyAssistantSystemDlg::Alarm_Algorithm(double alarm_para[6], double v_a, double v_b, double a_b, bool flag, double Sab)
{
	double Sp = -1;
	double Sr = -1;
	double Se1 = -1;
	double Se2 = -1;
	int rtn = 1;
	// Ԥ����ʽ
	if(abs(a_b) > 0.01)
	{
		double factor_a = (alarm_para[1] + alarm_para[2] + alarm_para[3]/2)*(alarm_para[1] + alarm_para[2] + alarm_para[3]/2)/2;
		double factor_b = (alarm_para[1] + alarm_para[2] + alarm_para[3]/2);

		Sp = alarm_para[0] * (v_a * (alarm_para[1] + alarm_para[2] + alarm_para[3]/2) + v_a*v_a/(2*alarm_para[5])) + alarm_para[4];

		Sr = Sp - v_b*v_b/(2*alarm_para[5]);

		Se1 = Sr + v_b*v_b/(2*a_b);

		Se2 = Sp - (v_b - v_a*a_b/alarm_para[5]) * factor_b	+ a_b*factor_a - v_a * v_b / alarm_para[5] 
		         + a_b*v_a*v_a/(2* alarm_para[5] * alarm_para[5]);

		// Ԥ������
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
		} // �������Ŀ��
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
*  ��ʾԤ�����:ShowAlarmLight();
*  ������Ԥ�������
*  �������ܣ�����Ԥ������ڽ�������ʾԤ�������
*  
*/

void CAsafetyAssistantSystemDlg::ShowAlarmLight(int alarmClass)
{
	// ����ÿ��Ԥ���Ľ����ѡ�񷽰�
	if (alarmClass < alarmLast && isAlarmfalg[alarmLast] && alarmClass == -1)
	{
		// do nothing
	}// ����µľ����ȼ�С��������һ�εľ����ȼ�,������һ�εľ�������ִ��
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
				SetTimer(TIMER2, 500, NULL); // 500ms��ִ��2S���������ۼ�4��
				break;
			case DANGEROUS:
				SetTimer(TIMER2, 200, NULL); // 200ms�����ִ��5S���������ۼ�25��
				break;
			case GODBLESSYOU:
				SetTimer(TIMER2, 50, NULL); // 200ms�����ִ��5S���������ۼ�100��
				break;
			default:
				break;
			}
		}
	}
	else
	{
		alarmLast = alarmClass;  // ������ǰ�ľ����ȼ�

		memset(isAlarmfalg, 0, sizeof(isAlarmfalg));
		memset(time_count, 0, sizeof(time_count));	// ���м���������
		isAlarmfalg[alarmClass] = true;

		KillTimer(TIMER2);
		switch(alarmClass)
		{
		case SAFE:					
			SetTimer(TIMER2, 10, NULL);
			break;
		case THREAT: 
			SetTimer(TIMER2, 500, NULL); // 500ms��ִ��2S���������ۼ�4��
			break;
		case DANGEROUS:
			SetTimer(TIMER2, 200, NULL); // 200ms�����ִ��5S���������ۼ�25��
			break;
		case GODBLESSYOU:
			SetTimer(TIMER2, 50, NULL); // 200ms�����ִ��5S���������ۼ�100��
			break;
		default:
			break;
		}
	} 
}