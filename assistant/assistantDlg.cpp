
// assistantDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "assistant.h"
#include "assistantDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MSG_MOUSE_L_DOWN   WM_USER + 305
#define WM_MSG_MOUSE_L_UP     WM_USER + 306
#define WM_MSG_MOUSE_R_DOWN   WM_USER + 307
#define WM_MSG_MOUSE_R_UP     WM_USER + 308
#define WM_MSG_MOUSE_MOVE     WM_USER + 309
#define WM_MSG_KEYDOWN        WM_USER + 310


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CassistantDlg 对话框




CassistantDlg::CassistantDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CassistantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_debug_enable = FALSE;
	m_game_enable = FALSE;
	m_record_enable = FALSE;
	m_script_enable = FALSE;
	m_capture_enable = FALSE;

	m_multiple = FALSE;

	m_sc = NULL;
	m_rec = NULL;
	m_cfg = new CConfig();
	m_pm = NULL;
	m_algm = NULL;
	m_canvas = NULL;

	m_hcap = NULL;
	m_hInstDll = NULL;
}

CassistantDlg::~CassistantDlg(void)
{
	if(m_cfg != NULL){
		delete m_cfg;
		m_cfg = NULL;
	}

	if(m_pm != NULL){
		delete m_pm;
		m_pm = NULL;
	}
}

void CassistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CassistantDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	/***********************   用户消息处理  *************************/
	ON_BN_CLICKED(IDC_START, &CassistantDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_PARAM, &CassistantDlg::OnBnClickedParam)
	ON_BN_CLICKED(IDC_CONFIG, &CassistantDlg::OnBnClickedConfig)
	ON_BN_CLICKED(IDC_CAPTURE, &CassistantDlg::OnBnClickedCapture)
	ON_BN_CLICKED(IDC_DEBUG, &CassistantDlg::OnBnClickedDebug)
	ON_BN_CLICKED(IDC_RECORD, &CassistantDlg::OnBnClickedRecord)
	ON_BN_CLICKED(IDC_BROWSER, &CassistantDlg::OnBnClickedBrowser)
	ON_BN_CLICKED(IDC_RUN, &CassistantDlg::OnBnClickedRun)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CassistantDlg::OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
	ON_WM_TIMER()

	/***********************   系统消息处理  *************************/
	ON_MESSAGE(WM_ALGM_OPENCV_WIN_NEW, &CassistantDlg::OnAlgmOpencvWinNew)
	ON_MESSAGE(WM_ALGM_OPENCV_WIN_DESTROY, &CassistantDlg::OnAlgmOpencvWinDestroy)
	ON_MESSAGE(WM_MSG_MOUSE_L_DOWN, &CassistantDlg::OnMsgMouseLDown)
	ON_MESSAGE(WM_MSG_MOUSE_L_UP, &CassistantDlg::OnMsgMouseLUp)
	ON_MESSAGE(WM_MSG_MOUSE_R_DOWN, &CassistantDlg::OnMsgMouseRDown)
	ON_MESSAGE(WM_MSG_MOUSE_R_UP, &CassistantDlg::OnMsgMouseRUp)
	ON_MESSAGE(WM_MSG_MOUSE_MOVE, &CassistantDlg::OnMsgMouseMove)
	ON_MESSAGE(WM_MSG_KEYDOWN, &CassistantDlg::OnMsgKeydown)
	ON_MESSAGE(WM_CHILD_EXIT_CANVAS, &CassistantDlg::OnChildExitCanvas)

	/***********************   内部消息处理  *************************/
	ON_MESSAGE(WM_SC_BUTTON_L_DOWN, &CassistantDlg::OnScButtonLDown)
	ON_MESSAGE(WM_SC_BUTTON_L_UP, &CassistantDlg::OnScButtonLUp)
	ON_MESSAGE(WM_SC_BUTTON_R_DOWN, &CassistantDlg::OnScButtonRDown)
	ON_MESSAGE(WM_SC_BUTTON_R_UP, &CassistantDlg::OnScButtonRUp)
	ON_MESSAGE(WM_SC_KEYDOWN, &CassistantDlg::OnScKeydown)
	ON_MESSAGE(WM_SC_END, &CassistantDlg::OnScEventEnd)
END_MESSAGE_MAP()


// CassistantDlg 消息处理程序

