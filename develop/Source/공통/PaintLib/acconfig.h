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
@BOTTOM@

// Byte order in a 32 bit word
#ifdef WORDS_BIGENDIAN
#define MSB_FIRST  // Motorola et al.
#else
#define LSB_FIRST // Intel et al.
#endif

#undef PL_SUPPORT_TGA
#undef PL_SUPPORT_BMP
#undef PL_SUPPORT_TIFF
#undef PL_SUPPORT_PICT
#undef PL_SUPPORT_JPEG
#undef PL_SUPPORT_PNG
#undef PL_SUPPORT_GIF
#ifdef _WINDOWS
  #define PL_SUPPORT_WEMF
#endif
#undef PL_SUPPORT_PCX
#undef PL_SUPPORT_PGM
#undef PL_SUPPORT_PPM
#undef PL_SUPPORT_PSD

// The following lines can be used to define maximum memory useage
// for a single BITMAP. If we need more, we assume something went wrong.
// If MAX_BITMAP_SIZE isn't defined, the code doesn't check for huge
// bitmaps and just tries to allocate the memory. This can take a
// long time.
// #ifndef MAX_BITMAP_SIZE
// #define MAX_BITMAP_SIZE 8L*1024*1024
// #endif

