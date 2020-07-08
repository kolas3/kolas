#include <windows.h>
#include <winwlx.h>

struct { 
    HMODULE hDll;
    BOOL (WINAPI *WlxNegotiate)();
    BOOL (WINAPI *WlxInitialize)();
    VOID (WINAPI *WlxDisplaySASNotice)();
    INT (WINAPI *WlxLoggedOutSAS)();
    BOOL (WINAPI *WlxActivateUserShell)();
    INT (WINAPI *WlxLoggedOnSAS)();
    VOID (WINAPI *WlxDisplayLockedNotice)();
    INT (WINAPI *WlxWkstaLockedSAS)();
    BOOL (WINAPI *WlxIsLockOk)();
    BOOL (WINAPI *WlxIsLogoffOk)();
    VOID (WINAPI *WlxLogoff)();
    VOID (WINAPI *WlxShutdown)();
    BOOL (WINAPI *WlxScreenSaverNotify)();
    BOOL (WINAPI *WlxStartApplication)();
    BOOL (WINAPI *WlxNetworkProviderLoad)();
} MsGina;

BOOL WINAPI WlxNegotiate(DWORD dwWinlogonVersion, PDWORD pdwDllVersion)
{
    MsGina.hDll = LoadLibrary(_T("msgina.dll"));
    if(MsGina.hDll == NULL) return FALSE;

    MsGina.WlxActivateUserShell = GetProcAddress(MsGina.hDll, _T("WlxActivateUserShell"));
    MsGina.WlxDisplayLockedNotice = GetProcAddress(MsGina.hDll, _T("WlxDisplayLockedNotice"));
    MsGina.WlxDisplaySASNotice = GetProcAddress(MsGina.hDll, _T("WlxDisplaySASNotice"));
    MsGina.WlxInitialize = GetProcAddress(MsGina.hDll, _T("WlxInitialize"));
    MsGina.WlxIsLockOk = GetProcAddress(MsGina.hDll, _T("WlxIsLockOk"));
    MsGina.WlxIsLogoffOk = GetProcAddress(MsGina.hDll, _T("WlxIsLogoffOk"));
    MsGina.WlxLoggedOnSAS = GetProcAddress(MsGina.hDll, _T("WlxLoggedOnSAS"));
    MsGina.WlxLoggedOutSAS = GetProcAddress(MsGina.hDll, _T("WlxLoggedOutSAS"));
    MsGina.WlxLogoff = GetProcAddress(MsGina.hDll, _T("WlxLogoff"));
    MsGina.WlxNegotiate = GetProcAddress(MsGina.hDll, _T("WlxNegotiate"));
    MsGina.WlxScreenSaverNotify = GetProcAddress(MsGina.hDll, _T("WlxScreenSaverNotify"));
    MsGina.WlxShutdown = GetProcAddress(MsGina.hDll, _T("WlxShutdown"));
    MsGina.WlxStartApplication = GetProcAddress(MsGina.hDll, _T("WlxStartApplication"));
    MsGina.WlxWkstaLockedSAS = GetProcAddress(MsGina.hDll, _T("WlxWkstaLockedSAS"));

    return MsGina.WlxNegotiate(dwWinlogonVersion, pdwDllVersion);
}

BOOL WINAPI WlxInitialize(
    LPWSTR lpWinsta,
    HANDLE hWlx,
    PVOID pvReserved,
    PVOID pWinlogonFunctions,
    PVOID *pWlxContext)
{
    return MsGina.WlxInitialize(
        lpWinsta,
        hWlx,
        pvReserved,
        pWinlogonFunctions,
        pWlxContext);
}

VOID WINAPI WlxDisplaySASNotice(PVOID pWlxContext)
{
    MsGina.WlxDisplaySASNotice(pWlxContext);
}

INT WINAPI WlxLoggedOutSAS(
    PVOID pWlxContext,
    DWORD dwSasType,
    PLUID pAuthenticationId,
    PSID pLogonSid,
    PDWORD pdwOptions,
    PHANDLE phToken,
    PWLX_MPR_NOTIFY_INFO pNprNotifyInfo,
    PVOID *pProfile)
{
    return MsGina.WlxLoggedOutSAS(
        pWlxContext,
        dwSasType,
        pAuthenticationId,
        pLogonSid,
        pdwOptions,
        phToken,
        pNprNotifyInfo,
        pProfile);
}

BOOL WINAPI WlxActivateUserShell(
    PVOID pWlxContext,
    PWSTR pszDesktopName,
    PWSTR pszMprLogonScript,
    PVOID pEnvironment)
{
    return MsGina.WlxActivateUserShell(
        pWlxContext,
        pszDesktopName,
        pszMprLogonScript,
        pEnvironment);
}

INT WINAPI WlxLoggedOnSAS(PVOID pWlxContext, DWORD dwSasType, PVOID pReserved)
{
    HANDLE hMutex;

    if(dwSasType == WLX_SAS_TYPE_CTRL_ALT_DEL) {
        hMutex = OpenMutex(MUTANT_QUERY_STATE, FALSE, _T("mygina mutex"));
        if(hMutex != NULL) {
            CloseHandle(hMutex);
            return WLX_SAS_ACTION_NONE;
        }
    }

    return MsGina.WlxLoggedOnSAS(pWlxContext, dwSasType, pReserved);
}

VOID WINAPI WlxDisplayLockedNotice(PVOID pWlxContext)
{
    MsGina.WlxDisplayLockedNotice(pWlxContext);
}

INT WINAPI WlxWkstaLockedSAS(PVOID pWlxContext, DWORD dwSasType)
{
    return MsGina.WlxWkstaLockedSAS(pWlxContext, dwSasType);
}

BOOL WINAPI WlxIsLockOk(PVOID pWlxContext)
{
    return MsGina.WlxIsLockOk(pWlxContext);
}

BOOL WINAPI WlxIsLogoffOk(PVOID pWlxContext)
{
    return MsGina.WlxIsLogoffOk(pWlxContext);
}

VOID WINAPI WlxLogoff(PVOID pWlxContext)
{
    MsGina.WlxLogoff(pWlxContext);
}

VOID WINAPI WlxShutdown(PVOID pWlxContext, DWORD ShutdownType)
{
    MsGina.WlxShutdown(pWlxContext, ShutdownType);
}

BOOL WINAPI WlxScreenSaverNotify(PVOID pWlxContext, BOOL *pSecure)
{
    return MsGina.WlxScreenSaverNotify(pWlxContext, pSecure);
}

BOOL WINAPI WlxStartApplication(
    PVOID pWlxContext,
    PWSTR pszDesktopName,
    PVOID pEnvironment,
    PWSTR pszCmdLine)
{
    return MsGina.WlxStartApplication(pWlxContext, pszDesktopName, pEnvironment, pszCmdLine);
}

BOOL WINAPI WlxNetworkProviderLoad(PVOID pWlxContext, PWLX_MPR_NOTIFY_INFO pNprNotifyInfo)
{
    return MsGina.WlxNetworkProviderLoad(pWlxContext, pNprNotifyInfo);
}
