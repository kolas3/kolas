#include "stdafx.h"
#include "keyHook.h"

#include <stdio.h>
#include <Windows.h>
#include <WINUSER.H>
typedef struct tagKBDLLHOOKSTRUCT {
    DWORD     vkCode;
    DWORD     scanCode;
    DWORD     flags;
    DWORD     time;
//    ULONG_PTR dwExtraInfo;
} KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

/*
typedef struct tagMSLLHOOKSTRUCT {
  POINT     pt;
  DWORD     mouseData;
  DWORD     flags;
  DWORD     time;
  ULONG_PTR dwExtraInfo;
} MSLLHOOKSTRUCT, *PMSLLHOOKSTRUCT;
*/

#define WH_KEYBOARD_LL     13
#define LLKHF_ALTDOWN        0x00000020


#define _WIN32_WINNT 0x0400 

#pragma data_seg(_T(".HOOKDATA"))//Shared data among all instances.
HHOOK hook = NULL;
HHOOK hook2 = NULL;
HWND hwnd = NULL;
#pragma data_seg()

#pragma comment(linker, _T("/SECTION:.HOOKDATA,RWS"))//linker directive

HINSTANCE hinstance = NULL;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }

	hinstance = (HINSTANCE)hModule;
	hook = NULL;

    return TRUE;
}


__declspec(dllexport) void installhook(HWND h,INT nOSversion)
{
	hook = NULL;
	hwnd = h;
	
	if(nOSversion == 0) hook = SetWindowsHookEx(WH_KEYBOARD ,hookprocold,hinstance,NULL); //for 95,98
	if(nOSversion == 1) hook = SetWindowsHookEx(WH_KEYBOARD_LL ,hookprocnew,hinstance,NULL); //for NT,XP

	hook2 = SetWindowsHookEx(WH_MOUSE ,mousehookproc,hinstance,NULL); //for NT,XP

	if(hook == NULL)
		MessageBox(NULL,_T("Unable to install hook"),_T("Error!"),MB_OK);
	
	if(hook2 == NULL)
		MessageBox(NULL,_T("Unable to install mouse hook"),_T("Error!"),MB_OK);

}

__declspec(dllexport) void installhook_client(HWND h,INT nOSversion)
{
	hook = NULL;
	hwnd = h;
	
	if(nOSversion == 0) hook = SetWindowsHookEx(WH_KEYBOARD ,hookprocold_client,hinstance,NULL); //for 95,98
	if(nOSversion == 1) hook = SetWindowsHookEx(WH_KEYBOARD_LL ,hookprocnew_client,hinstance,NULL); //for NT,XP

	hook2 = SetWindowsHookEx(WH_MOUSE ,mousehookproc,hinstance,NULL); //for NT,XP

	if(hook == NULL)
		MessageBox(NULL,_T("Unable to install hook"),_T("Error!"),MB_OK);
	
	if(hook2 == NULL)
		MessageBox(NULL,_T("Unable to install mouse hook"),_T("Error!"),MB_OK);

}


__declspec(dllexport) void removehook()
{
	UnhookWindowsHookEx(hook);
	UnhookWindowsHookEx(hook2);
}

__declspec(dllexport) LRESULT CALLBACK hookprocold(int nCode,WPARAM wParam,LPARAM lParam)
{
	//for win95,win98 version
	BOOL fEatKeystroke = FALSE; 
	
	if (nCode == HC_ACTION) {
		 
		 if (wParam == VK_LWIN || wParam == VK_RWIN) fEatKeystroke = TRUE;			
         
		 if (GetKeyState(VK_MENU))
		 {
			 if (wParam == VK_F4) fEatKeystroke = TRUE;
			 if (wParam == VK_ESCAPE) fEatKeystroke = TRUE;	
			 if (wParam == VK_TAB) 	fEatKeystroke = TRUE;	
		     if (wParam == VK_DELETE) fEatKeystroke = TRUE;	
		     if (wParam == VK_SPACE) fEatKeystroke = TRUE;	
		 }

 		if( wParam == VK_F4 ) fEatKeystroke = true;
		

		 if (GetKeyState(VK_CONTROL))
		 {
			if (wParam == VK_ESCAPE) fEatKeystroke = TRUE;			
		//	if (wParam == 0x50) fEatKeystroke = TRUE;	 		
 			if (wParam == 0x4E) fEatKeystroke = TRUE;	 		
		}

		 if(wParam == VK_RETURN)
		 {
			 if(GetAsyncKeyState(VK_SHIFT)) fEatKeystroke = TRUE;	
		 }

	}

   return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam)); 

}


