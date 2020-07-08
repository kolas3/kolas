/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jul 20 16:18:55 2001
 */
/* Compiler settings for C:\_work\sms\sms.idl:
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
    DWORD x;
    unsigned short s1;
    unsigned short s2;
    _TUCHAR  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ITcp = {0xDF06E9A7,0x44E4,0x4B59,{0x92,0x26,0x9E,0xC9,0xAD,0xE8,0x4A,0x50}};


const IID IID_IWTCP = {0x290E03C0,0xFD49,0x4E47,{0x9A,0xB3,0x1D,0x85,0x18,0x09,0xFF,0xD0}};


const IID LIBID_SMSLib = {0x5C37C91B,0x559F,0x47E1,{0x9C,0x06,0x90,0x12,0x97,0x4E,0xBE,0xDA}};


const CLSID CLSID_Tcp = {0x1005C628,0x391C,0x41AF,{0x90,0x44,0x91,0x28,0x61,0x9E,0xCB,0x6C}};


const CLSID CLSID_WTCP = {0x1D045036,0x1B02,0x422A,{0xA4,0x5F,0xA6,0x71,0xAF,0x40,0xF5,0xA6}};


#ifdef __cplusplus
}
#endif