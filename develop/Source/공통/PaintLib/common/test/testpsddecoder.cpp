/*
/--------------------------------------------------------------------
|
|      $Id: testpsddecoder.cpp,v 1.1 2009/12/09 05:34:03 박종운 Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#include "testpsddecoder.h"
#include "pltester.h"

#include "plpsddec.h"
#include "planybmp.h"
#include "plpngenc.h"
#include <string>

using namespace std;

#ifdef WIN32
const std::string sTestFileDir ("..\\..\\testpic\\");
#else
const std::string sTestFileDir ("testpic/");
#endif

PLTestPSDDecoder::PLTestPSDDecoder ()
{

}

PLTestPSDDecoder::~PLTestPSDDecoder ()
{
}

void PLTestPSDDecoder::RunTests ()
{
  test (string ("rgb8.psd"));
  test (string ("rgb8alpha.psd"));
  test (string ("rgb24.psd"));
  test (string ("rgb24alpha.psd"));
  test (string ("rgb24alpha2.psd"));
  test (string ("rgb24layers.psd"));
}

void PLTestPSDDecoder::test (const string& sFName)
{
  PLPSDDecoder Decoder;
  PLAnyBmp Bmp;
  string sFilePos = sTestFileDir+sFName;
  PLTRACE ("  Testing %s...\n", sFilePos.c_str());
  PLTRACE ("    Testing MakeBmpFromFile...\n");
  Decoder.MakeBmpFromFile (sFilePos.c_str(), &Bmp);

  {
    PLPSDDecoder Decoder;
//    PLPNGEncoder Encoder;
    PLTRACE ("    Testing OpenFile...\n");
    Decoder.OpenFile (sFilePos.c_str());
    int n = Decoder.GetNumLayers ();
    for (int i = 0; i<n; i++)
    {
      PLAnyBmp LayerBmp;
      PLTRACE ("    Testing GetNextLayer...\n");
      Decoder.GetNextLayer (LayerBmp);
      PLPoint Pt = Decoder.GetLayerOffset ();
/*
      char sz[256];
      sprintf (sz, "c:\\test%i.png", i);
      Encoder.MakeFileFromBmp (sz, &LayerBmp);
*/
    }
    PLAnyBmp FlatBmp;
    FlatBmp.Create (Decoder);
    PLTRACE ("    Testing GetImage...\n");
    Decoder.GetImage (FlatBmp);
    PLASSERT (FlatBmp == Bmp);
    Decoder.Close ();
  }
}

/*
/--------------------------------------------------------------------
|
|      $Log: testpsddecoder.cpp,v $
|      Revision 1.1  2009/12/09 05:34:03  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:49  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:13  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:03  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:08  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:29  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.9  2002/11/18 14:45:51  uzadow
|      Added PNG compression support, fixed docs.
|
|      Revision 1.8  2002/10/20 22:11:53  uzadow
|      Partial doc update.
|      Fixed tests when images were not available.
|
|      Revision 1.7  2002/08/07 20:39:54  uzadow
|      no message
|
|      Revision 1.6  2002/08/07 20:32:31  uzadow
|      Updated Makefile.in, fixed psd decoder layer stuff.
|
|      Revision 1.5  2002/08/07 18:50:39  uzadow
|      More cygwin compatibility changes.
|
|      Revision 1.4  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.3  2002/01/27 18:20:18  uzadow
|      Updated copyright message; corrected pcx decoder bug.
|
|      Revision 1.2  2001/10/21 17:54:40  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/10/21 17:12:40  uzadow
|      Added PSD decoder beta, removed BPPWanted from all decoders, added PLFilterPixel.
|
|
\--------------------------------------------------------------------
*/