__declspec(dllexport) LRESULT CALLBACK hookprocnew(int nCode,WPARAM wParam,LPARAM lParam)
{
   //for win2k, xp version
   BOOL fEatKeystroke = FALSE; 
     
   if (nCode == HC_ACTION) {
      switch (wParam) { 
  		
      case WM_KEYDOWN:  case WM_SYSKEYDOWN: 
     
	  case WM_KEYUP:    case WM_SYSKEYUP:
			
        
         PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam; 
         fEatKeystroke =  
			( ( p->flags & LLKHF_ALTDOWN ) != 0 ) ||
			(p->vkCode == VK_F11) || // F11추가
			((p->vkCode == VK_TAB) && ((p->flags & LLKHF_ALTDOWN) != 0)) ||
			((p->vkCode == VK_F4) && ((p->flags & LLKHF_ALTDOWN) != 0)) || 
            ((p->vkCode == VK_ESCAPE) && ((p->flags & LLKHF_ALTDOWN) != 0)) || 
            ((p->vkCode == VK_ESCAPE) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) ||
			((p->vkCode == VK_DELETE) && ((p->flags & LLKHF_ALTDOWN ) != 0) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0))||
			((p->vkCode == VK_DELETE) && ((p->flags & LLKHF_ALTDOWN ) != 0)) ||
            ((p->vkCode == VK_SPACE) && ((p->flags & LLKHF_ALTDOWN ) != 0)) ||
			((p->vkCode == 0x50) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) ||
			((p->vkCode == VK_RETURN) && (GetAsyncKeyState(VK_SHIFT))) ||
			(p->vkCode == VK_LWIN) || (p->vkCode == VK_RWIN) || 
			(( p->vkCode == 0x4E) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) || // new window
			(p->vkCode == VK_ESCAPE) ||
			((p->vkCode == 0x4F) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) || // open
			((p->vkCode == 0x53) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) ; // save					
        break;
      } 
   } 

	return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}


__declspec(dllexport) LRESULT CALLBACK hookprocold_client(int nCode,WPARAM wParam,LPARAM lParam)
{
	//for win95,win98 version
	BOOL fEatKeystroke = FALSE; 
	
	if (nCode == HC_ACTION) {
		 
		 if (wParam == VK_LWIN || wParam == VK_RWIN) fEatKeystroke = TRUE;			
         
		 if (GetKeyState(VK_MENU))
		 {
			 if (wParam == VK_F4) fEatKeystroke = TRUE;
			 if (wParam == VK_ESCAPE) fEatKeystroke = TRUE;	
			 if (wParam == VK_TAB) 	fEatKeystroke = TRUE;	
		     if (wParam == VK_DELETE) fEatKeystroke = TRUE;	
		     if (wParam == VK_SPACE) fEatKeystroke = TRUE;	
		 }

 		if( wParam == VK_F4 ) fEatKeystroke = true;
		

		 if (GetKeyState(VK_CONTROL))
		 {
			if (wParam == VK_ESCAPE) fEatKeystroke = TRUE;			
		//	if (wParam == 0x50) fEatKeystroke = TRUE;	 		
 			if (wParam == 0x4E) fEatKeystroke = TRUE;	 		
		}

		 if(wParam == VK_RETURN)
		 {
			 if(GetAsyncKeyState(VK_SHIFT)) fEatKeystroke = TRUE;	
		 }

	}

   return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam)); 

}