BOOL CassistantDlg::OnInitDialog()
{
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

	// TODO: 在此添加额外的初始化代码
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(this->GetSafeHwnd(),GWL_STYLE)& (WS_SYSMENU | WS_CLIPCHILDREN| WS_CLIPSIBLINGS));//设置成新的样式    
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(this->GetSafeHwnd(),GWL_STYLE)|WS_CAPTION);
	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

	skinppLoadSkin("MSN Messenger.ssk");

	((CComboBox *) GetDlgItem(IDC_COMBO1))->SetCurSel(0);

	((CSliderCtrl*)GetDlgItem(IDC_OPACITY))->SetRange(1, 255);
	((CSliderCtrl*)GetDlgItem(IDC_OPACITY))->SetTicFreq(10);
	((CSliderCtrl*)GetDlgItem(IDC_OPACITY))->SetPos(180);
	((CSliderCtrl*)GetDlgItem(IDC_OPACITY))->EnableWindow(FALSE);

	((CSliderCtrl*)GetDlgItem(IDC_SPEED))->SetRange(-3, 3);
	((CSliderCtrl*)GetDlgItem(IDC_SPEED))->SetTicFreq(1);
	((CSliderCtrl*)GetDlgItem(IDC_SPEED))->SetPos(0);

	CString AlgorithmName;
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(AlgorithmName);
	m_pm = new CParam(NULL, AlgorithmName);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CassistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CassistantDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		PostMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CassistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/********************************   控件消息响应   ************************************/

void CassistantDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

	CString AlgorithmName;

	if(m_pm != NULL){
		delete m_pm;
		m_pm = NULL;
	}
	
	int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO1))->GetCurSel();
	((CComboBox *)GetDlgItem(IDC_COMBO1))->GetLBText(nSel, AlgorithmName);
	m_pm = new CParam(NULL, AlgorithmName);
}

void CassistantDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码

	m_game_enable = !m_game_enable;
	if(m_game_enable){
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_PARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_START)->SetWindowText("停止");

		m_algm = new Cfqjs(this, m_hcap, m_canvas, m_cfg->getMouseConfig(0)->pos);
	}else{
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_PARAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_START)->SetWindowText("开始");

		delete m_algm;
		m_algm = NULL;
	}
}

void CassistantDlg::OnBnClickedParam()
{
	if(m_pm != NULL){
		m_pm->DoModal();
	}
}

void CassistantDlg::OnBnClickedConfig()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_cfg != NULL){
		m_cfg->DoModal();
	}
}

void CassistantDlg::OnBnClickedCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	m_capture_enable = TRUE;
	GetDlgItem(IDC_CAPTURE)->EnableWindow(FALSE);
	LoadDll(&m_hInstDll);
	SetTimer(1, 500, NULL);  
}

void CassistantDlg::OnBnClickedDebug()
{
	m_debug_enable = !m_debug_enable;

	if(m_debug_enable){

		if(m_canvas == NULL){
			m_canvas = new CcanvasDlg(this);
			m_canvas->Create(IDD_CANVAS_DIALOG, this);
			m_canvas->ShowWindow(SW_SHOW);
		}

		GetDlgItem(IDC_OPACITY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DEBUG)->SetWindowText("关闭调试");
	}
	else{

		if(m_canvas != NULL){
			::SendMessage(m_canvas->m_hWnd, WM_CLOSE, 0, 0);
			delete m_canvas;
			m_canvas = NULL;
		}

		GetDlgItem(IDC_OPACITY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEBUG)->SetWindowText("打开调试");
	}
}

afx_msg LRESULT CassistantDlg::OnChildExitCanvas(WPARAM wParam, LPARAM lParam)
{
	if(m_debug_enable){
		m_debug_enable = FALSE;
		if(m_canvas != NULL){
			delete m_canvas;
			m_canvas = NULL;
		}

		GetDlgItem(IDC_OPACITY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEBUG)->SetWindowText("打开调试");
	}

	return 0;
}

void CassistantDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(pScrollBar->GetDlgCtrlID() == IDC_OPACITY && m_canvas != NULL){
		int value = ((CSliderCtrl*)GetDlgItem(IDC_OPACITY))->GetPos();
		m_canvas->SetOpacity(value);
	}
	else if(pScrollBar->GetDlgCtrlID() == IDC_SPEED){
		int value = ((CSliderCtrl*)GetDlgItem(IDC_SPEED))->GetPos();
		if(m_sc != NULL){
			m_sc->setEXEspeed(value + ((CSliderCtrl*)GetDlgItem(IDC_SPEED))->GetRangeMax());
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CassistantDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent) {  
		HWND DeskHwnd = ::GetDesktopWindow(); 
		HDC DeskDC = ::GetWindowDC(DeskHwnd); 
		int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);  

		POINT MousePoint;  
		::GetCursorPos(&MousePoint); 
		HWND CaptureWindow = ::WindowFromPoint(MousePoint);
		RECT CaptureRect;
		::GetWindowRect(CaptureWindow, &CaptureRect);
		if( CaptureRect.left < 0 ) CaptureRect.left = 0;
		if (CaptureRect.top < 0 ) CaptureRect.top = 0;

		HPEN newPen = ::CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);
		::Rectangle(DeskDC, CaptureRect.left, CaptureRect.top, CaptureRect.right, CaptureRect.bottom);
		::Sleep(100);
		::Rectangle(DeskDC, CaptureRect.left, CaptureRect.top, CaptureRect.right, CaptureRect.bottom);

		::SetROP2(DeskDC, oldRop2);
		::SelectObject(DeskDC, oldPen);
		::DeleteObject(newPen);  
		::ReleaseDC(DeskHwnd, DeskDC);
	}

	CDialogEx::OnTimer(nIDEvent);
}

/************************************   algorithm   *****************************************/

afx_msg LRESULT CassistantDlg::OnAlgmOpencvWinNew(WPARAM wParam, LPARAM lParam)
{
	char *OpencvWindowTitle = (char *)wParam;
	cvNamedWindow(OpencvWindowTitle, CV_WINDOW_AUTOSIZE);

	return 0;
}


afx_msg LRESULT CassistantDlg::OnAlgmOpencvWinDestroy(WPARAM wParam, LPARAM lParam)
{
	char *OpencvWindowTitle = (char *)wParam;
	cvDestroyWindow(OpencvWindowTitle);

	return 0;
}

/************************************   Script   *****************************************/

void CassistantDlg::OnBnClickedBrowser()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;     //是否打开 
	CString defaultDir = "C:\\Users\\Administrator\\Desktop";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "所有文件 (*.*)|*.*||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);  
	INT_PTR result = openFileDlg.DoModal();  
	if(result == IDOK) {  
		CString FileName = openFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT1)->SetWindowText(FileName);
	}
}

void CassistantDlg::OnBnClickedRun()
{
	// TODO: 在此添加控件通知处理程序代码

	m_script_enable = !m_script_enable;

	if(m_script_enable){

		CString path;
		GetDlgItem(IDC_EDIT1)->GetWindowText(path);

		int value;
		value = ((CSliderCtrl*)GetDlgItem(IDC_SPEED))->GetPos();

		if(m_sc != NULL){
			delete m_sc;
			m_sc = NULL;
		}

		m_sc = new CScript(this, path, value + ((CSliderCtrl*)GetDlgItem(IDC_SPEED))->GetRangeMax());
		m_sc->StartScript();

		m_multiple = TermIsMultiple();

		GetDlgItem(IDC_RUN)->SetWindowText("停止");
	}
	else{
		m_sc->StopScript();
		m_multiple = FALSE;

		GetDlgItem(IDC_RUN)->SetWindowText("开始");
	}

}

