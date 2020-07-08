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
|      $Id: plfilterthreshold.h,v 1.1 2009/12/09 05:33:45 박종운 Exp $
|   
|
\--------------------------------------------------------------------
*/

#if !defined(INCL_PLFILTERTHRESHOLD)
#define INCL_PLFILTERTHRESHOLD

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "plfilter.h"

class PLBmp;

//! Returns an 8 bpp grayscale bitmap that contains only the channel
//! selected and is black wherever this channel has values <= threshold_min
//! or >= threshold_max.
class PLFilterThreshold : public PLFilter
{
public:
  //! 
	PLFilterThreshold(int threshold_min,int threshold_max, int channel);
  //! 
	virtual ~PLFilterThreshold();

  //! 
  virtual void Apply(PLBmp * pBmpSource, PLBmp * pBmpDest) const;

protected:
	int m_threshold_min;
	int m_threshold_max;
	int m_channel;


};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterthreshold.h,v $
|      Revision 1.1  2009/12/09 05:33:45  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:11  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:54  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:50  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:02  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:18  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.2  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.1  2000/09/26 14:28:47  Administrator
|      Added Threshold filter
|
|
|
\--------------------------------------------------------------------
*/
