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
|      $Id: plcontribdefs.h,v 1.1 2009/12/09 05:33:38 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_CONTRIBDEFS
#define INCL_CONTRIBDEFS

#include <math.h>

class PLContribDef
{
public:

    PLContribDef (double dWidth) : m_dWidth (dWidth) {}
    virtual ~PLContribDef() {}

    double GetWidth() const             { return m_dWidth; }
    void   SetWidth (double dWidth)     { m_dWidth = dWidth; }

    virtual double Filter (double dVal) const = 0;

protected:

    #define FILTER_PI  double (3.1415926535897932384626433832795)
    #define FILTER_2PI double (2.0 * 3.1415926535897932384626433832795)
    #define FILTER_4PI double (4.0 * 3.1415926535897932384626433832795)

    double  m_dWidth;
};

class PLBoxContribDef : public PLContribDef
{
public:

    PLBoxContribDef (double dWidth = 0.5) : PLContribDef(dWidth) {}
    virtual ~PLBoxContribDef() {}

    virtual double Filter (double dVal) const { return (fabs(dVal) <= m_dWidth ? 1.0 : 0.0); }
};

class PLBilinearContribDef : public PLContribDef
{
public:

    PLBilinearContribDef (double dWidth = 1.0) : PLContribDef(dWidth) {}
    virtual ~PLBilinearContribDef() {}

    virtual double Filter (double dVal) const
    {
        dVal = fabs(dVal);
        return (dVal < m_dWidth ? m_dWidth - dVal : 0.0);
    }
};

class PLGaussianContribDef : public PLContribDef
{
public:

    PLGaussianContribDef (double dWidth = 3.0) : PLContribDef(dWidth) {}
    virtual ~PLGaussianContribDef() {}

    virtual double Filter (double dVal) const
        {
            if (fabs (dVal) > m_dWidth)
            {
                return 0.0;
            }
            return exp (-dVal * dVal / m_dWidth-1) / sqrt (FILTER_2PI);
        }
};

class PLHammingContribDef : public PLContribDef
{
public:

    PLHammingContribDef (double dWidth = 0.5) : PLContribDef(dWidth) {}
    virtual ~PLHammingContribDef() {}

    virtual double Filter (double dVal) const
        {
            if (fabs (dVal) > m_dWidth)
            {
                return 0.0;
            }
            double dWindow = 0.54 + 0.46 * cos (FILTER_2PI * dVal);
            double dSinc = (dVal == 0) ? 1.0 : sin (FILTER_PI * dVal) / (FILTER_PI * dVal);
            return dWindow * dSinc;
        }
};

class PLBlackmanContribDef : public PLContribDef
{
public:

    PLBlackmanContribDef (double dWidth = 0.5) : PLContribDef(dWidth) {}
    virtual ~PLBlackmanContribDef() {}

    virtual double Filter (double dVal) const
        {
            if (fabs (dVal) > m_dWidth)
            {
                return 0.0;
            }
            double dN = 2.0 * m_dWidth + 1.0;
            return 0.42 + 0.5 * cos (FILTER_2PI * dVal / ( dN - 1.0 )) +
                   0.08 * cos (FILTER_4PI * dVal / ( dN - 1.0 ));
        }
};


#endif  // _FILTERS_H_

/*
/--------------------------------------------------------------------
|
|      $Log: plcontribdefs.h,v $
|      Revision 1.1  2009/12/09 05:33:38  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:07  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:49  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:46  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:30:24  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:14  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:18  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:21  황정영
|      2006.05.17
|
|      Revision 1.3  2002/11/04 22:40:13  uzadow
|      Updated for gcc 3.1
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/30 16:58:23  uzadow
|      Improved speed of 2passfilter.h, code readability changes.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.2  1999/12/02 17:07:34  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.1  1999/10/21 16:05:17  Ulrich von Zadow
|      Moved filters to separate directory. Added Crop, Grayscale and
|      GetAlpha filters.
|
|      Revision 1.1  1999/10/19 21:29:55  Ulrich von Zadow
|      Added filters.
|
|
\--------------------------------------------------------------------
*/
