/*The zlib/libpng License

Copyright (c) <year> <copyright holders>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.*/
/*
/--------------------------------------------------------------------
|
|      $Id: PictureDecoder.h,v 1.1 2009/12/09 05:34:37 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef __PICTUREDECODER_H_
#define __PICTUREDECODER_H_

#include "resource.h"       // main symbols
#include "planydec.h"
#include "plbitmap.h"
#include "plexcept.h"
#include "plstreamsink.h"

extern CComModule _Module;

/////////////////////////////////////////////////////////////////////////////
// CPictureDecoder
class ATL_NO_VTABLE CPictureDecoder : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPictureDecoder, &CLSID_PictureDecoder>,
	public ISupportErrorInfo,
	public IDispatchImpl<IPictureDecoder, &IID_IPictureDecoder, &LIBID_PAINTXLib>
{
public:
	CPictureDecoder()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PICTUREDECODER)

BEGIN_COM_MAP(CPictureDecoder)
	COM_INTERFACE_ENTRY(IPictureDecoder)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// PictureDecoder
public:
	STDMETHOD(LoadResPicture)(/*[in]*/ int ResourceID, /*[out, retval]*/ IUnknown ** ppUnk);
	STDMETHOD(LoadPicture)(/*[in]*/ BSTR Filename, /*[out, retval]*/ IUnknown ** ppUnk);
protected:
  IPictureDisp * BitmapToPicture(PLBmp* pBmp);
	HRESULT SignalPictureChanged(IUnknown * pUnknown);
	HRESULT CreateErrorInfo(PLTextException  TextException,REFIID riidSource);
	PLAnyPicDecoder m_AnyPicDecoder;
};

#endif //__PICTUREDECODER_H_
/*
/--------------------------------------------------------------------
|
|      $Log: PictureDecoder.h,v $
|      Revision 1.1  2009/12/09 05:34:37  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:03  박대우
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
|      Revision 1.1  2006/11/29 03:34:40  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:54  황정영
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
|
--------------------------------------------------------------------
*/
