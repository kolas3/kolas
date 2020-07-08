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
//! Defines a custom data source for LIBJPEG
GLOBAL(void)
jpeg_mem_src (j_decompress_ptr cinfo,
              JOCTET * pData,
              int FileSize,
              void *pDataSrc,
              JMETHOD(void, notifyCppWorld, (j_common_ptr)));
              // Jo Hagelberg 15.4.99: added for notification callback

/*
/--------------------------------------------------------------------
|
|      $Log: jmemsrc.h,v $
|      Revision 1.1  2009/12/09 05:33:46  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:12  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:01:55  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:52  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:02  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:19  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:23  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:29  황정영
|      2006.05.17
|
|      Revision 1.6  2000/10/30 21:45:04  uzadow
|      Added again after deleting the file by mistake.
|
|      Revision 1.4  2000/10/26 21:06:17  uzadow
|      Removed dependency on jpegint.h
|
|
\--------------------------------------------------------------------
*/
