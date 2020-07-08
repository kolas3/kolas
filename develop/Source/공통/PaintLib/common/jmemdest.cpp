/*
 * jmemdest.cpp
 *
 * This file contains a compression data destination which takes a
 * memory region as destination.
 * The only thing these routines really do is tell the library where the
 * data is on construction (jpeg_mem_dest()). Everything else is there
 * for error checking purposes.
 * Adapted from jdatasrc.c 9/96 by Ulrich von Zadow.
 * Adapted from jmemsrc.c 3.6.99 by MS
 */

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */

#include "plstdpch.h"
#include "pldatasink.h"

#include <stdio.h>

#include "jpeglib.h"
#include "jerror.h"
#include "jmemdest.h"

#include <stdio.h>

#define JPEG_WORK_BUFFER_SIZE 8192      // 8KB Work Buffer

typedef struct
{
  struct jpeg_destination_mgr pub;       // public fields

  PLDataSink * pDataSink ;                // Data Sink
  JOCTET * buffer;
}
datasink_dest_mgr;



/*
 * Initialize destination
 * before any data is actually written.
 */

METHODDEF(void)
jpeg_mem_dest_init (j_compress_ptr cinfo)
{
  /* Set up data pointer */
  datasink_dest_mgr *  dest = (datasink_dest_mgr * ) cinfo->dest;

  /* Allocate the output buffer --- it will be released when done with image */
  dest->buffer = (JOCTET *)
                 (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
                                             JPEG_WORK_BUFFER_SIZE);

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = JPEG_WORK_BUFFER_SIZE;
}

/*
 * Flush the output buffer --- called whenever buffer is full.
 *
 * If this procedure gets called, we have a buffer overrun condition -
 * We now send the data to the DataSink
 */

METHODDEF(boolean)
jpeg_mem_dest_empty_output_buffer (j_compress_ptr cinfo)
{
  datasink_dest_mgr * dest = (datasink_dest_mgr *) cinfo->dest;

  if (dest->pDataSink->WriteNBytes(JPEG_WORK_BUFFER_SIZE, dest->buffer) !=
      (size_t) JPEG_WORK_BUFFER_SIZE)
    ERREXIT(cinfo, JERR_FILE_WRITE);

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = JPEG_WORK_BUFFER_SIZE;

  return true;
}


/*
 * Terminate source --- called by jpeg_finish_compress
 * after all data has been written.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void)
jpeg_mem_dest_term (j_compress_ptr cinfo)
{
  datasink_dest_mgr * dest = (datasink_dest_mgr *) cinfo->dest;
  size_t datacount = JPEG_WORK_BUFFER_SIZE - dest->pub.free_in_buffer;

  /* Write any data remaining in the buffer */
  if (datacount > 0)
  {
    if (dest->pDataSink->WriteNBytes(datacount, dest->buffer) != datacount)
      ERREXIT(cinfo, JERR_FILE_WRITE);
  }

}


/*
 * Prepare for ouput. This routine tells the jpeg library where to find
 * the output buffer & sets up the function pointers the library needs.
 *
 */

GLOBAL(void)
jpeg_mem_dest(
  j_compress_ptr cinfo,
  JOCTET * pData,
  int FileSize,
  PLDataSink * pDataSink
)
{
  datasink_dest_mgr * dest;

  if (cinfo->dest == NULL)
  {   /* first time for this JPEG object? */
    cinfo->dest = (struct jpeg_destination_mgr *)
                  (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                              sizeof(datasink_dest_mgr));
  }

  dest = (datasink_dest_mgr *) cinfo->dest;
  dest->pub.init_destination = jpeg_mem_dest_init;
  dest->pub.empty_output_buffer = jpeg_mem_dest_empty_output_buffer;
  dest->pub.term_destination = jpeg_mem_dest_term;
  dest->pDataSink = pDataSink;
}