__declspec(dllexport) LRESULT CALLBACK hookprocnew_client(int nCode,WPARAM wParam,LPARAM lParam)
{
   //for win2k, xp version
   BOOL fEatKeystroke = FALSE; 
     
   if (nCode == HC_ACTION) {
      switch (wParam) { 
  		
      case WM_KEYDOWN:  case WM_SYSKEYDOWN: 
     
	  case WM_KEYUP:    case WM_SYSKEYUP:
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam; 	
         fEatKeystroke =  			 
			( ( p->flags & LLKHF_ALTDOWN ) != 0 ) ||			
//			(p->vkCode == VK_F11) || // F11추가
			((p->vkCode == VK_TAB) && ((p->flags & LLKHF_ALTDOWN) != 0)) ||
			((p->vkCode == VK_F4) && ((p->flags & LLKHF_ALTDOWN) != 0)) || 
            ((p->vkCode == VK_ESCAPE) && ((p->flags & LLKHF_ALTDOWN) != 0)) || 
            ((p->vkCode == VK_ESCAPE) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) ||
			((p->vkCode == VK_DELETE) && ((p->flags & LLKHF_ALTDOWN ) != 0) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0))||
			((p->vkCode == VK_DELETE) && ((p->flags & LLKHF_ALTDOWN ) != 0)) ||
            ((p->vkCode == VK_SPACE) && ((p->flags & LLKHF_ALTDOWN ) != 0)) ||
			((p->vkCode == 0x50) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) ||
			((p->vkCode == VK_RETURN) && (GetAsyncKeyState(VK_SHIFT))) ||
			(p->vkCode == VK_LWIN) || (p->vkCode == VK_RWIN) || 
			(( p->vkCode == 0x4E) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) || // new window
			/*(p->vkCode == VK_ESCAPE) ||*/
			((p->vkCode == 0x4F) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) || // open
			((p->vkCode == 0x53) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)) ; // save					

			HWND hTopWnd = GetForegroundWindow();
			if(hwnd != hTopWnd)
// 			TCHAR szTitle[MAX_PATH+1];
// 			memset(szTitle, NULL, sizeof(szTitle));
// 			::GetWindowText(hTopWnd, szTitle, MAX_PATH);
// 			if(0 != _tcscmp(_T("제목없음 - UserMain"), szTitle))
 			{
 				fEatKeystroke = fEatKeystroke || (p->vkCode == VK_F11) || (p->vkCode == VK_F12);
 			}
			else
			{
				TRACE(_T(""));
			}

         
        break;
      } 
   } 

	return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}


__declspec(dllexport) LRESULT CALLBACK mousehookproc(int nCode,WPARAM wParam,LPARAM lParam)
{
   //for win2k, xp version
   BOOL fEatMousestroke = FALSE; 
 

   if (nCode == HC_ACTION) {
      switch (wParam) { 
  		
      case WM_RBUTTONDOWN:  case WM_RBUTTONUP: 
		// 마우스 우클릭으로 팝업메뉴가 띄울 경우엔 SetCapture가 호출되기 때문에 해제해준다.
		ReleaseCapture(); 
		fEatMousestroke = TRUE;
		break;
     
		
	  case WM_LBUTTONDOWN:  case WM_LBUTTONUP:
	
		 if(GetAsyncKeyState(VK_SHIFT))
		 		fEatMousestroke = TRUE;
		 

	    break;
		
      } 
   } 
	
   return(fEatMousestroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam)); 

}

#define     TASKBAR         _T("Shell_TrayWnd")

/****************************************
 * Disable Task Manager (CTRL+ALT+DEL). *
 * TRUE=Enable, FALSE=Disable           *
 * (Win NT/2K/XP)                       *
 ****************************************/
__declspec(dllexport)  int TaskManager_Enable_Disable(BOOL bEnableDisable)
{
    #define KEY_DISABLETASKMGR  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System")
    #define VAL_DISABLETASKMGR  _T("DisableTaskMgr")

    HKEY    hKey;
    DWORD   val;
	LONG	r;

    if (RegOpenKey(HKEY_CURRENT_USER, KEY_DISABLETASKMGR, &hKey) != ERROR_SUCCESS)
        if (RegCreateKey(HKEY_CURRENT_USER, KEY_DISABLETASKMGR, &hKey) != ERROR_SUCCESS)
			return 0;

    if (bEnableDisable) // Enable
    {
        r = RegDeleteValue(hKey, VAL_DISABLETASKMGR);
    }
    else                // Disable
    {
        val = 1;
        r = RegSetValueEx(hKey, VAL_DISABLETASKMGR, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
    }

    RegCloseKey(hKey);

    return (r == ERROR_SUCCESS ? 1 : 0);
}


/*************************
 * Hide/Show Taskbar.    *
 * TRUE=Show, FALSE=Hide * 
 * (Win9x/NT/2K/XP)      *
 *************************/
__declspec(dllexport) Taskbar_Show_Hide(BOOL bShowHide)
{
    HWND    hWnd;

    hWnd = FindWindow(TASKBAR, NULL);
    if (hWnd == NULL)
        return 0;

    ShowWindow(hWnd, bShowHide ? SW_SHOW : SW_HIDE);
    UpdateWindow(hWnd);

    return 1;
}