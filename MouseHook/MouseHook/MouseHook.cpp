// MouseHook.cpp : ���� DLL �ĳ�ʼ�����̡�
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


//���������ڹ����,�������̹߳������±���;
#pragma data_seg(".SHARED")
static HHOOK  hhkMouse = NULL;		//��깳�Ӿ��;
static HINSTANCE g_hInstDll = NULL; //��dll��ʵ�����;
static HWND g_hWnd = NULL;			//����dll�������ھ��
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")


//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CMouseHookApp

BEGIN_MESSAGE_MAP(CMouseHookApp, CWinApp)
END_MESSAGE_MAP()


// CMouseHookApp ����

CMouseHookApp::CMouseHookApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMouseHookApp ����

CMouseHookApp theApp;


// CMouseHookApp ��ʼ��

BOOL CMouseHookApp::InitInstance()
{
	CWinApp::InitInstance();

	//��ȡ����dll���
	g_hInstDll = ::AfxGetInstanceHandle();

	return TRUE;
}

//�ͼ���깳�ӹ���
LRESULT CALLBACK LowLevelMouseProc(
	int nCode,      // hook code
	WPARAM wParam,  // message identifier
	LPARAM lParam   // mouse coordinates
	)
{
	//�������Ϣʱ�����䷢��������
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


/*��װ�ͼ���깳�ӣ��Ӷ��ػ�ϵͳ���е������Ϣ*/
BOOL WINAPI StartHookMouse(HWND hWnd)
{
	g_hWnd = hWnd;

	hhkMouse =::SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hInstDll, 0);
	if(hhkMouse == NULL)
		return FALSE;
	else
		return TRUE;
}

/**ж�صͼ���깳��**/
VOID WINAPI StopHookMouse()
{
	if (hhkMouse != NULL)
	{
		::UnhookWindowsHookEx(hhkMouse);
	}
}