/*
|
|      $Id: plpicenc.cpp,v 1.1 2009/12/09 05:33:54 박종운 Exp $
|      Generic Picture Encoder Class
|
|      Abstract base class to dump picture data to memory or file.
|      Classes derived from this class implement concrete encoders
|      for specific file formats.
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

// not quite ready for prime-time; bdelmee; 2/99

#include "plstdpch.h"
#include "plpicenc.h"
#include "plfilesink.h"
#include "plexcept.h"
// only for the tracing facility
#include "plpicdec.h"

#include <errno.h>

PLPicEncoder::PLPicEncoder()
  : PLObject()
// Creates an encoder
{}


PLPicEncoder::~PLPicEncoder()
{}


// Encodes a picture by creating a file data sink and
// calling SaveBmp with this data sink.

void PLPicEncoder::MakeFileFromBmp (const char * pszFName, PLBmp * pBmp)
{
  PLFileSink FileSink;
  int err;

  char sz[1024];
  sprintf (sz, "--- Encoding file %s. ---\n", pszFName);
  Trace (1, sz);

  // We allocate a buffer large enough to hold the raw bitmap
  // plus some overhead.In most cases this should be enough to
  // hold the uncompressed data in any format, plus headers, etc...
  // Some "pathological" cases however may end up with a CODEC
  // producing more data than the uncompressed version!
  int bufsize = pBmp->GetMemUsed();
  bufsize = bufsize < 20000 ? bufsize + 4096 : int(1.2 * bufsize);
  err = FileSink.Open( pszFName, bufsize );
  if (err)
  {
    sprintf (sz, "Opening %s failed: %s", pszFName, strerror(errno));
    raiseError (err, sz);
  }

  SaveBmp ( pBmp, &FileSink );
  FileSink.Close ();
}

// Encodes a picture by getting the encoded data from pDataSrc.
// Saves the results to pDataSrc
// Actually, a wrapper so thin around "DoEncode", you could see through...
void PLPicEncoder::SaveBmp (PLBmp* pBmp, PLDataSink* pDataSnk)
{
  DoEncode( pBmp, pDataSnk );
}

////////////////////
// As long as the tracing code lives in the base decoder,
// we'll just forward everything to it without bothering the user

void PLPicEncoder::SetTraceConfig( int Level, char * pszFName )
{
  PLPicDecoder::SetTraceConfig( Level, pszFName );
}

void PLPicEncoder::raiseError( int Code, char * pszErr )
{
  PLPicDecoder::raiseError( Code, pszErr );
}

void PLPicEncoder::Trace( int TraceLevel, const char * pszMessage )
{
  PLPicDecoder::Trace( TraceLevel, pszMessage );
}
/*
/--------------------------------------------------------------------
|
|      $Log: plpicenc.cpp,v $
|      Revision 1.1  2009/12/09 05:33:54  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:17  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:00  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:56  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:05  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:24  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:32  황정영
|      2006.05.17
|
|      Revision 1.5  2002/08/05 20:43:12  uzadow
|      cygwin compatibility changes
|
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/02/24 13:00:24  uzadow
|      Documentation update; removed buggy PLFilterRotate.
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.8  2001/02/04 14:31:52  uzadow
|      Member initialization list cleanup (Erik Hoffmann).
|
|      Revision 1.7  2001/02/04 14:07:24  uzadow
|      Changed max. filename length.
|
|      Revision 1.6  2000/01/16 20:43:14  anonymous
|      Removed MFC dependencies
|
|      Revision 1.5  2000/01/10 23:52:59  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.4  2000/01/08 15:51:30  Ulrich von Zadow
|      Misc. modifications to png encoder.
|
|      Revision 1.3  1999/10/03 18:50:51  Ulrich von Zadow
|      Added automatic logging of changes.
|
|
--------------------------------------------------------------------
*/
