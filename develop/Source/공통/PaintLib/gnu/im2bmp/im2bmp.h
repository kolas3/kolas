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
//----------------------------------------------------------------
//
// FILE   : im2bmp.h
// AUTHOR : Jose Miguel Buenaposada. (jmbuena@dia.fi.upm.es)     
//
//----------------------------------------------------------------


//-------------- RECURSION PROTECTION ----------------------------
#ifndef _IM2BMP_H_
#define _IM2BMP_H_

//--------------------- INCLUDE ----------------------------------
#include <iostream>
#include <string>
#include <fstream>
#include "plstdpch.h"    // Paintlib
#include "planybmp.h"    // Paintlib
#include "planydec.h"    // Paintlib
#include "plbmpenc.h"   // Paintlib

//------------------ DEFINITION ----------------------------------
void printusemessage();
void parseargs(int nargs, char** args);

#endif
