/*
 * jmemsrc.c
 *
 * This file contains a decompression data source which takes a 
 * memory region as source. 
 * The only thing these routines really do is tell the library where the 
 * data is on construction (jpeg_mem_src()). Everything else is there 
 * for error checking purposes.
 * Adapted from jdatasrc.c 9/96 by Ulrich von Zadow.
 */
 
/* this is not a core library module, so it doesn't define JPEG_INTERNALS */

#include "plstdpch.h"

#include <stdio.h>

extern "C"
{
#include "jpeglib.h"
#include "jerror.h"
}
#include "jmemsrc.h"

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
}

/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * If this procedure gets called, we have a buffer overrun condition - 
 * there is not enough data in the buffer to satisfy the decoder.
 * The procedure just generates a warning and feeds the decoder a fake 
 * JPEG_EOI marker.
 */

METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
  struct jpeg_source_mgr * src = cinfo->src;
  static JOCTET FakeEOI[] = { 0xFF, JPEG_EOI };

  /* Generate warning */
  WARNMS(cinfo, JWRN_JPEG_EOF);
  
  /* Insert a fake EOI marker */
  src->next_input_byte = FakeEOI;
  src->bytes_in_buffer = 2;

  return TRUE;
}


METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  struct jpeg_source_mgr * src = cinfo->src;
  
  if(num_bytes >= (long)src->bytes_in_buffer)
  {
    fill_input_buffer(cinfo);
    return;
  }

  src->bytes_in_buffer -= num_bytes;
  src->next_input_byte += num_bytes;
}


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void)
term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}


/*
 * Prepare for input. This routine tells the jpeg library where to find 
 * the data & sets up the function pointers the library needs.
 *
 * Jo Hagelberg 15.4.99
 * added pDataSrc and JMETHOD( notifyCppworld ) for progress notification 
 */

GLOBAL(void)
jpeg_mem_src (
              j_decompress_ptr cinfo,
              JOCTET * pData,
              int FileSize,
              void *pDataSrc,
              JMETHOD(void, notifyCppWorld, (j_common_ptr))
             )
{
  struct jpeg_source_mgr * src;

  if (cinfo->src == NULL)
  {   /* first time for this JPEG object? */
      cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                            sizeof(struct jpeg_source_mgr));
  }

  /* Jo Hagelberg 15.4.99               
   * added progress notification for JPEG 
   */
  if ( cinfo->progress == NULL)
  {   /* first time and progress notification wanted? */
      cinfo->progress = (struct jpeg_progress_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                            sizeof(struct jpeg_progress_mgr));
      /* static callback function in JpegDecoder */
      cinfo->progress->progress_monitor = notifyCppWorld;
  }

  /* Jo Hagelberg 15.4.99               
   * put CDataSource instance ptr into client data, so we can use it in the callback
   * NOTE: define a client data struct if this should be needed for other stuff, too
   */
  cinfo->client_data = pDataSrc ;

  src = cinfo->src;

  /* Set up function pointers */
  src->init_source = init_source;
  src->fill_input_buffer = fill_input_buffer;
  src->skip_input_data = skip_input_data;
  src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->term_source = term_source;

  /* Set up data pointer */
  src->bytes_in_buffer = FileSize;
  src->next_input_byte = pData;
}
/*
/--------------------------------------------------------------------
|
|      $Log: jmemsrc.cpp,v $
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
|      Revision 1.3  2001/09/16 20:57:17  uzadow
|      Linux version name prefix changes
|
|      Revision 1.2  2001/09/15 21:02:44  uzadow
|      Cleaned up stdpch.h and config.h to make them internal headers.
|
|      Revision 1.1  2000/10/30 14:32:50  uzadow
|      Removed dependency on jinclude.h
|
|      Revision 1.5  2000/09/01 13:27:07  Administrator
|      Minor bugfixes
|
|
\--------------------------------------------------------------------
*/
