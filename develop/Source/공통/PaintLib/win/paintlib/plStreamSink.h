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
|      $Id: plStreamSink.h,v 1.1 2009/12/09 05:34:46 박종운 Exp $
|      Copyright (c) 1996-2000 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#if !defined(AFX_STREAMSINK_H__5FCF8981_7C33_11D4_AD12_00010209C12B__INCLUDED_)
#define AFX_STREAMSINK_H__5FCF8981_7C33_11D4_AD12_00010209C12B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "plobject.h"
#include "pldatasink.h"
#include "plpaintlibdefs.h"

class PLStreamSink : public PLDataSink
{
public:
	IStream * GetIStream();
	PLStreamSink();
	virtual ~PLStreamSink();

  //!
  virtual int Open
    (
	  int MaxFileSize
    );

  //!
  virtual void Close
    ();

private:
  IStream * m_pIStream;
  PLBYTE * m_pDataBuf;
	HGLOBAL m_hMem;
protected:
};

#endif // !defined(AFX_STREAMSINK_H__5FCF8981_7C33_11D4_AD12_00010209C12B__INCLUDED_)

/*
/--------------------------------------------------------------------
|
|      $Log: plStreamSink.h,v $
|      Revision 1.1  2009/12/09 05:34:46  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:06  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:43  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:20  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:28  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:44  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:58  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:08  황정영
|      2006.05.17
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.1  2000/09/01 14:19:46  Administrator
|      no message
|
|
\--------------------------------------------------------------------
*/
