// MouseHook.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MouseHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_MSG_MOUSE_L_DOWM   WM_USER + 305
#define WM_MSG_MOUSE_L_UP     WM_USER + 306
#define WM_MSG_MOUSE_R_DOWM   WM_USER + 307
#define WM_MSG_MOUSE_R_UP     WM_USER + 308
#define WM_MSG_MOUSE_MOVE     WM_USER + 309 
#define WM_MSG_KEYDOWN        WM_USER + 310


//将变量放在共享段,及所有线程共享以下变量;
#pragma data_seg(".SHARED")
static HHOOK  hhkMouse = NULL;		//鼠标钩子句柄;
static HHOOK  hhkKeyboard = NULL;	//键盘钩子句柄;
static HINSTANCE g_hInstDll = NULL; //本dll的实例句柄;
static HWND g_hWnd = NULL;			//调用dll的主窗口句柄
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")


//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMouseHookApp

BEGIN_MESSAGE_MAP(CMouseHookApp, CWinApp)
END_MESSAGE_MAP()


// CMouseHookApp 构造

CMouseHookApp::CMouseHookApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMouseHookApp 对象

CMouseHookApp theApp;


// CMouseHookApp 初始化

BOOL CMouseHookApp::InitInstance()
{
	CWinApp::InitInstance();

	//获取自身dll句柄
	g_hInstDll = ::AfxGetInstanceHandle();

	return TRUE;
}

//低级鼠标钩子过程
LRESULT CALLBACK LowLevelMouseProc(
	int nCode,      // hook code
	WPARAM wParam,  // message identifier
	LPARAM lParam   // mouse coordinates
	)
{
	//有鼠标消息时，将其发给主程序
	if (g_hWnd != NULL && nCode == HC_ACTION)
	{
		if(wParam == WM_LBUTTONDOWN){
			::PostMessage(g_hWnd, WM_MSG_MOUSE_L_DOWM, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_LBUTTONUP){
			::PostMessage(g_hWnd, WM_MSG_MOUSE_L_UP, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_RBUTTONDOWN){
			::PostMessage(g_hWnd, WM_MSG_MOUSE_R_DOWM, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_RBUTTONUP){
			::PostMessage(g_hWnd, WM_MSG_MOUSE_R_UP, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_MOUSEMOVE){
			::PostMessage(g_hWnd, WM_MSG_MOUSE_MOVE, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
	}

	return CallNextHookEx(hhkMouse,nCode,wParam,lParam);
}


/*安装低级鼠标钩子，从而截获系统所有的鼠标消息*/
BOOL WINAPI StartHookMouse(HWND hWnd)
{
	TRACE(_T("MOUSEDLL# Start Hook Mouse...\r\n"));

	g_hWnd = hWnd;

	hhkMouse =::SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hInstDll, 0);
	if(hhkMouse == NULL)
		return FALSE;
	else
		return TRUE;
}

/**卸载低级鼠标钩子**/
VOID WINAPI StopHookMouse()
{
	TRACE(_T("MOUSEDLL# Stop Hook Mouse...\r\n"));

	if (hhkMouse != NULL)
	{
		::UnhookWindowsHookEx(hhkMouse);
	}
}


//键盘钩子过程
LRESULT CALLBACK LowLevelKeyboardProc(
	int nCode,      // hook code
	WPARAM wParam,  // message identifier
	LPARAM lParam   // mouse coordinates
	)
{
	if(nCode ==HC_ACTION && wParam == WM_KEYDOWN)	//按键按下
	{
		PKBDLLHOOKSTRUCT pHookStruct = (PKBDLLHOOKSTRUCT)lParam;

		DWORD dwvk = pHookStruct->vkCode;
		DWORD dwMsg = 1 + (pHookStruct->scanCode << 16) + (pHookStruct->flags << 24);

#if 0
		char KeyName[20];
		GetKeyNameTextA(dwMsg,KeyName,sizeof(KeyName));
		TRACE("MOUSEDLL# keydown:%d, %s\r\n", dwvk, KeyName);
#endif

		if (g_hWnd != NULL)
			::PostMessage(g_hWnd, WM_MSG_KEYDOWN, dwvk, dwMsg);	
	}

	return CallNextHookEx(hhkKeyboard, nCode, wParam, lParam);
}


/**安装低级键盘钩子**/
BOOL WINAPI StartHookKeyboard(HWND hWnd){

	TRACE(_T("MOUSEDLL# Start Hook Keyboard...\r\n"));

	g_hWnd = hWnd;

	hhkKeyboard =::SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hInstDll, 0);
	if(hhkKeyboard == NULL)
		return FALSE;
	else
		return TRUE;
}

/**卸载键盘钩子**/
VOID WINAPI StopHookKeyboard(){

	TRACE(_T("MOUSEDLL# Stop Hook Keyboard...\r\n"));

	if (hhkKeyboard != NULL)
	{
		::UnhookWindowsHookEx(hhkKeyboard);
	}
}
