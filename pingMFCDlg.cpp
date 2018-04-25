// pingMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pingMFC.h"
#include "pingMFCDlg.h"
#include <WINSOCK2.H>     
#include <STDIO.H>     
#include <STDLIB.H> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma pack(4) 

#define ICMP_ECHO 8 
#define ICMP_ECHOREPLY 0 

#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header) 

/* The IP header */ 
typedef struct iphdr { 
	unsigned int h_len:4; // length of the header 
	unsigned int version:4; // Version of IP 
	unsigned char tos; // Type of service 
	unsigned short total_len; // total length of the packet 
	unsigned short ident; // unique identifier 
	unsigned short frag_and_flags; // flags 
	unsigned char ttl; 
	unsigned char proto; // protocol (TCP, UDP etc) 
	unsigned short checksum; // IP checksum 
	
	unsigned int sourceIP; 
	unsigned int destIP; 
	
}IpHeader; 

// 
// ICMP header 
// 
typedef struct icmphdr { 
	BYTE i_type; 
	BYTE i_code; /* type sub code */ 
	USHORT i_cksum; 
	USHORT i_id; 
	USHORT i_seq; 
	/* This is not the std header, but we reserve space for time */ 
	ULONG timestamp; 
}IcmpHeader; 

#define STATUS_FAILED 0xFFFF 
#define DEF_PACKET_SIZE    32 
#define DEF_PACKET_NUMBER  4    /* 发送数据报的个数 */ 
#define MAX_PACKET 1024 

#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s)) 
#define xfree(p) HeapFree (GetProcessHeap(),0,(p)) 

void fill_icmp_data(char *, int); 
USHORT checksum(USHORT *, int); 
int decode_resp(char *,int ,struct sockaddr_in *); 
  
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPingMFCDlg dialog

CPingMFCDlg::CPingMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPingMFCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPingMFCDlg)
	m_ip = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPingMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPingMFCDlg)
	DDX_Text(pDX, IDC_EDITip, m_ip);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPingMFCDlg, CDialog)
	//{{AFX_MSG_MAP(CPingMFCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ping, Onping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPingMFCDlg message handlers

BOOL CPingMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPingMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPingMFCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPingMFCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CPingMFCDlg::Usage(char *progname)
{
//	fprintf(stderr,"Usage:\n"); 
//	fprintf(stderr,"%s [number of packets] [data_size]\n",progname); 
//	fprintf(stderr,"datasize can be up to 1Kb\n"); 
    ExitProcess(STATUS_FAILED); 
}

int CPingMFCDlg::decode_resp(char *buf, int bytes, sockaddr_in *from)
{
	IpHeader *iphdr; 
	IcmpHeader *icmphdr; 
	unsigned short iphdrlen; 
	
	iphdr = (IpHeader *)buf; 
	
	iphdrlen = (iphdr->h_len) * 4 ; // number of 32-bit words *4 = bytes 
	
	if (bytes < iphdrlen + ICMP_MIN) { 
//		printf("Too few bytes from %s\n",inet_ntoa(from->sin_addr)); 
	} 
	
	icmphdr = (IcmpHeader*)(buf + iphdrlen); 
	
	if (icmphdr->i_type != ICMP_ECHOREPLY) { 
//		fprintf(stderr,"non-echo type %d recvd\n",icmphdr->i_type); 
		return 1; 
	} 
	if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) { 
//		fprintf(stderr,"someone else's packet!\n"); 
		return 1; 
	} 
//	printf("%d bytes from %s:",bytes, inet_ntoa(from->sin_addr)); 
//	printf(" icmp_seq = %d. ",icmphdr->i_seq); 
//	printf(" time: %d ms ",GetTickCount()-icmphdr->timestamp); 
//	printf("\n"); 
//	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDITstate);
//	pEdit->SetWindowText("已ping通");
     return 0; 
}

USHORT CPingMFCDlg::checksum(USHORT *buffer, int size)
{
	unsigned long cksum=0; 
	
	while(size >1) { 
		cksum+=*buffer++; 
		size -=sizeof(USHORT); 
	} 
	
	if(size) { 
		cksum += *(UCHAR*)buffer; 
	} 
	
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
     return (USHORT)(~cksum); 
}

