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
|      $Id: ComPicDecoder.h,v 1.1 2009/12/09 05:34:36 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef __COMPICDECODER_H_
#define __COMPICDECODER_H_

#include "resource.h"       // main symbols
#include "planydec.h"
#include "plwinbmp.h"

/////////////////////////////////////////////////////////////////////////////
// CComPicDecoder
class ATL_NO_VTABLE CComPicDecoder :
      public CComObjectRootEx<CComSingleThreadModel>,
      public CComCoClass<CComPicDecoder, &CLSID_PicDecoder>,
      public IDispatchImpl<IPicDecoder, &IID_IPicDecoder, &LIBID_PAINTXLib>
{
public:
  CComPicDecoder();
  STDMETHOD(CreatePictureFromFile)(/*[in]*/ BSTR bstrFilename,
      /*[in]*/ int intBPPWanted,
      /*[out, retval]*/ IUnknown ** ppUnk);
  void raiseError
  ( int Code,
    char * pszErr
  );
  HRESULT SaveBmpToGMemStream(PLWinBmp* WinBmp, LPSTREAM * lpStream);


  DECLARE_REGISTRY_RESOURCEID(IDR_COMPICDECODER)

  BEGIN_COM_MAP(CComPicDecoder)
  COM_INTERFACE_ENTRY(IPicDecoder)
  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

  // IPicDecoder
protected:
  PLAnyPicDecoder m_AnyPicDecoder;
};

#endif //__COMPICDECODER_H_
/*
/--------------------------------------------------------------------
|
|      $Log: ComPicDecoder.h,v $
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
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2000/01/10 23:53:00  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|
--------------------------------------------------------------------
*/
