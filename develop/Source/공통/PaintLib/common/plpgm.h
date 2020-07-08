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
|      Jose Miguel Buenaposada Biencinto. Mar 2000.
|
\--------------------------------------------------------------------
*/

//! Defines structures and constants present in PGM files.

#ifndef INCL_PLPGM
#define INCL_PLPGM

typedef struct _PgmHeader
{
    int ImageType;           // Image Type
    int ImageWidth;          // Image Width
    int ImageHeight;         // Image Height
	int MaxGrayValue;  

} PGMHEADER;

#define PGM_MAXLINESIZE 80  // Maximum number of characters per line

// Definitions for image types.
#define PGM_P2 0
#define PGM_P5 1

#endif
