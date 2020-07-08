/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Oct 21 17:37:04 2019
 */
/* Compiler settings for PaintX.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IPictureDecoder = {0xB69FE5D2,0x9F53,0x11D2,{0xB1,0x3F,0x44,0x45,0x53,0x54,0x00,0x00}};


const IID LIBID_PAINTXLib = {0xB69FE5C4,0x9F53,0x11D2,{0xB1,0x3F,0x44,0x45,0x53,0x54,0x00,0x00}};


const CLSID CLSID_PictureDecoder = {0xB69FE5D3,0x9F53,0x11D2,{0xB1,0x3F,0x44,0x45,0x53,0x54,0x00,0x00}};


#ifdef __cplusplus
}
#endif

