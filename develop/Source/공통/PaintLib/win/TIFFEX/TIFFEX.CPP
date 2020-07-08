/*
/--------------------------------------------------------------------
|
|      $Id: TIFFEX.CPP,v 1.1 2009/12/09 05:34:39 박종운 Exp $
|
|      test TIFF encoding support in PaintLib
|      bdelmee; 2/99
|
\--------------------------------------------------------------------
*/

#include "plstdpch.h"

//  define 'EXTENDED_TEST' to see CTiffDecoderEx and CTiffEncoderEx in action
#define EXTENDED_TEST 1

#include "planybmp.h"
#include "pltiffdec.h"
#include "pltiffencex.h"

#ifdef  EXTENDED_TEST
#   include "plfilesrc.h"
#   include "plfilesink.h"
#endif


int main( int argc, char* argv[] )
{
  PLAnyBmp      bmp;
#ifdef _DEBUG
  PLPicDecoder::SetTraceConfig( 3, NULL );
#endif

  if (argc < 2)
  {
    fputs( "Usage: tiffex 'filename'\n", stderr );
    return -1;
  }

#ifndef EXTENDED_TEST

  PLTIFFDecoder  td;
  // decode image
  td.MakeBmpFromFile( argv[1], &bmp );

#else   // test tag reading capability

  PLTIFFDecoder  myTD;
  myTD.OpenFile( argv[1] );

  int i;
  /*
  uint32 w, h;
  i = myTD.GetField(TIFFTAG_IMAGEWIDTH,&w);
  i = myTD.GetField(TIFFTAG_IMAGELENGTH,&h);
  i = myTD.GetField(TIFFTAG_MAKE,&cp);
  i = myTD.GetField(TIFFTAG_MODEL,&cp);
  */
  char*  imgdsc = NULL;
  char*  author = NULL;
  i = myTD.GetField(TIFFTAG_IMAGEDESCRIPTION,&imgdsc);
  i = myTD.GetField(TIFFTAG_ARTIST,&author);

  myTD.GetImage (bmp);
#endif

  // ASSERT( bmp.GetBitsPerPixel() == 1 );

  // build output file name
  char outname[80];
  char* cp = strrchr( argv[1], '.' );
  strcpy( outname, argv[1] );
  if (cp)
  {
    int n = cp - argv[1];
    strcpy( outname + n, "_bis" );
    strcat( outname + n + 4, cp );
  }
  else
    strcat( outname, "_bis" );

  // now re-encode
#ifndef EXTENDED_TEST

  PLTIFFEncoder  te;
  te.MakeFileFromBmp( outname, &bmp );

#else   // test tag writing capability

  PLTIFFEncoderEx  tex;
  uint32 ui32;
  uint16 ui16;
  PLFileSink outfile;
  outfile.Open( outname, bmp.GetMemUsed() + 4096 );
  tex.Associate( &outfile );

  // mandatory 'base_line' tags
  tex.SetBaseTags( &bmp );
  ui32 = bmp.GetHeight();
  // one strip = the whole image (no strip, no tile)
  tex.SetField( TIFFTAG_ROWSPERSTRIP,    ui32 );
  // select compression scheme
  ui16 = bmp.GetBitsPerPixel() == 1 ?
         COMPRESSION_CCITTFAX4 : COMPRESSION_PACKBITS;
  tex.SetField( TIFFTAG_COMPRESSION,    ui16 );
  tex.SetField( TIFFTAG_DOCUMENTNAME, outname );
  tex.SetField( TIFFTAG_SOFTWARE, "paintlib test encoder" );
  // original artist or tired hacker ;-)
  tex.SetField( TIFFTAG_ARTIST, author ? author : "bdelmee" );
  if (imgdsc && *imgdsc)
    tex.SetField( TIFFTAG_IMAGEDESCRIPTION, imgdsc );

  tex.SaveBmp( &bmp, &outfile );
  // only really needed if you encode several bitmaps
  tex.Dissociate();
  // also implicit in destructor
  outfile.Close();

#endif

  return 0;
}
/*
/--------------------------------------------------------------------
|
|      $Log: TIFFEX.CPP,v $
|      Revision 1.1  2009/12/09 05:34:39  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:03  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:36  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:16  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:25  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:40  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:56  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:07  황정영
|      2006.05.17
|
|      Revision 1.9  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.8  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.7  2000/12/02 19:41:42  uzadow
|      no message
|
|      Revision 1.6  2000/01/10 23:53:00  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.5  2000/01/08 15:56:12  Ulrich von Zadow
|      Made sure change logging works in every file.
|
|
\--------------------------------------------------------------------
*/
