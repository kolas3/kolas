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
|      $Id: grarray.h,v 1.1 2009/12/09 05:35:03 박종운 Exp $
|      Array of CGrItems
|
|      Contains an array of graphic items sorted by z-order.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_GRARRAY
#define INCL_GRARRAY

#include "gritem.h"

class CGrItemArray : public CObArray
{

DECLARE_DYNAMIC (CGrItemArray);
// Construction
public:
  CGrItemArray
    ();

  virtual ~CGrItemArray
    ();
    // Deletes the array & all elements in it.

  CGrItem * GetItem
    ( int i
    );
    // Returns item at the i'th position in the array.

  void Empty
    ();
    // Destroys all elements in the array.

  virtual int AddItem
    ( CGrItem * pItem,
      int Pos = -1
    );
    // Type-safe version of CObArray::Add. If the parameter Pos is
    // omitted, the element is inserted in z-Order, otherwise at
    // position Pos. (Specifying Pos can lead to an unsorted array
    // and should only be used when you're inserting in sorted order
    // anyway.)

  void Draw
    ( PLBmp * pCanvas,
      CRect * pRect
    );
    // Draws all the elements on the canvas by invoking their Draw()-
    // methods.

  void GetRect
    ( CRect * pRect
    );
    // Returns smallest rectangle containing all the elements.

private:
  int sortedInsert
    ( CGrItem * pItem
    );
};

#endif

/*
/--------------------------------------------------------------------
|
|      $Log: grarray.h,v $
|      Revision 1.1  2009/12/09 05:35:03  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:15  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:27  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:51  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:36  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:56  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:03  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:13  황정영
|      2006.05.17
|
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
--------------------------------------------------------------------
*/