void CPingMFCDlg::fill_icmp_data(char *icmp_data, int datasize)
{
	IcmpHeader *icmp_hdr; 
	char *datapart; 
	
	icmp_hdr = (IcmpHeader*)icmp_data; 
	
	icmp_hdr->i_type = ICMP_ECHO; 
	icmp_hdr->i_code = 0; 
	icmp_hdr->i_id = (USHORT)GetCurrentProcessId(); 
	icmp_hdr->i_cksum = 0; 
	icmp_hdr->i_seq = 0; 
	
	datapart = icmp_data + sizeof(IcmpHeader); 
	// 
	// Place some junk in the buffer. 
	// 
     memset(datapart,'E', datasize - sizeof(IcmpHeader)); 
}

void CPingMFCDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	MessageBox("sddd");
	CDialog::OnCancel();
}

void CPingMFCDlg::Onping() 
{
	int argc;
	char **argv=(char**)malloc(sizeof(char**));
	argc=2;
	CString s;
	GetDlgItem(IDC_EDITip)->GetWindowText(s);
	argv[1]=(LPSTR)(LPCTSTR)(s); 
      WSADATA wsaData; 
      SOCKET sockRaw; 
      struct sockaddr_in dest,from; 
      struct hostent * hp; 
      int bread,datasize,times; 
      int fromlen = sizeof(from); 
      int timeout = 1000; 
      int statistic = 0;  /* 用于统计结果 */  
      char *dest_ip; 
      char *icmp_data; 
      char *recvbuf; 
      unsigned int addr=0; 
      USHORT seq_no = 0; 
  
      if (WSAStartup(MAKEWORD(2,1),&wsaData) != 0){ 
          fprintf(stderr,"WSAStartup failed: %d\n",GetLastError()); 
          ExitProcess(STATUS_FAILED); 
      } 
  
      if (argc <2 ) { 
          Usage(argv[0]); 
      } 
      sockRaw = WSASocket(AF_INET,SOCK_RAW,IPPROTO_ICMP,NULL, 0,WSA_FLAG_OVERLAPPED); 
  
      // 
     //注：为了使用发送接收超时设置(即设置SO_RCVTIMEO, SO_SNDTIMEO)， 
     //    必须将标志位设为WSA_FLAG_OVERLAPPED ! 
     // 
 
     if (sockRaw == INVALID_SOCKET) { 
  //       fprintf(stderr,"WSASocket() failed: %d\n",WSAGetLastError()); 
         ExitProcess(STATUS_FAILED); 
     } 
     bread = setsockopt(sockRaw,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout, 
         sizeof(timeout)); 
     if(bread == SOCKET_ERROR) { 
   //      fprintf(stderr,"failed to set recv timeout: %d\n",WSAGetLastError()); 
         ExitProcess(STATUS_FAILED); 
     } 
     timeout = 1000; 
     bread = setsockopt(sockRaw,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout, 
         sizeof(timeout)); 
     if(bread == SOCKET_ERROR) { 
    //     fprintf(stderr,"failed to set send timeout: %d\n",WSAGetLastError()); 
         ExitProcess(STATUS_FAILED); 
     } 
     memset(&dest,0,sizeof(dest)); 
 
     hp = gethostbyname(argv[1]); 
 
     if (!hp){ 
         addr = inet_addr(argv[1]); 
     } 
     if ((!hp) && (addr == INADDR_NONE) ) { 
      //   fprintf(stderr,"Unable to resolve %s\n",argv[1]); 
         ExitProcess(STATUS_FAILED); 
     } 
 
     if (hp != NULL) 
         memcpy(&(dest.sin_addr),hp->h_addr,hp->h_length); 
     else 
         dest.sin_addr.s_addr = addr; 
 
     if (hp) 
         dest.sin_family = hp->h_addrtype; 
     else 
         dest.sin_family = AF_INET; 
 
     dest_ip = inet_ntoa(dest.sin_addr); 
 
     // 
     //  atoi函数原型是: int atoi( const char *string ); 
     //  The return value is 0 if the input cannot be converted to an integer ! 
     // 
     if(argc>2) 
     { 
         times=atoi(argv[2]); 
         if(times == 0) 
             times=DEF_PACKET_NUMBER; 
     } 
     else 
         times=DEF_PACKET_NUMBER; 
 
     if (argc >3) 
     { 
         datasize = atoi(argv[3]); 
         if (datasize == 0) 
             datasize = DEF_PACKET_SIZE; 
         if (datasize >1024)   /* 用户给出的数据包大小太大 */ 
         { 
        //     fprintf(stderr,"WARNING : data_size is too large !\n"); 
             datasize = DEF_PACKET_SIZE; 
         } 
     } 
     else 
         datasize = DEF_PACKET_SIZE; 
 
     datasize += sizeof(IcmpHeader); 
 
     icmp_data = (char*)xmalloc(MAX_PACKET); 
     recvbuf = (char*)xmalloc(MAX_PACKET); 
 
     if (!icmp_data) { 
      //   fprintf(stderr,"HeapAlloc failed %d\n",GetLastError()); 
         ExitProcess(STATUS_FAILED); 
     } 
 
 
     memset(icmp_data,0,MAX_PACKET); 
     fill_icmp_data(icmp_data,datasize); 
 
     // 
     //显示提示信息 
     // 
    // fprintf(stdout,"\nPinging %s .\n\n",dest_ip); 
	
	 //MessageBox("pinging");
 
 
     for(int i=0;i<times;++i){ 
         int bwrote; 
 
         ((IcmpHeader*)icmp_data)->i_cksum = 0; 
         ((IcmpHeader*)icmp_data)->timestamp = GetTickCount(); 
 
         ((IcmpHeader*)icmp_data)->i_seq = seq_no++; 
         ((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data,datasize); 
 
         bwrote = sendto(sockRaw,icmp_data,datasize,0,(struct sockaddr*)&dest,sizeof(dest)); 
         if (bwrote == SOCKET_ERROR){ 
             if (WSAGetLastError() == WSAETIMEDOUT) { 
          //       printf("Request timed out.\n"); 
                 continue; 
             } 
          //   fprintf(stderr,"sendto failed: %d\n",WSAGetLastError()); 
             ExitProcess(STATUS_FAILED); 
         } 
         if (bwrote < datasize ) { 
         //    fprintf(stdout,"Wrote %d bytes\n",bwrote); 
         } 
         bread = recvfrom(sockRaw,recvbuf,MAX_PACKET,0,(struct sockaddr*)&from,&fromlen); 
         if (bread == SOCKET_ERROR){ 
             if (WSAGetLastError() == WSAETIMEDOUT) { 
          //       printf("Request timed out.\n"); 
                 continue; 
             } 
          //   fprintf(stderr,"recvfrom failed: %d\n",WSAGetLastError()); 
             ExitProcess(STATUS_FAILED); 
         } 
         if(!decode_resp(recvbuf,bread,&from)) 
		 { 
			 
			statistic++; /* 成功接收的数目++ */} 
		 else
		 {
			 
// 			 CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDITstate);
// 			pEdit->SetWindowText("无法ping通..");
		 }
		
         Sleep(1000); 
 
     } 
 
     /* 
     Display the statistic result 
     */ 
   //  fprintf(stdout,"\nPing statistics for %s \n",dest_ip); 
   //  fprintf(stdout,"    Packets: Sent = %d,Received = %d, Lost = %d (%2.0f%% loss)\n",times, 
   //      statistic,(times-statistic),(float)(times-statistic)/times*100); 
 
 
     WSACleanup(); 
	 if (statistic>0)
	 {
		 CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATE);
		 // CEdit* pEdit=(CEdit*)GetDlgItem(IDC_STATE);
		 pStatic->SetWindowText("ping通");
	 }
	 else
	 {
		 
		 CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATE);
		 // CEdit* pEdit=(CEdit*)GetDlgItem(IDC_STATE);
		 pStatic->SetWindowText("ping不通");
		 }
}
