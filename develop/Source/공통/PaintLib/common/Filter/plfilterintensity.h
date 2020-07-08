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
|      $Id: plfilterintensity.h,v 1.1 2009/12/09 05:33:41 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#if !defined(INCL_PLFILTERINTENSITY)
#define INCL_PLFILTERINTENSITY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "plfilter.h"
#include "plpaintlibdefs.h"

class PLBmp;

//! Changes the intensity of a 24 or 32 bpp bitmap. 
//! Applies the factor intensityFactor = 1.0 + csupp * pow((v-m_offset), m_exponent)
//! with csupp = intensity/pow(255.0, m_exponent);
//! on the v-Value of the image after a HSV transform.
//! The bitmap stays unchanged for intensity = 20, offset = 128, exponent = 1.
class PLFilterIntensity : public PLFilter  
{
public:
	PLFilterIntensity (double intensity, PLBYTE offset, double exponent);
	virtual ~PLFilterIntensity();
  virtual void Apply(PLBmp *pBmpSource, PLBmp* pBmpDest) const;

protected:
  double m_intensity;
  double m_offset;
  double m_exponent;
};
#endif

/*
/--------------------------------------------------------------------
|
|      $Log: plfilterintensity.h,v $
|      Revision 1.1  2009/12/09 05:33:41  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:09  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:52  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:47  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:01  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:17  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:21  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:22  황정영
|      2006.05.17
|
|      Revision 1.3  2002/02/24 13:00:47  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 22:03:26  uzadow
|      Added PL prefix to basic data types.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.2  2001/01/15 15:05:31  uzadow
|      Added PLBmp::ApplyFilter() and PLBmp::CreateFilteredCopy()
|
|      Revision 1.1  2000/11/06 23:20:22  uzadow
|      Added Contrast, Intensity and Lightness filters by
|      Thomas Hirschmann
|
|
\--------------------------------------------------------------------
*/
