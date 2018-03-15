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


//将变量放在共享段,及所有线程共享以下变量;
#pragma data_seg(".SHARED")
static HHOOK  hhkMouse = NULL;		//鼠标钩子句柄;
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
			::SendMessage(g_hWnd, WM_MSG_MOUSE_L_DOWM, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_LBUTTONUP){
			::SendMessage(g_hWnd, WM_MSG_MOUSE_L_UP, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_RBUTTONDOWN){
			::SendMessage(g_hWnd, WM_MSG_MOUSE_R_DOWM, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_RBUTTONUP){
			::SendMessage(g_hWnd, WM_MSG_MOUSE_R_UP, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
		if(wParam == WM_MOUSEMOVE){
			::SendMessage(g_hWnd, WM_MSG_MOUSE_MOVE, wParam, MAKELPARAM(((PMSLLHOOKSTRUCT)lParam)->pt.x, ((PMSLLHOOKSTRUCT)lParam)->pt.y));	
		}
	}

	return CallNextHookEx(hhkMouse,nCode,wParam,lParam);
}


/*安装低级鼠标钩子，从而截获系统所有的鼠标消息*/
BOOL WINAPI StartHookMouse(HWND hWnd)
{
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
	if (hhkMouse != NULL)
	{
		::UnhookWindowsHookEx(hhkMouse);
	}
}