afx_msg LRESULT CassistantDlg::OnScButtonLDown(WPARAM wParam, LPARAM lParam)
{
	if(m_sc != NULL){
		int count = m_sc->getEXEcount();
		char status[128];
		sprintf(status, "%d 次", count);
		GetDlgItem(IDC_COUNT)->SetWindowText(status);
		
		sprintf(status, "%s,%d,%d", desc[BUTTON_L_DOWN], (int)wParam, (int)lParam);
		GetDlgItem(IDC_EDIT4)->SetWindowText(status);

#ifdef _SCRIPT_READ_PROGRESS_
		int percent = m_sc->getEXEpercent();
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(percent);
#endif

		MakeMouseClickEx(BUTTON_L_DOWN, (int)wParam, (int)lParam);
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnScButtonLUp(WPARAM wParam, LPARAM lParam)
{
	if(m_sc != NULL){
		int count = m_sc->getEXEcount();
		char status[128];
		sprintf(status, "%d 次", count);
		GetDlgItem(IDC_COUNT)->SetWindowText(status);

		sprintf(status, "%s,%d,%d", desc[BUTTON_L_UP], (int)wParam, (int)lParam);
		GetDlgItem(IDC_EDIT4)->SetWindowText(status);

#ifdef _SCRIPT_READ_PROGRESS_
		int percent = m_sc->getEXEpercent();
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(percent);
#endif

		MakeMouseClickEx(BUTTON_L_UP, (int)wParam, (int)lParam);
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnScButtonRDown(WPARAM wParam, LPARAM lParam)
{
	if(m_sc != NULL){
		int count = m_sc->getEXEcount();
		char status[128];
		sprintf(status, "%d 次", count);
		GetDlgItem(IDC_COUNT)->SetWindowText(status);

		sprintf(status, "%s,%d,%d", desc[BUTTON_R_DOWN], (int)wParam, (int)lParam);
		GetDlgItem(IDC_EDIT4)->SetWindowText(status);

#ifdef _SCRIPT_READ_PROGRESS_
		int percent = m_sc->getEXEpercent();
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(percent);
#endif

		MakeMouseClickEx(BUTTON_R_DOWN, (int)wParam, (int)lParam);
	}

	return 0;
}


afx_msg LRESULT CassistantDlg::OnScButtonRUp(WPARAM wParam, LPARAM lParam)
{
	if(m_sc != NULL){
		int count = m_sc->getEXEcount();
		char status[128];
		sprintf(status, "%d 次", count);
		GetDlgItem(IDC_COUNT)->SetWindowText(status);

		sprintf(status, "%s,%d,%d", desc[BUTTON_R_UP], (int)wParam, (int)lParam);
		GetDlgItem(IDC_EDIT4)->SetWindowText(status);

#ifdef _SCRIPT_READ_PROGRESS_
		int percent = m_sc->getEXEpercent();
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(percent);
#endif

		MakeMouseClickEx(BUTTON_R_UP, (int)wParam, (int)lParam);
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnScKeydown(WPARAM wParam, LPARAM lParam)
{
	if(m_sc != NULL){
		int count = m_sc->getEXEcount();
		char status[128];
		sprintf(status, "%d 次", count);
		GetDlgItem(IDC_COUNT)->SetWindowText(status);

		CString KeyName((char*)lParam); 
		sprintf(status, "KEY:%d,%s", (int)wParam, KeyName);
		GetDlgItem(IDC_EDIT4)->SetWindowText(status);

#ifdef _SCRIPT_READ_PROGRESS_
		int percent = m_sc->getEXEpercent();
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(percent);
#endif

		keybd_event((int)wParam,0,0,0);
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnScEventEnd(WPARAM wParam, LPARAM lParam)
{
	if(m_sc != NULL){
		delete m_sc;
		m_sc = NULL;
	}

	m_script_enable = FALSE;
	GetDlgItem(IDC_RUN)->SetWindowText("开始");

	return 0;
}

/************************************   record   *****************************************/

void CassistantDlg::OnBnClickedRecord()
{
	// TODO: 在此添加控件通知处理程序代码

	m_record_enable = !m_record_enable;

	if(m_record_enable){

		LoadDll(&m_hInstDll);

		CTime tm = CTime::GetCurrentTime();
		CString filename = tm.Format("%Y%m%d_%H%M%S.txt");	
		CString filepath = CConfig::GetCurrentPath();

		m_rec = new CRecord(filepath + filename);

		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3)->SetWindowText(filename);
		GetDlgItem(IDC_RECORD)->SetWindowText("停止");

	}else{
		DelDLL(&m_hInstDll);
		delete m_rec;

		GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RECORD)->SetWindowText("记录");
	}
}

/************************************   hook   *****************************************/

BOOL CassistantDlg::LoadDll(HINSTANCE* _hInstDll)
{
	BOOL ret = FALSE;

	*_hInstDll = LoadLibrary(_T("MouseHook.dll"));
	if(*_hInstDll == NULL){
		AfxMessageBox(_T("no MouseHook.dll"));
		return FALSE;
	}

	if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1) || 
		BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2) ||
		BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3) ||
		BST_CHECKED == IsDlgButtonChecked(IDC_CHECK4)){

			typedef BOOL (CALLBACK *StartHookMouse)(HWND hWnd); 
			StartHookMouse StartMouseHook = (StartHookMouse)::GetProcAddress(*_hInstDll, "StartHookMouse");
			if(StartMouseHook == NULL){
				AfxMessageBox(_T("no StartHookMouse"));
				return FALSE;
			}
			ret = StartMouseHook(m_hWnd);
	}

	if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5))
	{
		typedef BOOL (CALLBACK *StartHookKeyboard)(HWND hWnd); 
		StartHookKeyboard StartKeyboardHook = (StartHookKeyboard)::GetProcAddress(*_hInstDll, "StartHookKeyboard");
		if(StartKeyboardHook == NULL){
			AfxMessageBox(_T("no StartHookKeyboard"));
			return FALSE;
		}

		ret |= StartKeyboardHook(m_hWnd);
	}

	return ret;
}

