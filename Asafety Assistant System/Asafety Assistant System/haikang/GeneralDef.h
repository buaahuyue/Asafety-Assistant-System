
#ifndef GENERALDEF_H
#define GENERALDEF_H

#include "HCNetSDK.h"
#include "DecodeCardSdk.h"
#include "DataType.h"
#include "plaympeg4.h"



#define MAX_CHANS      24              //设备支持的最大通道数
#define MAX_DEVICE     10             //最大设备数
#define MAX_OUTPUTS	   16	//16最大的窗口数
#define MAX_PREVIEWS		512	//可预览的最大数

#define MAX_CRUISE_SEQ  32
#define MAX_CRUISE_POINT 32
#define MAX_CRUISE_PRESET 128
#define MAX_CRUISE_TIME 255
#define MAX_CRUISE_SPEED 15

//设备区域配置
#define TREE_ALL_T	 0        //代表根节点
#define DEVICETYPE 	 1		  //代表设备
#define CHANNELTYPE	 2	      //代表通道


#define DEMO_FULL_WIDTH 1024	//demo默认的宽度
#define DEMO_FULL_HEIGHT 768	//demo默认的高度
#define OUTPUT_INTERVAL 4		//每个画面的间隔大小


//定时器时间
#define CYCLE_PREVIEW_TIMER 2    //循环预览定时器
////////////////////////////////////////////////自己添加//////////////////////
#define DEFAULT_BRIGHTNESS	6			//default brightness
#define DEFAULT_CONTRAST	6			//default contrast
#define DEFAULT_SATURATION  6			//default saturation
#define DEFAULT_HUE			6			//default hue
#define DEFAULT_SHARPNESS   6			//default sharpness
#define DEFAULT_DENOISING	6			//default denoising
#define DEFAULT_VOLUME		50			//default volume
#define TREE_ALL			 0	//device list	
#define DEVICE_LOGOUT	     1   //device not log in
#define DEVICE_LOGIN	     2   //devic3e login
#define DEVICE_FORTIFY	     3   //on guard
#define DEVICE_ALARM		 4   //alarm on device
#define DEVICE_FORTIFY_ALARM 5	//onguard & alarm on device

#define CHAN_ORIGINAL		 6   //no preview, no record
#define CHAN_PLAY		 	 7   //preview
#define CHAN_RECORD			 8   //record
#define CHAN_PLAY_RECORD	 9   //preview and record

#define CHAN_ALARM		 	 10   //no preview, no record, only alarm
#define CHAN_PLAY_ALARM			 11   //review, no record, with alarm info
#define CHAN_PLAY_RECORD_ALARM	 12   //preview & record & alarm
#define CHAN_OFF_LINE			 13	 //channel off-line
#define MAX_DEVICES			512	//max device number
#define MAX_OUTPUTS1			512	//16max windownumber
#define MAX_PREVIEWS		512	//max preview number
/////////////////////////////////////////////////////////////////////////////
typedef struct STRU_CHANNEL_INFO
{
	char    chChanName[40];     //通道名称
	int		iChanIndex;			//监控通道号 = 数组索引+startchan
	int		iPicResolution;				//图片分辨率
	int		iPicQuality;				//图片质量
	char	chAccessChanIP[16];     //ip接入通道的ip地址
	BOOL    bEnable;              //是否有效
    NET_DVR_DECODERCFG_V30  struDecodercfg;   //通道的解码器信息
	STRU_CHANNEL_INFO()
	{
		chChanName[0]       ='\0';
		iChanIndex          = -1;
		iPicResolution		= 0;
		iPicQuality			= 2;
		chAccessChanIP[0]	= '\0';	
		bEnable             = FALSE;
	}
}CHANNEL_INFO,*pCHANNEL_INFO;

