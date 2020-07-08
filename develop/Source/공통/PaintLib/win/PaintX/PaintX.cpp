/*
/--------------------------------------------------------------------
|
|      $Id: PaintX.cpp,v 1.1 2009/12/09 05:34:36 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

// Note: Proxy/Stub Information
//    To build a separate proxy/stub DLL,
//    run nmake -f PaintXps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "PaintX.h"

#include "PaintX_i.c"
#include "PictureDecoder.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_PictureDecoder, CPictureDecoder)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
bool WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_PAINTXLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return true;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(true);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(true);
}

/*
/--------------------------------------------------------------------
|
|      $Log: PaintX.cpp,v $
|      Revision 1.1  2009/12/09 05:34:36  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:02  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:35  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:16  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:24  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:38  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:53  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:07  황정영
|      2006.05.17
|
|      Revision 1.7  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.6  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.5  2000/09/01 14:13:49  Administrator
|      Removed MFC from paintX, added MSCV paintX sample.
|
|      Revision 1.4  2000/01/10 23:53:00  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|
--------------------------------------------------------------------
*/