BOOL CassistantDlg::DelDLL(HINSTANCE* _hInstDll)
{
	if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1) || 
		BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2) ||
		BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3) ||
		BST_CHECKED == IsDlgButtonChecked(IDC_CHECK4)){

		typedef VOID (CALLBACK *StopHookMouse)(); 
		StopHookMouse StopMouseHook =(StopHookMouse)::GetProcAddress(*_hInstDll, "StopHookMouse");
		if(StopMouseHook == NULL){
			AfxMessageBox(_T("no StopHookMouse"));
			return FALSE;
		}

		StopMouseHook();
	}

	if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5)){
		typedef VOID (CALLBACK *StopHookKeyboard)(); 
		StopHookKeyboard StopKeyboardHook =(StopHookKeyboard)::GetProcAddress(*_hInstDll, "StopHookKeyboard");
		if(StopKeyboardHook == NULL){
			AfxMessageBox(_T("no StopHookKeyboard"));
			return FALSE;
		}

		StopKeyboardHook();
	}

	::FreeLibrary(*_hInstDll);
	*_hInstDll = NULL;

	return TRUE;
}


afx_msg LRESULT CassistantDlg::OnMsgMouseLDown(WPARAM wParam, LPARAM lParam)
{
	char status[32] = {0};

	if(!ClickIsValid(LOWORD(lParam), HIWORD(lParam)))
		return 0;

	if(m_capture_enable){
		m_capture_enable = FALSE;
		DelDLL(&m_hInstDll);
		KillTimer(1);

		POINT CapturePoint;  
		RECT CaptureRect;  
		::GetCursorPos(&CapturePoint); 
		m_hcap = ::WindowFromPoint(CapturePoint) ;
		if(m_hcap != NULL){
			::GetWindowRect(m_hcap, &CaptureRect);
			if( CaptureRect.left < 0 ) CaptureRect.left = 0;  
			if (CaptureRect.top < 0 ) CaptureRect.top = 0;  
			sprintf(status, "%d, %d, %d, %d", CaptureRect.left, CaptureRect.top, CaptureRect.right, CaptureRect.bottom);
			GetDlgItem(IDC_EDIT5)->SetWindowText(status);
		}else{
			GetDlgItem(IDC_EDIT5)->SetWindowText("NULL, NULL");
		}
		
		GetDlgItem(IDC_CAPTURE)->EnableWindow(TRUE);
	}

	if(m_record_enable && m_rec != NULL){
		if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1)){
			m_rec->SetRecord(BUTTON_L_DOWN, LOWORD(lParam), HIWORD(lParam));
			sprintf(status, "%s, %d, %d", desc[BUTTON_L_DOWN], LOWORD(lParam), HIWORD(lParam));
			GetDlgItem(IDC_EDIT2)->SetWindowText(status);
		}
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnMsgMouseLUp(WPARAM wParam, LPARAM lParam)
{
	char status[20] = {0};

	if(!ClickIsValid(LOWORD(lParam), HIWORD(lParam)))
		return 0;

	if(m_record_enable && m_rec != NULL){
		if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1)){
			m_rec->SetRecord(BUTTON_L_UP, LOWORD(lParam), HIWORD(lParam));
			sprintf(status, "%s, %d, %d", desc[BUTTON_L_UP], LOWORD(lParam), HIWORD(lParam));
			GetDlgItem(IDC_EDIT2)->SetWindowText(status);
		}
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnMsgMouseRDown(WPARAM wParam, LPARAM lParam)
{
	char status[20] = {0};

	if(!ClickIsValid(LOWORD(lParam), HIWORD(lParam)))
		return 0;

	if(m_record_enable && m_rec != NULL){
		if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3)){
			m_rec->SetRecord(BUTTON_R_DOWN, LOWORD(lParam), HIWORD(lParam));
			sprintf(status, "%s, %d, %d", desc[BUTTON_R_DOWN], LOWORD(lParam), HIWORD(lParam));
			GetDlgItem(IDC_EDIT2)->SetWindowText(status);
		}
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnMsgMouseRUp(WPARAM wParam, LPARAM lParam)
{
	char status[20] = {0};

	if(!ClickIsValid(LOWORD(lParam), HIWORD(lParam)))
		return 0;

	if(m_record_enable && m_rec != NULL){
		if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3)){
			m_rec->SetRecord(BUTTON_R_UP, LOWORD(lParam), HIWORD(lParam));
			sprintf(status, "%s, %d, %d", desc[BUTTON_R_UP], LOWORD(lParam), HIWORD(lParam));
			GetDlgItem(IDC_EDIT2)->SetWindowText(status);
		}
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnMsgMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(m_capture_enable){
		char status[20];
		sprintf(status, "%d,%d",LOWORD(lParam), HIWORD(lParam));
		GetDlgItem(IDC_EDIT5)->SetWindowText(status);
	}

	return 0;
}