typedef struct STRU_DEVICE_INFO
{

	LONG    lLoginID;
	int		iDeviceChanNum;		    //设备的通道数
	int		iStartChan;				//设备开始通道号
	int 	iIPChanNum;				//最大数字通道个数
	int		iEnableChanNum;			//有效通道数
	BOOL    bIPRet;                 //是否支持ip接入
	
	CHANNEL_INFO struChanInfo[MAX_CHANNUM_V30];


	STRU_DEVICE_INFO()
	{
		 lLoginID = -1;
		 iDeviceChanNum      =  -1;
		 iStartChan          = 0;
		 iIPChanNum          = 0;
		 iEnableChanNum      = -1;
		 bIPRet              = FALSE;//判断当前通道有效，为1表示有效，为0表示无效
	}	 
}LOCAL_DEVICE_INFO,*pLOCAL_DEVICE_INFO;
///////////////////////////////////如下是自己添加///////////
/*enum DEMO_CHANNEL_TYPE
{
	DEMO_CHANNEL_TYPE_INVALID = -1,
	DEMO_CHANNEL_TYPE_ANALOG  = 0,
	DEMO_CHANNEL_TYPE_IP
};
typedef struct STRU_CHANNEL_INFO1{ 
	int		iDeviceIndex;			//device index
	int		iChanIndex;			//channel index

	DEMO_CHANNEL_TYPE iChanType;
	int     iChannelNO;         //channel NO.       

	char	chChanName[100];	//channel name
	DWORD	dwProtocol;			//network protocol
	
	DWORD dwStreamType; //码流类型，0-主码流，1-子码流，2-码流3，
	DWORD dwLinkMode;//码流连接方式: 0：TCP方式,1：UDP方式,2：多播方式,3 - RTP方式，4-RTP/RTSP,5-RSTP/HTTP

	DWORD	dwPreviewMode;		//预览模式 0-正常模式 1-延时预览
	int		iPicResolution;				//resolution
	int		iPicQuality;				//image quality
	LONG	lRealHandle;          //preview handle
	BOOL    bLocalManualRec;     //manual record
	BOOL    bAlarm;				//alarm
	BOOL	bEnable;			//enable
	DWORD	dwImageType;		//channel status icon
	char	chAccessChanIP[16];//ip addr of IP channel

	STRU_CHANNEL_INFO1 *pNext;
	STRU_CHANNEL_INFO1()
	{
		iDeviceIndex		= -1;
		iChanIndex			= -1;
		iChannelNO          = -1;
		iChanType           = DEMO_CHANNEL_TYPE_INVALID;
		chChanName[0]		= '\0';
		dwProtocol			= 0;

		dwStreamType        = 0;
		dwLinkMode          = 0;

		iPicResolution		= 0;
		iPicQuality			= 2;

		lRealHandle			= -1; 
		bLocalManualRec		= FALSE;
		bAlarm				= FALSE;
		bEnable				= FALSE;
		dwImageType			= CHAN_ORIGINAL;
		chAccessChanIP[0]	= '\0';	 		  
		pNext				= NULL;
		dwPreviewMode		= 0;
	}
}CHANNEL_INFO1,*PCHANNEL_INFO1;
//2011-12-16 被动解码结构 silujie
typedef struct STRU_PASSIVEDECODE_CHANINFO
{
	LONG	lPassiveHandle;
	LONG lRealHandle;
	LONG lUserID;
	LONG lSel;
	HANDLE hFileThread;
	HANDLE hFileHandle;
	HANDLE hExitThread;
	HANDLE hThreadExit;
    CString strRecordFilePath;
	STRU_PASSIVEDECODE_CHANINFO()
	{
		lPassiveHandle = -1;
		lRealHandle = -1;
		lUserID = -1;
		lSel = -1;
		hFileThread = NULL;
		hFileHandle = NULL;
		hExitThread = NULL;
		hThreadExit = NULL;
		strRecordFilePath = "";
	}
}PASSIVEDECODE_CHANINFO, *LPPASSIVEDECODE_CHANINFO;
typedef struct STRU_DEVICE_INFO1{
	int		iDeviceIndex;			//device index
	LONG	lLoginID;				//ID
	DWORD	dwDevSoftVer;			//erserved
	
	char	chLocalNodeName[100];	//local device node
	char	chDeviceName[100];		//device name
	char	chDeviceIP[130];			//device IP: IP,pppoe address, or network gate address, etc
	char    chDeviceIPInFileName[130];  //if chDeviceIP includes ':'(IPV6),change it to '.', for '.'is not allowed in window's file name
	//char	chDevNetCard1IP[33];	//IP
	char	chLoginUserName[NAME_LEN];	//login user name
	char	chLoginPwd[NAME_LEN];	//password
	char	chDeviceMultiIP[130];	//multi-cast group address
	//char	chIPServerIP[32];		//IP
	char	chSerialNumber[50];		//SN
	int		iDeviceChanNum;		    //channel numder  (analog + ip)
	int		iStartChan;				//start channel number
	int		iDeviceType;			//device type
	int 	iDiskNum;				//HD number
	LONG 	lDevicePort;			//port number
	int		iAlarmInNum;			//alarm in number
	int		iAlarmOutNum;			//alarm out number
	int 	iAudioNum;				//voice talk number
	int		iAnalogChanNum;			//analog channel number
	int 	iIPChanNum;				//max IP channel number
	int     iGroupNO;               //IP Group NO.
	
	BOOL	bCycle;					//if this device is on cycle recording
	BOOL	bPlayDevice;			//will be added later
	BOOL	bVoiceTalk;				//on voice talkor not
	//LONG    lAudioHandle[MAX_AUDIO_V40]; //voicebroadcast handle
	//BOOL	bCheckBroadcast[MAX_AUDIO_V40]; //Add to broad cast group
	LONG	lFortifyHandle;			//on guard handle
	BOOL    bAlarm;					//whether there is alarm
	int	    iDeviceLoginType;		//register mode  0 - fix IP   1- IPSERVER mode    2 -  domain name 
	DWORD	dwImageType;			//device status icon
	BOOL	bIPRet;					//support IP conection
	int		iEnableChanNum;			//valid channel number
	BOOL	bDVRLocalAllRec;		//local recording
	//LONG	lVoiceCom[MAX_AUDIO_V40];	//voice transmit
	LONG	lFirstEnableChanIndex;			//first enabled channel index
	LONG	lTranHandle;				//232 transparent channel handle
    BYTE byZeroChanNum;		//Zero channel number
    BYTE byMainProto;			//main stream protocol type 0-Private, 1-rtp/tcp, 2-rtp/rtsp
    BYTE bySubProto;				//sub stream protocol type -Private, 1-rtsp
    BYTE bySupport;             //ability
	STRU_CHANNEL_INFO1 struChanInfo[MAX_CHANNUM_V30*2]; //channel structure
	NET_DVR_IPPARACFG_V40 struIPParaCfgV40;				//IP channel parameters
	NET_DVR_IPALARMINCFG struAlarmInCfg;
	NET_DVR_IPALARMOUTCFG struAlarmOutCfg;
	STRU_DEVICE_INFO *pNext;
	CHANNEL_INFO struZeroChan[16];	
	char sSecretKey[16];
	int iAudioEncType;
	PASSIVEDECODE_CHANINFO struPassiveDecode[32];
	BYTE bySupport1;	//能力，位与结果为0表示不支持，1表示支持
	// bySupport1 & 0x1, 表示是否支持snmp v30
	//bySupport1 & 0x2, 支持区分回放和下载	
	//bySupport1 & 0x4, 是否支持布防优先级	
	//bySupport1 & 0x8, 智能设备是否支持布防时间段扩展
	//bySupport1 & 0x10, 表示是否支持多磁盘数（超过33个）
	//bySupport1 & 0x40 表示是否支持延迟预览
	STRU_DEVICE_INFO1()
	{	
		iGroupNO = -1;
		iDeviceIndex		= -1;
		lLoginID            = -1;	
		dwDevSoftVer		= 0;
		chLocalNodeName[0]  = '\0';	
		chDeviceName[0]     = '\0';	
		chDeviceIP[0]	    = '\0';	
		chDeviceIPInFileName[0] = '\0';	
		//chDevNetCard1IP[0]	= '\0';
		chLoginUserName[0]= '\0';	
		chLoginPwd[0] = '\0';	
		chDeviceMultiIP[0]  = '\0';		
		chSerialNumber[0]   = '\0';	 
		iDeviceChanNum      = -1;
		iStartChan			= 0;
		iDeviceType         = 0;	
		iDiskNum            = 0;				
		lDevicePort         = 8000;	
		iAlarmInNum			= 0;	
		iAlarmOutNum        = 0;
		iAnalogChanNum		= 0;
		iIPChanNum			= 0;
		
		bCycle              = FALSE;
		bPlayDevice			= FALSE;
		bVoiceTalk			= FALSE;
	//	bCheckBroadcast[0]     = FALSE;	
		//bCheckBroadcast[1]     = FALSE;	
		lFortifyHandle      = -1;	
		bAlarm				= FALSE;
		iDeviceLoginType    = 0;	
		dwImageType			= DEVICE_LOGOUT;
		bIPRet				= FALSE;
		pNext = NULL;
		//lVoiceCom[0] = -1;
		//lVoiceCom[1] = -1;
		lFirstEnableChanIndex = 0;
		lTranHandle = -1;
		byZeroChanNum = 0;
      //  lAudioHandle[0] = -1;
       // lAudioHandle[1] = -1;
		memset(&struIPParaCfgV40,0, sizeof(struIPParaCfgV40));
		memset(&struAlarmOutCfg,0, sizeof(NET_DVR_IPALARMOUTCFG));
		memset(&struAlarmInCfg,0, sizeof(NET_DVR_IPALARMINCFG));
		memcpy(sSecretKey, "StreamNotEncrypt", 16);
		iAudioEncType = 0;   
		bySupport1 = 0;
	}	
}LOCAL_DEVICE_INFO1, *PLOCAL_DEVICE_INFO1;
typedef struct STRU_VIDEO_EFFECT{
	UINT	m_iBrightness;				//1-10
	UINT	m_iContrast;				//1-10
	UINT	m_iSaturation;				//1-10
	UINT	m_iHue;						//1-10
	UINT	m_iSharpness;
	UINT	m_iDenoising;
	STRU_VIDEO_EFFECT()
	{
		m_iBrightness = DEFAULT_BRIGHTNESS;
		m_iContrast = DEFAULT_CONTRAST;	
		m_iSaturation = DEFAULT_SATURATION;
		m_iHue = DEFAULT_HUE;
		m_iSharpness = DEFAULT_SHARPNESS;
		m_iDenoising = DEFAULT_DENOISING;
	}
}VIDEO_INFO, *PVIDEO_INFO;
typedef struct STRU_LOCAL_RECORD_TIME{
	WORD	iStartHour;
	WORD	iStartMinute;
	WORD	iStopHour;
	WORD	iStopMinute;
	WORD	iStartTime;
	WORD	iStopTime;
}LOCAL_RECORD_TIME,*PLOCAL_RECORD_TIME;
typedef struct STRU_LOCAL_PARAM
{
    BOOL        bReconnect;                 //reconnect
	BOOL		bCyclePlay;				    //cycle play
	int			iCycleTime;				    //cycle time, default 20
	BOOL		bUseCard;					//hadrware decode
	BOOL		bNTSC;						//hardware decode mode,FALSE,PAL;TRUE,NTSC;default as pal

	BOOL		bAutoRecord;				//auto record;
	BOOL		bCycleRecord;				//cycle record
	int			iStartRecordDriver;		    //client record starting HD dirve
	int			iEndRecordDriver;			//client record stop HD drive
	int			iRecFileSize;				//record file size
	int			iRecordFileInterval;		//record file interval
	char		chDownLoadPath[100];		//remote file download directory
	char		chPictureSavePath[100];		//image capture directory
	char		chRemoteCfgSavePath[100];	//remote config file saving directory
	char		chClientRecordPath[100];		//client record path

	BOOL		bAutoCheckDeviceTime;		//check time with device
	LONG		lCheckDeviceTime;			//check time interval

	int			iAlarmDelayTime ;			//alarm delay time

	int         iAlarmListenPort;

	BOOL		bAutoSaveLog;				//auto save local log info
	BOOL		bAlarmInfo;					//display alarm info on log list
	BOOL		bSuccLog;				//display log access on log list
	BOOL		bFailLog;					//display filaure operation on log list
	BOOL		bAllDiskFull;				//HD full

	//preview
	BOOL		bPlaying;					//on playing
	BOOL		bCycling;					//cycle playing
	BOOL		bPaused;					//cycle pause
	BOOL		bNextPage;				    //next page
	BOOL		bFrontPage;				    //previous page
	BOOL		bEnlarged;				    //enlarge image
	BOOL		bFullScreen;				//full screen
	BOOL		bMultiScreen;				//multi-split-window full screen
	BOOL		bNoDecode;					//soft decode or not
	BOOL		bPreviewBlock;				//preview block or not

	VIDEO_INFO	struVideoInfo[MAX_OUTPUTS];	//video parameter
	int			iVolume;					//volume
	BOOL		bBroadCast;					//voice broadcast
	char		chIPServerIP[128];
	BOOL		bOutputDebugString;

	LOCAL_RECORD_TIME struLocalRecordTime[7][4];
	DWORD		dwBFrameNum;//throw B frame number
	DWORD   nLogLevel;
    BOOL    bCycleWriteLog;
	DWORD   nTimeout;
	STRU_LOCAL_PARAM()
	{
        bReconnect          = TRUE;
		bCyclePlay			= FALSE;
		iCycleTime			= 20;
		bUseCard		    = FALSE;
		bNTSC				= FALSE;
		bAutoRecord			= FALSE;
		bCycleRecord		= FALSE;
		iStartRecordDriver = 0;
		iEndRecordDriver   = 0;
		iRecFileSize	   = 1;
		iRecordFileInterval = 60;
		sprintf_s(chDownLoadPath, "%s","C:\\DownLoad");
		sprintf_s(chPictureSavePath, "%s", "C:\\Picture");
		sprintf_s(chRemoteCfgSavePath ,"%s", "C:\\SaveRemoteCfgFile");
		sprintf_s(chClientRecordPath ,"%s", "C:\\mpeg4record\\2008-04-30");
		sprintf_s(chIPServerIP ,"%s", "172.7.28.123");

		bAutoCheckDeviceTime	=  FALSE;
		lCheckDeviceTime	= 0;

		iAlarmDelayTime		= 10;
		iAlarmListenPort	= 7200;

		bAutoSaveLog		= TRUE;
		bAlarmInfo			= TRUE;
		bSuccLog			= TRUE;
		bFailLog			= TRUE;

		bAllDiskFull		= FALSE;
		bPlaying			= FALSE;
		bCycling			= FALSE;
		bPaused				= FALSE;
		bNextPage			= FALSE;
		bFrontPage			= FALSE;
		bEnlarged			= FALSE;
		bFullScreen			= FALSE;
		bMultiScreen		= FALSE;
		iVolume				= DEFAULT_VOLUME;
		bBroadCast			= FALSE;
		bNoDecode			= FALSE;
		bPreviewBlock		= TRUE;
		bOutputDebugString	= FALSE;
		dwBFrameNum			= 0;
		nLogLevel				= 3;
        bCycleWriteLog      = FALSE;
		nTimeout				= 5000;
		memset(&struLocalRecordTime[0][0], 0, sizeof(LOCAL_RECORD_TIME)*7*4);
	}
}LOCAL_PARAM, *PLOCAL_PARAM;*/

#endif