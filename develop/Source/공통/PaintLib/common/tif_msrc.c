/* $Header: /K2UP_CVS/Repository/KOLAS III/develop/Source/공통/PaintLib/common/tif_msrc.c,v 1.1 2009/12/09 05:33:59 박종운 Exp $ */

/*
 * Copyright (c) 1988-1996 Sam Leffler
 * Copyright (c) 1991-1996 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

/* Adapted to TIFFs in memory regions by U.v.Zadow */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tif_msrc.h"
#ifdef _WIN32
#include <crtdbg.h>
#endif

// in write mode, we must record the highest written to offset
#define CHECKNEWSIZE \
	if (*pInfo->mode == 'w' && pInfo->CurPos > *pInfo->pFileSize) \
		*pInfo->pFileSize = pInfo->CurPos


static tsize_t
_tiffReadProc(thandle_t fd, tdata_t buf, tsize_t size)

{
	MemSrcTIFFInfo * pInfo = (MemSrcTIFFInfo *) fd;

#ifdef _WINDOWS
	//_ASSERT( *(pInfo->mode) == 'r' );
#endif
	// even when writing, libtiff initially tries to read a directory....
	if (*(pInfo->mode) != 'r')
		return 0;

	/* Make sure we don't run over the end of the file */
	if (size+pInfo->CurPos > pInfo->MaxFileSize)
		size = pInfo->MaxFileSize-pInfo->CurPos;

	memcpy ((void *)buf, (void *)(pInfo->pData+pInfo->CurPos), size);
	pInfo->CurPos += size;
	CHECKNEWSIZE;

	return (tsize_t) size;
}

static tsize_t
_tiffWriteProc(thandle_t fd, tdata_t buf, tsize_t size)
{
#if 0
	TIFFError ("TIFFOpen", "Function disabled.");
	return 0;
#else
	MemSrcTIFFInfo * pInfo = (MemSrcTIFFInfo *) fd;

#ifdef _WINDOWS
	_ASSERT( *(pInfo->mode) == 'w' );
#endif

	/* Make sure we don't run over the end of the file */
	if (size+pInfo->CurPos > pInfo->MaxFileSize)
		size = pInfo->MaxFileSize-pInfo->CurPos;

	memcpy ((void *)(pInfo->pData+pInfo->CurPos), (void *)buf, size);
	pInfo->CurPos += size;
	CHECKNEWSIZE;

	return (tsize_t) size;
#endif
}

static toff_t
_tiffSeekProc(thandle_t fd, toff_t off, int whence)
{
	MemSrcTIFFInfo * pInfo = (MemSrcTIFFInfo *) fd;

	switch(whence)
	{
		case 1:
			pInfo->CurPos += off;
			break;
		case 2:
			pInfo->CurPos = *pInfo->pFileSize - off;
			break;
		case 0:
		default:
			pInfo->CurPos = off;
			break;
	}
	CHECKNEWSIZE;
	return pInfo->CurPos;
}

static int
_tiffCloseProc(thandle_t fd)
{
	MemSrcTIFFInfo * pInfo = (MemSrcTIFFInfo *) fd;
	free (pInfo);

	return 0;
}

static toff_t
_tiffSizeProc(thandle_t fd)
{
	MemSrcTIFFInfo * pInfo = (MemSrcTIFFInfo *) fd;
	return ((toff_t)(*pInfo->pFileSize));
}

static int
_tiffDummyMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	return(0);
}

static void
_tiffDummyUnmapProc(thandle_t fd, tdata_t base, toff_t size)
{}


TIFF*
TIFFFdOpen(int ifd, const char* name, const char* mode)
{
	TIFFError ("TIFFFdOpen", "Function disabled.");
	return NULL;
}

TIFF*
TIFFOpen(const char* name, const char* mode)
{
	/* This version doesn't work with files */
	TIFFError ("TIFFOpen", "Function disabled.");
	return NULL;
}

TIFF*
TIFFOpenMem (unsigned char * pData, 
			 int maxFileSize, 
			 int *fSize )
{
	TIFF* tif;
	MemSrcTIFFInfo * pInfo;

	pInfo = (MemSrcTIFFInfo *) malloc (sizeof (MemSrcTIFFInfo));
	pInfo->pData = pData;
	strcpy( pInfo->mode, fSize ? "w" : "r" ); 
	// register extern int
	pInfo->pFileSize = fSize ? fSize : &(pInfo->rFileSize);
	pInfo->MaxFileSize = maxFileSize;
	*pInfo->pFileSize  = fSize ? 0 : maxFileSize;
	pInfo->CurPos = 0;

	tif = TIFFClientOpen( "MemSource", pInfo->mode, (thandle_t)pInfo,
						  _tiffReadProc, _tiffWriteProc,
						  _tiffSeekProc, _tiffCloseProc, _tiffSizeProc,
						  _tiffDummyMapProc, _tiffDummyUnmapProc);
	return (tif);
}

tdata_t
_TIFFmalloc(tsize_t s)
{
	return ((tdata_t)malloc (s));
}

void
_TIFFfree(tdata_t p)
{
	free (p);
}

tdata_t
_TIFFrealloc(tdata_t p, tsize_t s)
{
	return realloc (p, s);
}

void
_TIFFmemset(void* p, int v, tsize_t c)
{
	memset(p, v, c);
}

void
_TIFFmemcpy(void* d, const tdata_t s, tsize_t c)
{
	memcpy (d, s, c);
}

int
_TIFFmemcmp(const tdata_t p1, const tdata_t p2, tsize_t c)
{
	return memcmp (p1, p2, c);
}

TIFFErrorHandler _TIFFerrorHandler = NULL;
TIFFErrorHandler _TIFFwarningHandler = NULL;
/*
/--------------------------------------------------------------------
|
|      $Log: tif_msrc.c,v $
|      Revision 1.1  2009/12/09 05:33:59  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:47  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:08  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:01  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:07  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:27  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:30:34  황정영
|      2006.05.17
|
|      Revision 1.4  2002/08/07 18:33:51  uzadow
|      Removed dependency on internal libtiff header tiffiop.h
|
|      Revision 1.3  1999/12/08 15:39:46  Ulrich von Zadow
|      Unix compatibility changes
|
|
--------------------------------------------------------------------
*/