afx_msg LRESULT CassistantDlg::OnMsgKeydown(WPARAM wParam, LPARAM lParam)
{
	DWORD dwvk = wParam;
	DWORD dwMsg= lParam;
	char KeyName[20] = {0};

	if(m_record_enable && m_rec != NULL){
		if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5)){
			m_rec->SetRecord(KEY_DOWN, dwvk, dwMsg);
			GetKeyNameTextA(dwMsg,KeyName,sizeof(KeyName));
			GetDlgItem(IDC_EDIT2)->SetWindowText(KeyName);
		}
	}

	return 0;
}

VOID CassistantDlg::MakeMouseClick(int event, int x, int y)
{
	int screenX = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度 
	int screenY = GetSystemMetrics(SM_CYSCREEN); //屏幕高度 

	if((x>screenX) || (y>screenY))
		return;

	SetCursorPos(x, y);	

	if(event == BUTTON_L_DOWN)
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);	
	else if((event == BUTTON_L_UP))
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);	
	else if((event == BUTTON_R_DOWN))
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);	
	else if((event == BUTTON_R_UP))
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);	
	else{

	}
}

/************************************   multiple   *****************************************/
BOOL CassistantDlg::TermIsMultiple()
{
	if(m_cfg != NULL){
		for(int i=1; i<m_cfg->getMouseSize(); i++){
			MouseCalibration *Mouse = m_cfg->getMouseConfig(i);
			if(Mouse != NULL && Mouse->enable){
				return TRUE;
			}
		}
	}

	return FALSE;
}

VOID CassistantDlg::MakeMouseClickEx(int event, int x, int y)
{
	if(m_multiple && event != BUTTON_L_DOWN)		// 多终端模式下只响应点击事件
		return;

	MakeMouseClick(event, x, y);

	if(m_multiple){
		MakeMouseClick(BUTTON_L_UP, x, y);			// 多终端模式下将DOWN事件补全为点击事件
		MouseCalibration *MouseDefault = m_cfg->getMouseConfig(0);
		for(int i=1; i<m_cfg->getMouseSize(); i++){
			MouseCalibration *Mouse = m_cfg->getMouseConfig(i);
			if(Mouse != NULL && Mouse->enable){
				::Sleep(100);						// 克隆事件到其他终端
				MakeMouseClick(BUTTON_L_DOWN, 
					x+Mouse->pos.x-MouseDefault->pos.x, 
					y+Mouse->pos.y-MouseDefault->pos.y);
				MakeMouseClick(BUTTON_L_UP, 
					x+Mouse->pos.x-MouseDefault->pos.x, 
					y+Mouse->pos.y-MouseDefault->pos.y);
			}
		}
	}
}

/************************************   public   *****************************************/

BOOL CassistantDlg::ClickIsValid(int x, int y)
{
	CRect ClientRect;
	GetWindowRect(&ClientRect);
	CPoint pt(x, y);
	if (ClientRect.PtInRect(pt))
		return FALSE;
	else
		return TRUE;
}

