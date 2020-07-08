// Document modified at : Monday, June 10, 2002 01:17:48 PM , by user : Administrator , from computer : GJPARK
//////////////////////////////////////////////////////////////////////
/*
 Smtp.cpp: implementation of the CSmtp and CSmtpMessage classes

 Written by Robert Simpson (robert@blackcastlesoft.com)
 Created 11/1/2000
 Version 1.7 -- Last Modified 06/18/2001

 1.7 - Modified the code that gets the GMT offset and the code that
       parses the date/time as per Noa Karsten's suggestions on 
       codeguru.
     - Added an FD_ZERO(&set) to the last part of SendCmd(), since
       I use the set twice and only zero it out once.  Submitted by
       Marc Allen.
     - Removed the requirement that a message have a body and/or an
       attachment.  This allows for sending messages with only a
       subject line.  Submitted by Marc Allen.
 1.6 - Apparently older versions of the STL do not have the clear()
       method for basic_string's.  I modified the code to use 
       erase() instead.
     - Added #include <atlbase.h> to the smtp.h file, which will
       allow any app to use these classes without problems.
 1.5 - Guess I should have checked EncodeQuotedPrintable() as well,
       since it did the same thing BreakMessage() did in adding an
       extranneous CRLF to the end of any text it processed.  Fixed.
 1.4 - BreakMesage() added an extranneous CRLF to the end of any
       text it processed, which is now fixed.  Certainly not a big
       deal, but it caused text attachments to not be 100% identical
       to the original.
 1.3 - Added a new class, CSmtpMimePart, to which the CSmtpAttachment
       and CSmtpMessageBody classes inherit.  This was done for
       future expansion.  CSmtpMimePart has a new ContentId string
       value for optionally assigning a unique content ID value to
       body parts and attachments.  This was done to support the
       multipart/related enhancement
     - Support for multipart/related messages, which can be used
       for sending html messages with embedded images.
     - Modifed CSmtpMessage, adding a new MimeType member variable
       so the user can specify a certain type of MIME format to use
       when coding the message.
     - Fixed a bug where multipart/alternative messages with multiple
       message bodies were not properly processed when attachments
       were also included in the message.
     - Some small optimizations during the CSmtpMessage::Parse routine

 1.2 - Vastly improved the time it takes to break a message,
       which was dog slow with large attachments.  My bad.
     - Added another overridable, SmtpProgress() which is
       called during the CSmtp::SendCmd() function when there
       is a large quantity of data being sent over the wire.
       Added CMD_BLOCK_SIZE to support the above new feature
     - Added support for UNICODE
     - Added the CSmtpAttachment class for better control and
       expandability for attachments.
     - Added alternative implementations for CSmtp::SendMessage
       which make it easier to send simple messages via a single
       function call.
     - Added a constructor to CSmtpAddress for assigning default
       values during initialization.
     - Added a #pragma comment(lib,"wsock32.lib") to the smtp.h
       file so existing projects don't have to have their linker
       options modified.

 1.1 - Rearranged the headers so they are written out as:
       From,To,Subject,Date,MimeVersion,
       followed by all remaining headers
     - Modified the class to support multipart/alternative with
       multiple message bodies.

 Note that CSimpleMap does not sort the key values, and CSmtp
 takes advantage of this by writing the headers out in the reverse
 order of how they will be parsed before being sent to the SMTP
 server.  If you modify the code to use std::map or any other map
 class, the headers may be alphabetized by key, which may cause
 some mail clients to show the headers in the body of the message
 or cause other undesirable results when viewing the message.
*/
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Smtp.h"

#include "RotatingLog.h"


#define LOG_FILE_PATH "..\\tunning\\SmtpLog.txt"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSmtpMessageBody
//////////////////////////////////////////////////////////////////////
CSmtpMessageBody::CSmtpMessageBody(LPCTSTR pszBody, LPCTSTR pszEncoding, LPCTSTR pszCharset, EncodingEnum encode)
{
	// Set the default message encoding method
	// To transfer html messages, make Encoding = _T("text/html")
	if (pszEncoding) Encoding = pszEncoding;
	if (pszCharset)  Charset  = pszCharset;
	if (pszBody)     Data     = pszBody;
	TransferEncoding          = encode;
}

const CSmtpMessageBody& CSmtpMessageBody::operator=(LPCTSTR pszBody)
{
	Data = pszBody;
	return *this;
}

const CSmtpMessageBody& CSmtpMessageBody::operator=(const String& strBody)
{
	Data = strBody;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSmtpAttachment
//////////////////////////////////////////////////////////////////////
CSmtpAttachment::CSmtpAttachment(LPCTSTR pszFilename, LPCTSTR pszAltName, BOOL bIsInline, LPCTSTR pszEncoding, LPCTSTR pszCharset, EncodingEnum encode)
{
	if (pszFilename) FileName = pszFilename;
	if (pszAltName)  AltName  = pszAltName;
	if (pszEncoding) Encoding = pszEncoding;
	if (pszCharset)  Charset  = pszCharset;
	TransferEncoding = encode;
	Inline = bIsInline;
}

const CSmtpAttachment& CSmtpAttachment::operator=(LPCTSTR pszFilename)
{
	FileName = pszFilename;
	return *this;
}

const CSmtpAttachment& CSmtpAttachment::operator=(const String& strFilename)
{
	FileName = strFilename;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSmtpAddress
//////////////////////////////////////////////////////////////////////
CSmtpAddress::CSmtpAddress(LPCTSTR pszAddress, LPCTSTR pszName)
{
	if (pszAddress) Address = pszAddress;
	if (pszName) Name = pszName;
}

const CSmtpAddress& CSmtpAddress::operator=(LPCTSTR pszAddress)
{
	Address = pszAddress;
	return *this;
}

const CSmtpAddress& CSmtpAddress::operator=(const String& strAddress)
{
	Address = strAddress;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSmtpMessage
//////////////////////////////////////////////////////////////////////
CSmtpMessage::CSmtpMessage()
{
	TIME_ZONE_INFORMATION tzi;
	DWORD dwRet;
	long Offset; 
	
	// Get local time and timezone offset 
	GetLocalTime(&Timestamp); 
	GMTOffset = 0; 
	dwRet = GetTimeZoneInformation(&tzi); 
	Offset = tzi.Bias; 
	if (dwRet == TIME_ZONE_ID_STANDARD) Offset += tzi.StandardBias; 
	if (dwRet == TIME_ZONE_ID_DAYLIGHT) Offset += tzi.DaylightBias; 
	GMTOffset = -((Offset / 60) * 100 + (Offset % 60)); 
	
	MimeType = mimeGuess;
}

// Write all the headers to the e-mail message.
// This is done just before sending it, when we're sure the user wants it to go out.
void CSmtpMessage::CommitHeaders()
{
	TCHAR szTime[64];
	TCHAR szDate[64];
	TCHAR szOut[1024];
	String strHeader;
	String strValue;
	int n;

	// Assign a few standard headers to the message
	strHeader = _T("X-Priority");
	strValue  = _T("3 (Normal)");
	// Only add the key if it doesn't exist already in the headers map
	if (Headers.FindKey(strHeader) == -1) Headers.Add(strHeader,strValue);

	strHeader = _T("X-MSMail-Priority");
	strValue  = _T("Normal");
	if (Headers.FindKey(strHeader) == -1) Headers.Add(strHeader,strValue);

	strHeader = _T("X-Mailer");
	//strValue  = _T("ATL CSmtp Class Mailer by Robert Simpson (robert@blackcastlesoft.com)");
	strValue  = _T("ATL CSmtp Class Mailer by ECO Corp. (gjpark@eco.co.kr)");
	if (Headers.FindKey(strHeader) == -1) Headers.Add(strHeader,strValue);

	strHeader = _T("Importance");
	strValue  = _T("Normal");
	if (Headers.FindKey(strHeader) == -1) Headers.Add(strHeader,strValue);

	// Get the time/date stamp and GMT offset for the Date header.
	GetDateFormat(MAKELCID(LANG_ENGLISH, SORT_DEFAULT),0,&Timestamp,_T("ddd, d MMM yyyy"),szDate,64); 
	GetTimeFormat(MAKELCID(LANG_ENGLISH, SORT_DEFAULT),0,&Timestamp,_T("H:mm:ss"),szTime,64); 

	// Add the date/time stamp to the message headers 
	wsprintf(szOut,_T("%s %s %c%4.4d"),szDate,szTime,(GMTOffset>0)?'+':'-',GMTOffset); 

	strHeader = _T("Date");
	strValue = szOut;
	Headers.Remove(strHeader);
	Headers.Add(strHeader,strValue);

	// Write out the subject header
	strHeader = _T("Subject");
	strValue = Subject;
	Headers.Remove(strHeader);
	Headers.Add(strHeader,strValue);

	// Write out the TO header
	strValue.erase();
	strHeader = _T("To");
	if (Recipient.Name.length())
	{
	wsprintf(szOut,_T("\"%s\" "),Recipient.Name.c_str());
	strValue += szOut;
	}
	if (Recipient.Address.length())
	{
	wsprintf(szOut,_T("<%s>"),Recipient.Address.c_str());
	strValue += szOut;
	}
	// Write out all the CC'd names
	for (n = 0;n < CC.GetSize();n++)
	{
	if (strValue.length()) strValue += _T(",\r\n\t");
	if (CC[n].Name.length())
	{
	  wsprintf(szOut,_T("\"%s\" "),CC[n].Name.c_str());
	  strValue += szOut;
	}
	wsprintf(szOut,_T("<%s>"),CC[n].Address.c_str());
	strValue += szOut;
	}
	Headers.Remove(strHeader);
	Headers.Add(strHeader,strValue);

	// Write out the FROM header
	strValue.erase();
	strHeader = _T("From");
	if (Sender.Name.length())
	{
	wsprintf(szOut,_T("\"%s\" "),Sender.Name.c_str());
	strValue += szOut;
	}
	wsprintf(szOut,_T("<%s>"),Sender.Address.c_str());
	strValue += szOut;
	Headers.Remove(strHeader);
	Headers.Add(strHeader,strValue);
}

// Parse a message into a single string
void CSmtpMessage::Parse(String& strDest)
{
	String strHeader;
	String strValue;
	String strTemp;
	String strBoundary;  
	String strInnerBoundary;
	TCHAR szOut[1024];
	int n;
	
	strDest.erase();
	// Get a count of the sections to see if this will be a multipart message
	n  = Message.GetSize();
	n += Attachments.GetSize();
	
	// Remove this header in case the message is being reused
	strHeader = _T("Content-Type");
	Headers.Remove(strHeader);
	
	// If we have more than one section, then this is a multipart MIME message
	if (n > 1)
	{
		wsprintf(szOut,_T("CSmtpMsgPart123X456_000_%8.8X"),GetTickCount());
		strBoundary = szOut;
		
		lstrcpy(szOut,_T("multipart/"));
		
		if (MimeType == mimeGuess)
		{
			if (Attachments.GetSize() == 0) MimeType = mimeAlternative;
			else MimeType = mimeMixed;
		}
		switch(MimeType)
		{
		case mimeAlternative:
			lstrcat(szOut,_T("alternative"));
			break;
		case mimeMixed:
			lstrcat(szOut,_T("mixed"));
			break;
		case mimeRelated:
			lstrcat(szOut,_T("related"));
			break;
		}
		lstrcat(szOut,_T(";\r\n\tboundary=\""));
		lstrcat(szOut,strBoundary.c_str());
		lstrcat(szOut,_T("\""));
		
		strValue = szOut;
		Headers.Add(strHeader,strValue);
	}
	
	strHeader = _T("MIME-Version");
	strValue  = MIME_VERSION;
	Headers.Remove(strHeader);
	Headers.Add(strHeader,strValue);
	
	// Remove any message ID in the header and replace it with this message ID, if it exists
	strHeader = _T("Message-ID");
	Headers.Remove(strHeader);
	if (MessageId.length())
	{
		wsprintf(szOut,_T("<%s>"),MessageId.c_str());
		strValue = szOut;
		Headers.Add(strHeader,strValue);
	}
	
	// Finalize the message headers
	CommitHeaders();
	
	// Write out all the message headers -- done backwards on purpose!
	for (n = Headers.GetSize();n > 0;n--)
	{
		wsprintf(szOut,_T("%s: %s\r\n"),Headers.GetKeyAt(n-1).c_str(),Headers.GetValueAt(n-1).c_str());
		strDest += szOut;
	}
	if (strBoundary.length())
	{
		wsprintf(szOut,_T("\r\n%s\r\n"),MULTIPART_MESSAGE);
		strDest += szOut;
	}
	
	// If we have attachments and multiple message bodies, create a new multipart section
	// This is done so we can display our multipart/alternative section separate from the
	// main multipart/mixed environment, and support both attachments and alternative bodies.
	if (Attachments.GetSize() && Message.GetSize() > 1 && strBoundary.length())
	{
		wsprintf(szOut,_T("CSmtpMsgPart123X456_001_%8.8X"),GetTickCount());
		strInnerBoundary = szOut;
		
		wsprintf(szOut,_T("\r\n--%s\r\nContent-Type: multipart/alternative;\r\n\tboundary=\"%s\"\r\n"),strBoundary.c_str(),strInnerBoundary.c_str());
		strDest += szOut;
	}
	
	for (n = 0;n < Message.GetSize();n++)
	{
		// If we're multipart, then write the boundary line
		if (strBoundary.length() || strInnerBoundary.length())
		{
			strDest += _T("\r\n--");
			// If we have an inner boundary, write that one.  Otherwise write the outer one
			if (strInnerBoundary.length()) strDest += strInnerBoundary;
			else strDest += strBoundary;
			strDest += _T("\r\n");
		}
		strValue.erase();
		strDest += _T("Content-Type: ");
		strDest += Message[n].Encoding;
		// Include the character set if the message is text
		if (_tcsnicmp(Message[n].Encoding.c_str(),_T("text/"),5) == 0)
		{
			wsprintf(szOut,_T(";\r\n\tcharset=\"%s\""),Message[n].Charset.c_str());
			strDest += szOut;
		}
		strDest += _T("\r\n");
		
		// Encode the message
		strValue = Message[n].Data;		
				
		// 13/04/08 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업 
		//                   메일 헤더 인코딩 타입 미정의로 디코딩이 안되서 
		//                   수신자 쪽에서 메일 내용이 표시안되는 문제 (경기도립중앙도서관 요청사항) 
/*//BEFORE-----------------------------------------------------------------------
		// 2008.08.29 REM BY PDJ : 유니코드에서 Encode가 필요 없음
		//EncodeMessage(Message[n].TransferEncoding,strValue,strTemp);
*///AFTER------------------------------------------------------------------------
		EncodeMessage(Message[n].TransferEncoding, strValue, strTemp);
/*//END------------------------------------------------------------------------*/		
		// Write out the encoding method used and the encoded message
		strDest += _T("Content-Transfer-Encoding: ");    
		strDest += strTemp;
		
		// If the message body part has a content ID, write it out
		if (Message[n].ContentId.length())
		{
			wsprintf(szOut,_T("\r\nContent-ID: <%s>"),Message[n].ContentId.c_str());
			strDest += szOut;
		}
		strDest += _T("\r\n\r\n");
		strDest += strValue;
	}
	
	// If we have multiple message bodies, write out the trailing inner end sequence
	if (strInnerBoundary.length())
	{
		wsprintf(szOut,_T("\r\n--%s--\r\n"),strInnerBoundary.c_str());
		strDest += szOut;
	}
	
	// Process any attachments
	for (n = 0;n < Attachments.GetSize();n++)
	{
		HANDLE hFile;
		LPBYTE pData;
		LPTSTR pszFile;
		LPTSTR pszExt;
		DWORD dwSize;
		DWORD dwBytes;
		CRegKey cKey;
		TCHAR szType[MAX_PATH];
		TCHAR szFilename[MAX_PATH];
		
		// Get the filename of the attachment
		strValue = Attachments[n].FileName;
		
		// Open the file
		lstrcpy(szFilename,strValue.c_str());
		hFile = CreateFile(szFilename,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			// Get the size of the file, allocate the memory and read the contents.
			dwSize = GetFileSize(hFile,NULL);
			pData = (LPBYTE)malloc(dwSize + 1);
			ZeroMemory(pData,dwSize+1);
			
			if (ReadFile(hFile,pData,dwSize,&dwBytes,NULL))
			{
				// Write out our boundary marker
				if (strBoundary.length())
				{
					wsprintf(szOut,_T("\r\n--%s\r\n"),strBoundary.c_str());
					strDest += szOut;
				}
				
				// If no alternate name is supplied, strip the path to get the base filename
				if (!Attachments[n].AltName.length())
				{
					// Strip the path from the filename
					pszFile = _tcsrchr(szFilename,'\\');
					if (!pszFile) pszFile = szFilename;
					else pszFile ++;
				}
				else pszFile = (LPTSTR)Attachments[n].AltName.c_str();
				
				// Set the content type for the attachment.
				lstrcpy(szType,_T("application/octet-stream"));
				
				// Check the registry for a content type that overrides the above default
				pszExt = _tcschr(pszFile,'.');
				if (pszExt)
				{
					if (!cKey.Open(HKEY_CLASSES_ROOT,pszExt,KEY_READ))
					{
						DWORD dwSize = MAX_PATH;
						cKey.QueryValue(szType,_T("Content Type"),&dwSize);
						cKey.Close();
					}
				}
				
				// If the attachment has a specific encoding method, use it instead
				if (Attachments[n].Encoding.length())
					lstrcpy(szType,Attachments[n].Encoding.c_str());
				
				// Write out the content type and attachment types to the message
				wsprintf(szOut,_T("Content-Type: %s"),szType);
				strDest += szOut;
				// If the content type is text, write the charset
				if (_tcsnicmp(szType,_T("text/"),5) == 0)
				{
					wsprintf(szOut,_T(";\r\n\tcharset=\"%s\""),Attachments[n].Charset.c_str());
					strDest += szOut;
				}
				wsprintf(szOut,_T(";\r\n\tname=\"%s\"\r\n"),pszFile);
				strDest += szOut;
				
				// Encode the attachment
				EncodeMessage(Attachments[n].TransferEncoding,strValue,strTemp,pData,dwSize);
				
				// Write out the transfer encoding method
				wsprintf(szOut,_T("Content-Transfer-Encoding: %s\r\n"),strTemp.c_str());
				strDest += szOut;
				
				// Write out the attachment's disposition
				strDest += _T("Content-Disposition: ");
				
				if (Attachments[n].Inline) strDest += _T("inline");
				else strDest += _T("attachment");
				
				strDest += _T(";\r\n\tfilename=\"");
				strDest += pszFile;
				
				// If the attachment has a content ID, write it out
				if (Attachments[n].ContentId.length())
				{
					wsprintf(szOut,_T("\r\nContent-ID: <%s>"),Attachments[n].ContentId.c_str());
					strDest += szOut;
				}
				strDest += _T("\r\n\r\n");
				
				// Write out the encoded attachment
				strDest += strValue;
				strTemp.erase();
				strValue.erase();
			}
			// Close the file and clear the temp buffer
			CloseHandle(hFile);
			free(pData);
		}
	}
  
	// If we are multipart, write out the trailing end sequence
	if (strBoundary.length())
	{
		wsprintf(szOut,_T("\r\n--%s--\r\n"),strBoundary.c_str());
		strDest += szOut;
	}
}

// Parses text into quoted-printable lines.
// See RFC 1521 for full details on how this works.
void CSmtpMessage::EncodeQuotedPrintable(String& strDest, String& strSrc)
{
	String strTemp;
	String strTemp2;
	LPTSTR pszTok1;
	LPTSTR pszTok2;
	TCHAR szSub[16];
	TCHAR ch;
	int n;
	
	strDest.erase();
	if (!strSrc.length()) return;
	
	// Change = signs and non-printable characters to =XX
	pszTok1 = (LPTSTR)strSrc.c_str();
	pszTok2 = pszTok1;
	do
	{
		if (*pszTok2 == '=' || *pszTok2 > 126 || 
			(*pszTok2 < 32 && (*pszTok2 != '\r' && *pszTok2 != '\n' && *pszTok2 != '\t')))
		{
			ch = *pszTok2;
			*pszTok2 = 0;
			strTemp += pszTok1;
			*pszTok2 = ch;
			wsprintf(szSub,_T("=%2.2X"),(BYTE)*pszTok2);
			strTemp += szSub;
			pszTok1 = pszTok2 + 1;
		}
		pszTok2 ++;
	} while (*pszTok2);

	// Append anything left after the search
	if (_tcslen(pszTok1)) strTemp += pszTok1;
	
	pszTok1 = (LPTSTR)strTemp.c_str();
	while (pszTok1)
	{
		pszTok2 = _tcschr(pszTok1,'\r');
		if (pszTok2) *pszTok2 = 0;
		while (1)
		{
			if (_tcslen(pszTok1) > 76)
			{
				n = 75; // Breaking at the 75th character
				if (pszTok1[n-1] == '=') n -= 1; // If the last character is an =, don't break the line there
				else if (pszTok1[n-2] == '=') n -= 2; // If we're breaking in the middle of a = sequence, back up!
				
				// Append the first section of the line to the total string
				ch = pszTok1[n];
				pszTok1[n] = 0;
				strDest += pszTok1;
				pszTok1[n] = ch;
				strDest += _T("=\r\n");
				pszTok1 += n;
			}
			else // Line is less than or equal to 76 characters
			{
				n = _tcslen(pszTok1); // If we have some trailing data, process it.
				if (n)
				{
					if (pszTok1[n-1] == ' ' || pszTok1[n-1] == '\t') // Last character is a space or tab
					{
						wsprintf(szSub,_T("=%2.2X"),(BYTE)pszTok1[n-1]);
						// Replace the last character with an =XX sequence
						pszTok1[n-1] = 0;
						strTemp2 = pszTok1;
						strTemp2 += szSub;
						// Since the string may now be larger than 76 characters, we have to reprocess the line
						pszTok1 = (LPTSTR)strTemp2.c_str();
					}
					else // Last character is not a space or tab
					{
						strDest += pszTok1;
						if (pszTok2) strDest += _T("\r\n");
						break; // Exit the loop which processes this line, and move to the next line
					}
				}
				else
				{
					if (pszTok2) strDest += _T("\r\n");
					break; // Move to the next line
				}
			}
		}
		if (pszTok2)
		{
			*pszTok2 = '\r';
			pszTok2 ++;
			if (*pszTok2 == '\n') pszTok2 ++;
		}
		pszTok1 = pszTok2;
	}
}

// Breaks a message's lines into a maximum of 76 characters
// Does some semi-intelligent wordwrapping to ensure the text is broken properly.
// If a line contains no break characters, it is forcibly truncated at the 76th char
void CSmtpMessage::BreakMessage(String& strDest, String& strSrc, int nLength)
{
	String strTemp = strSrc;
	String strLine;
	LPTSTR pszTok1;
	LPTSTR pszTok2;
	LPTSTR pszBreak;
	LPTSTR pszBreaks = _T(" -;.,?!");
	TCHAR ch;
	BOOL bNoBreaks;
	int nLen;
	
	strDest.erase();
	if (!strSrc.length()) return;
	
	nLen = strTemp.length();
	nLen += (nLen / 60) * 2;
	
	strDest.reserve(nLen);
	
	// Process each line one at a time
	pszTok1 = (LPTSTR)strTemp.c_str();
	while (pszTok1)
	{
		pszTok2 = _tcschr(pszTok1,'\r');
		if (pszTok2) *pszTok2 = 0;
		
		bNoBreaks = (!_tcspbrk(pszTok1,pszBreaks));
		nLen = _tcslen(pszTok1);
		while (nLen > nLength)
		{
			// Start at the 76th character, and move backwards until we hit a break character
			pszBreak = &pszTok1[nLength - 1];
			
			// If there are no break characters in the string, skip the backward search for them!
			if (!bNoBreaks)
			{
				while (!_tcschr(pszBreaks,*pszBreak) && pszBreak > pszTok1)
					pszBreak--;
			}
			pszBreak ++;
			ch = *pszBreak;
			*pszBreak = 0;
			strDest += pszTok1;
			
			strDest += _T("\r\n");
			*pszBreak = ch;
			
			nLen -= (pszBreak - pszTok1);
			// Advance the search to the next segment of text after the break
			pszTok1 = pszBreak;
		}
		strDest += pszTok1;
		if (pszTok2)
		{
			strDest += _T("\r\n");
			*pszTok2 = '\r';
			pszTok2 ++;
			if (*pszTok2 == '\n') pszTok2 ++;
		}
		pszTok1 = pszTok2;
	}
}

// Makes the message into a 7bit stream
void CSmtpMessage::Make7Bit(String& strDest, String& strSrc)
{
	LPTSTR pszTok;
	
	strDest = strSrc;
	
	pszTok = (LPTSTR)strDest.c_str();
	do
	{
		// Replace any characters above 126 with a ? character
		if (*pszTok > 126 || *pszTok < 0)
			*pszTok = '?';
		pszTok ++;
	} while (*pszTok);
}

// Encodes a message or binary stream into a properly-formatted message
// Takes care of breaking the message into 76-byte lines of text, encoding to
// Base64, quoted-printable and etc.
void CSmtpMessage::EncodeMessage(EncodingEnum code, String& strMsg, String& strMethod, LPBYTE pByte, DWORD dwSize)
{
	String strTemp;
	LPTSTR pszTok1;
	LPTSTR pszTok2;
	LPSTR pszBuffer = NULL;
	LPSTR pszTemp;
	DWORD dwStart = GetTickCount();
	
	if (!pByte)
	{
		INT len = WideCharToMultiByte(CP_ACP, 0, strMsg.c_str(), -1, NULL, 0, NULL,NULL);
		pszBuffer = (LPSTR)malloc(len);
		WideCharToMultiByte(CP_ACP, 0, strMsg.c_str(), -1, pszBuffer, len, NULL, NULL);
//		pszBuffer = (LPSTR)malloc(strMsg.length() + 1);
//		_T2A(pszBuffer,strMsg.c_str());

		pByte = (LPBYTE)pszBuffer;
//		dwSize = strMsg.length();
		dwSize = strlen(pszBuffer);
	}
	
	// Guess the encoding scheme if we have to
	if (code == encodeGuess) code = GuessEncoding(pByte, dwSize);
	
	switch(code)
	{
	case encodeQuotedPrintable:
		strMethod = _T("quoted-printable");
		
		pszTok1 = (LPTSTR)malloc((dwSize+1) * sizeof(TCHAR));
		_A2T(pszTok1,(LPSTR)pByte);
		strMsg = pszTok1;
		free(pszTok1);
		
		EncodeQuotedPrintable(strTemp, strMsg);
		break;
	case encodeBase64:
		strMethod = _T("base64");
		{
			CBase64 cvt;      
			cvt.Encode(pByte, dwSize);
			pszTemp = (LPSTR)cvt.EncodedMessage();
			pszTok1 = (LPTSTR)malloc((lstrlenA(pszTemp)+1) * sizeof(TCHAR));
			_A2T(pszTok1,pszTemp);
		}
		strMsg = pszTok1;
		free(pszTok1);
		
		BreakMessage(strTemp, strMsg);
		break;
	case encode7Bit:
		strMethod = _T("7bit");
		
		pszTok1 = (LPTSTR)malloc((dwSize+1) * sizeof(TCHAR));
		_A2T(pszTok1,(LPSTR)pByte);
		strMsg = pszTok1;
		free(pszTok1);
		
		Make7Bit(strTemp, strMsg);
		strMsg = strTemp;
		BreakMessage(strTemp, strMsg);
		break;
	case encode8Bit:
		strMethod = _T("8bit");
		
		pszTok1 = (LPTSTR)malloc((dwSize+1) * sizeof(TCHAR));
		_A2T(pszTok1,(LPSTR)pByte);
		strMsg = pszTok1;
		free(pszTok1);
		
		BreakMessage(strTemp, strMsg);
		break;
	}
	
	if (pszBuffer) free(pszBuffer);
	
	strMsg.erase();
	
	// Parse the message text, replacing CRLF. sequences with CRLF.. sequences
	pszTok1 = (LPTSTR)strTemp.c_str();
	do
	{
		pszTok2 = _tcsstr(pszTok1,_T("\r\n."));
		if (pszTok2)
		{
			*pszTok2 = 0;
			strMsg += pszTok1;
			*pszTok2 = '\r';
			strMsg += _T("\r\n..");
			pszTok1 = pszTok2 + 3;
		}
	} while (pszTok2);  
	strMsg += pszTok1;
	
	TCHAR szOut[MAX_PATH];
	wsprintf(szOut,_T("Encoding took %dms\n"),GetTickCount() - dwStart);
	OutputDebugString(szOut);
}

// Makes a best-guess of the proper encoding to use for this stream of bytes
// It does this by counting the # of lines, the # of 8bit bytes and the number
// of 7bit bytes.  It also records the line and the count of lines over
// 76 characters.
// If the stream is 90% or higher 7bit, it uses a text encoding method.  If the stream
// is all at or under 76 characters, it uses 7bit or 8bit, depending on the content.
// If the lines are longer than 76 characters, use quoted printable.
// If the stream is under 90% 7bit characters, use base64 encoding.
EncodingEnum CSmtpMessage::GuessEncoding(LPBYTE pByte, DWORD dwLen)
{
	int n7Bit = 0;
	int n8Bit = 0;
	int nLineStart = 0;
	int nLinesOver76 = 0;
	int nLines = 0;
	DWORD n;
	
	// Count the content type, byte by byte
	for (n = 0;n < dwLen; n++)
	{
		if (pByte[n] > 126 || (pByte[n] < 32 && pByte[n] != '\t' && pByte[n] != '\r' && pByte[n] != '\n'))
			n8Bit ++;
		else n7Bit ++;
		
		// New line?  If so, record the line size
		if (pByte[n] == '\r')
		{
			nLines ++;
			nLineStart = (n - nLineStart) - 1;
			if (nLineStart > 76) nLinesOver76 ++;
			nLineStart = n + 1;      
		}
	}
	// Determine if it is mostly 7bit data
	if ((n7Bit * 100) / dwLen > 89)
	{
		// At least 90% text, so use a text-base encoding scheme
		if (!nLinesOver76)
		{
			if (!n8Bit) return encode7Bit;
			else return encode8Bit;
		}
		else return encodeQuotedPrintable;
	}
	return encodeBase64;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSmtp
//////////////////////////////////////////////////////////////////////
CSmtp::CSmtp()
{
	LPSERVENT pEnt;

	m_bExtensions = TRUE;       // Use ESMTP if possible
	m_dwCmdTimeout = 30;        // Default to 30 second timeout
	//m_hSocket = INVALID_SOCKET;

	// Try and get the SMTP service entry by name
	pEnt = getservbyname("SMTP","tcp");
	if (pEnt) 
		m_wSmtpPort = pEnt->s_port;
	else 
		m_wSmtpPort = htons(25);
/*
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 );
	if ( WSAStartup( wVersionRequested, &wsaData ) != 0 ) {
		AfxMessageBox("WSAStartup Error !!!");
		//return FALSE;
	}
*/
}

CSmtp::~CSmtp()
{
	// Make sure any open connections are shut down
	Close();
}

// Connects to a SMTP server.  Returns TRUE if successfully connected, or FALSE otherwise.
BOOL CSmtp::Connect(LPCTSTR pszServer)
{
	int nRet;
	BOOL bRet;
	
	CString sLog;
	CRotatingLog Log(LOG_FILE_PATH, DATE_FILE_NAME);

	WSADATA wsaData;
	if (0!=WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		Log.StartAddLineStop("[SMTP   ][CONNECT ][00][X] WSAStartup error !!! ### EXIT ###");
		return FALSE;
	}
	
	// 이미 연결되어 있는 Socket을 닫는다
	Close();  
	Log.StartAddLineStop("[SMTP   ][CONNECT ][01][-] Close Socket already opened...");

	if (!m_wsSMTPServer.Create()) {
		sLog.Format(_T("[SMTP   ][CONNECT ][02][X] Create socket (ErrorCode:%s) !!! ### EXIT ###"), 
					this->GetErrorMsg(WSAGetLastError()));
		Log.StartAddLineStop(sLog);
		return FALSE;
	}
	Log.StartAddLineStop("[SMTP   ][CONNECT ][02][O] Creating Socket !!!");

	if (!m_wsSMTPServer.Connect(pszServer, 25)) {
		sLog.Format(_T("[SMTP   ][CONNECT ][03][X] Connect to SMTP Server(%s:%d) (ErrorCode:%s) !!! ### EXIT ###"), 
					pszServer, 25, this->GetErrorMsg(WSAGetLastError()));
		Log.StartAddLineStop(sLog);
		Close();
		return FALSE;
	}
	sLog.Format(_T("[SMTP   ][CONNECT ][03][O] Connect to SMTP Server(%s:%d) !!!"), pszServer, 25);
	Log.StartAddLine(sLog);
	Log.AddLineStop("[SMTP   ][CONNECT ][04][-] Now Send 'NULL' command !!!");
	nRet = SendCmd(NULL);
	if (nRet != 220) {
		Log.StartAddLineStop("[SMTP   ][CONNECT ][04][X] Send 'NULL' command !!! ### EXIT ###");
		Close();
		return FALSE;
	}

	Log.StartAddLineStop("[SMTP   ][CONNECT ][05][-] Now Send 'HELLO' command !!!");
	bRet = SendHello();
	if (bRet) {
		Log.StartAddLineStop("[SMTP   ][CONNECT ][05][X] Send 'HELLO' command !!! ### EXIT ###");
		Close();
		return FALSE;
	}
	
	return TRUE;
}

// Closes any active SMTP sessions and shuts down the socket.
void CSmtp::Close()
{
	m_wsSMTPServer.Close();
/*
  if (m_hSocket != INVALID_SOCKET)
  {
    // If we're connected to a server, tell them we're quitting
    if (m_bConnected) SendQuitCmd();
    // Shutdown and close the socket
    shutdown(m_hSocket,2);
    closesocket(m_hSocket);
  }
  m_hSocket = INVALID_SOCKET;
*/
}


// Send a command to the SMTP server and wait for a response
int CSmtp::SendCmd(LPTSTR pszCmd)
{
	// Command가 NULL이 아닐 경우 Command를 Send한다
	CString sLog;
	CRotatingLog Log(LOG_FILE_PATH, DATE_FILE_NAME);
	
	sLog.Format(_T("[SMTP   ][COMMAND ][01][-] Send command :\n %s !!!"), pszCmd);
	Log.StartAddLineStop(sLog);

	DWORD dwLen;
	dwLen = lstrlen(pszCmd);
	
	if (NULL!=pszCmd) {				
		//Send the body	
#ifdef _UNICODE
		LPSTR pszBuff;
		
		pszBuff = (LPSTR) malloc((dwLen + 1) * sizeof(TCHAR)); // JMC 20060224
		
		memset(pszBuff, 0x00, (dwLen + 1) * 2);
		WideCharToMultiByte(CP_ACP, 0, pszCmd, dwLen, pszBuff, (dwLen + 1) * sizeof(TCHAR), NULL, NULL);
		
		//_T2A(pszBuff, pszCmd);
		dwLen = strlen(pszBuff);
		
		
		if (SOCKET_ERROR == m_wsSMTPServer.Send(pszBuff, dwLen)) {
			Log.StartAddLineStop("[SMTP   ][COMMAND ][01][X] SOCKET_ERROR !!! ### EXIT ###");
			free(pszBuff);
			return -1;
		}
		
		free(pszBuff); // JMC 20060224
#else
		m_wsSMTPServer.Send(pszCmd, dwLen);
#endif
	}
// 		INT len = WideCharToMultiByte(CP_ACP, 0, pszCmd, -1, NULL, 0, NULL,NULL);
//		char* ctmp = new char[len];
//		WideCharToMultiByte(CP_ACP, 0, pszCmd, -1, ctmp, len, NULL, NULL);
//
//		len = strlen(ctmp);
//		if (SOCKET_ERROR == m_wsSMTPServer.Send(ctmp,len)) {
//			Log.StartAddLineStop("[SMTP   ][COMMAND ][01][X] SOCKET_ERROR !!! ### EXIT ###");
//			delete [] ctmp;
//			return -1;
//		}
//		delete [] ctmp;
//		//Log.StartAddLineStop("[SMTP   ][COMMAND ][01][O] Send Command !!!");		
	
	
	// 결과를 받는다
	CHAR szResult [CMD_RESPONSE_SIZE];
	CHAR szRetCode[                4];
	
//	memset(szResult, 0, CMD_RESPONSE_SIZE);
	
	// 2008.06.26 UPDATE BY PDW {{++
	// BYTE 계산을 할수있도록 리턴값을 받는다
//	if (SOCKET_ERROR == m_wsSMTPServer.Receive(szResult, CMD_RESPONSE_SIZE)) {
//		Log.StartAddLineStop("[SMTP   ][COMMAND ][01][X] SOCKET_ERROR !!! ### EXIT ###");
//		return -1;
//	}

	INT nData = m_wsSMTPServer.Receive(szResult, CMD_RESPONSE_SIZE);				
	if (SOCKET_ERROR == nData) {
		Log.StartAddLineStop("[SMTP   ][COMMAND ][01][X] SOCKET_ERROR !!! ### EXIT ###");
		return -1;
	}
	if( szResult )
		szResult[nData]='\0';
	
	// 2008.06.26 UPDATE BY PDW --}}

	// 2008.06.20 UPDATE BY PDW {{++
	// UNICODE 변형
	int len = MultiByteToWideChar(CP_ACP,0,szResult,-1,NULL,0);
 	WCHAR* ctmp = new WCHAR[len];
    MultiByteToWideChar(CP_ACP,0,szResult,-1,ctmp,len);
	m_strResult = CString(ctmp);
	delete []ctmp;	
	// 2008.06.20 UPDATE BY PDW --}}
	TCHAR szUniRetCode[4];
	_tcsncpy(szUniRetCode, &m_strResult[0], 3);
	szUniRetCode[3] = '\0';

	sLog.Format(_T("[SMTP   ][COMMAND ][02][-] Return Code : %s !!!"), szUniRetCode);
	Log.StartAddLineStop(sLog);

	return _ttoi(szUniRetCode);
}	



// Placeholder function -- overridable
// This function is called when the SMTP server gives us an unexpected error
// The <nError> value is the SMTP server's numeric error response, and the <pszErr>
// is the descriptive error text
//
// <pszErr> may be NULL if the server failed to respond before the timeout!
// <nError> will be -1 if a catastrophic failure occurred.
//
// Return 0, or nError.  The return value is currently ignored.
int CSmtp::SmtpError(int /*nError*/, LPTSTR pszErr)
{
#ifdef _DEBUG
	if (pszErr) {
		OutputDebugString(_T("SmtpError: "));
		OutputDebugString(pszErr);
		OutputDebugString(_T("\n"));
	}
#endif
  return 0;
}

// Placeholder function -- overridable
// Currently the only warning condition that this class is designed for is
// an authentication failure.  In that case, <nWarning> will be 535,
// which is the RFC error for authentication failure.  If authentication
// fails, you can override this function to prompt the user for a new
// username and password.  Change the <m_strUser> and <m_strPass> member
// variables and return TRUE to retry authentication.
//
// <pszWarning> may be NULL if the server did not respond in time!
//
// Return FALSE to abort authentication, or TRUE to retry.
int CSmtp::SmtpWarning(int /*nWarning*/, LPTSTR pszWarning)
{
#ifdef _DEBUG
	if (pszWarning)
	{
		OutputDebugString(_T("SmtpWarning: "));
		OutputDebugString(pszWarning);
		OutputDebugString(_T("\n"));
	}
#endif
  return 0;
}

// Placeholder function -- overridable
// This is an informational callback only, and provides a means to inform
// the caller as the SMTP session progresses.
// ALWAYS check for NULL values on <pszCmd> and <pszResponse> before performing
// any actions!
// <nResponse> will be -1 if a catastrophic failure occurred, but that will
// be raised in the SmtpError() event later on during processing.
void CSmtp::SmtpCommandResponse(LPTSTR pszCmd, int /*nResponse*/, LPTSTR pszResponse)
{
#ifdef _DEBUG
	if (pszCmd)
	{
		TCHAR szOut[MAX_PATH+1];
		OutputDebugString(_T("SmtpCommand : "));
		while (lstrlen(pszCmd) > MAX_PATH)
		{
			lstrcpyn(szOut,pszCmd,MAX_PATH+1);
			OutputDebugString(szOut);
			//Sleep(100);
			pszCmd += MAX_PATH;
		}
		OutputDebugString(pszCmd);
	}
	OutputDebugString(_T("SmtpResponse: "));
	OutputDebugString(pszResponse);
	OutputDebugString(_T("\n"));
#endif
}

// Placeholder function -- overridable
// This is a progress callback to indicate that data is being sent over the wire
// and that the operation may take some time.
// Return TRUE to continue sending, or FALSE to abort the transfer
BOOL CSmtp::SmtpProgress(LPSTR /*pszBuffer*/, DWORD /*dwBytesSent*/, DWORD /*dwBytesTotal*/)
{
	return TRUE; // Continue sending the data
}

// Raises a SmtpError() condition
int CSmtp::RaiseError(int nError)
{
	// If the error code is -1, something catastrophic happened
	// so we're effectively not connected to any SMTP server.
	if (nError == -1) m_bConnected = FALSE;
	return SmtpError(nError, (LPTSTR)m_strResult.c_str());
}

// Warnings are recoverable errors that we may be able to continue working with
int CSmtp::RaiseWarning(int nWarning)
{
	return SmtpWarning(nWarning, (LPTSTR)m_strResult.c_str());
}

// E-Mail's a message
// Returns 0 if successful, -1 if an internal error occurred, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendMessage(CSmtpMessage &msg)
{
	int nRet;
	int n;
	//  int nRecipients = 0;
	int nRecipientCount = 0;
	
	CString sLog;
	CRotatingLog Log(LOG_FILE_PATH, DATE_FILE_NAME);

	// Check if we have a sender
	if (!msg.Sender.Address.length()) {
		Log.StartAddLineStop("[SMTP   ][MESSAGE ][01][X] None Sender !!! ### EXIT ###");
		return -1;
	}
	
	// Check if we have recipients
	if (!msg.Recipient.Address.length() && !msg.CC.GetSize()) {
		Log.StartAddLineStop("[SMTP   ][MESSAGE ][01][X] None Recipients !!! ### EXIT ###");
		return -1;
	}
	
	// Check if we have a message body or attachments
	// *** Commented out to remove the requirement that a message have a body or attachments
	//  if (!msg.Message.GetSize() && !msg.Attachments.GetSize()) return -1;
	
	// Send the sender's address
	sLog.Format(_T("[SMTP   ][MESSAGE ][01][-] Sender Address : %s"), (LPTSTR)msg.Sender.Address.c_str());
	Log.StartAddLineStop(sLog);

	nRet = SendFrom((LPTSTR)msg.Sender.Address.c_str());
	if (nRet) {
		sLog.Format(_T("[SMTP   ][MESSAGE ][01][X] Return Code : %d !!! ### EXIT ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
					nRet, nRet, (LPCTSTR)this->GetSmtpRetMsg(nRet));
		Log.StartAddLineStop(sLog);
		return nRet;
	}
	//sLog.Format("[SMTP   ][MESSAGE ][01][O] Return Code : %d !!!", nRet);
	//Log.StartAddLineStop(sLog);
	
	// If we have a recipient, send it
	nRecipientCount = 0; // Count of recipients
	if (msg.Recipient.Address.length()) {
		sLog.Format(_T("[SMTP   ][MESSAGE ][02][-] Recipient Address : %s"), (LPTSTR)msg.Recipient.Address.c_str());
		Log.StartAddLineStop(sLog);

		nRet = SendTo((LPTSTR)msg.Recipient.Address.c_str());

		if (!nRet) 
			nRecipientCount++;
		else
		{
			sLog.Format(_T("[SMTP   ][MESSAGE ][02][X] Recipient no exist !!! ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
						nRet, this->GetSmtpRetMsg(nRet));
			Log.StartAddLineStop(sLog);
		}
		sLog.Format(_T("[SMTP   ][MESSAGE ][02][-] Recipient Count : %d !!!"), nRecipientCount);
		Log.StartAddLineStop(sLog);
	}
	
	// If we have any CC's, send those.
	for (n = 0;n < msg.CC.GetSize();n++) {
		sLog.Format(_T("[SMTP   ][MESSAGE ][03][-] CC Address : %s !!!"), (LPTSTR)msg.CC[n].Address.c_str());
		Log.StartAddLineStop(sLog);

		nRet = SendTo((LPTSTR)msg.CC[n].Address.c_str());

		if (!nRet)
			nRecipientCount++;
		else
		{
			sLog.Format(_T("[SMTP   ][MESSAGE ][02][X] Recipient no exist !!! ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
						nRet, this->GetSmtpRetMsg(nRet));
			Log.StartAddLineStop(sLog);
		}
		
		sLog.Format(_T("[SMTP   ][MESSAGE ][03][-] Recipient Count : %d !!!"), nRecipientCount);
		Log.StartAddLineStop(sLog);
	}
	
	// If we have any bcc's, send those.
	for (n = 0;n < msg.BCC.GetSize();n++) {
		sLog.Format(_T("[SMTP   ][MESSAGE ][04][-] BCC Address : %s !!!"), (LPTSTR)msg.BCC[n].Address.c_str());
		Log.StartAddLineStop(sLog);
		
		nRet = SendTo((LPTSTR)msg.BCC[n].Address.c_str());
		
		if (!nRet)
			nRecipientCount++;
		else
		{
			sLog.Format(_T("[SMTP   ][MESSAGE ][02][X] Recipient no exist !!! ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
						nRet, this->GetSmtpRetMsg(nRet));
			Log.StartAddLineStop(sLog);
		}
		sLog.Format(_T("[SMTP   ][MESSAGE ][03][-] Recipient Count : %d !!!"), nRecipientCount);
		Log.StartAddLineStop(sLog);
	}
	
	// If we failed on all recipients, we must abort.
	if (!nRecipientCount) {
		//RaiseError(nRet);
		sLog.Format(_T("[SMTP   ][MESSAGE ][03][-] Recipient no exist !!! ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
					nRet, this->GetSmtpRetMsg(nRet));
		Log.StartAddLineStop(sLog);
	} 
	else 
	{
		Log.StartAddLineStop("[SMTP   ][MESSAGE ][05][-] Now Send Message Data !!!");

		nRet = SendData(msg);
	}

	return nRet;
}

// Simplified way to send a message.
// <pvAttachments> can be either an LPTSTR containing NULL terminated strings, in which
// case <dwAttachmentCount> should be zero, or <pvAttachments> can be an LPTSTR * 
// containing an array of LPTSTR's, in which case <dwAttachmentCount> should equal the
// number of strings in the array.
int CSmtp::SendMessage(CSmtpAddress &addrFrom, CSmtpAddress &addrTo, LPCTSTR pszSubject, LPTSTR pszMessage, LPVOID pvAttachments, DWORD dwAttachmentCount)
{
	CSmtpMessage message;
	CSmtpMessageBody body;
	CSmtpAttachment attach;
	
	body = pszMessage;
	
	message.Sender = addrFrom;
	message.Recipient = addrTo;
	message.Message.Add(body);
	message.Subject = pszSubject;
	
	// If the attachment count is zero, but the pvAttachments variable is not NULL,
	// assume that the ppvAttachments variable is a string value containing NULL terminated
	// strings.  A double NULL ends the list.
	// Example: LPTSTR pszAttachments = "foo.exe\0bar.zip\0autoexec.bat\0\0";
	if (!dwAttachmentCount && pvAttachments)
	{
		LPTSTR pszAttachments = (LPTSTR)pvAttachments;
		while (lstrlen(pszAttachments))
		{
			attach.FileName = pszAttachments;
			message.Attachments.Add(attach);
			pszAttachments = &pszAttachments[lstrlen(pszAttachments)];
		}
	}
	
	// dwAttachmentCount is not zero, so assume pvAttachments is an array of LPTSTR's
	// Example: LPTSTR *ppszAttachments = {"foo.exe","bar.exe","autoexec.bat"};
	if (pvAttachments && dwAttachmentCount)
	{
		LPTSTR *ppszAttachments = (LPTSTR *)pvAttachments;    
		while (dwAttachmentCount-- && ppszAttachments)
		{
			attach.FileName = ppszAttachments[dwAttachmentCount];
			message.Attachments.Add(attach);
		}
	}
	return SendMessage(message);
}

// Yet an even simpler method for sending a message
// <pszAddrFrom> and <pszAddrTo> should be e-mail addresses with no decorations
// Example:  "foo@bar.com"
// <pvAttachments> and <dwAttachmentCount> are described above in the alternative
// version of this function
int CSmtp::SendMessage(LPTSTR pszAddrFrom, LPTSTR pszAddrTo, LPTSTR pszSubject, LPTSTR pszMessage, LPVOID pvAttachments, DWORD dwAttachmentCount)
{
	CSmtpAddress addrFrom(pszAddrFrom);
	CSmtpAddress addrTo(pszAddrTo);
	
	return SendMessage(addrFrom,addrTo,pszSubject,pszMessage,pvAttachments,dwAttachmentCount);
}

// Tell the SMTP server we're quitting
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendQuitCmd()
{
	int nRet;
	
	if (!m_bConnected) return 0;
	
	nRet = SendCmd(_T("QUIT\r\n"));
	if (nRet != 221) {
		//RaiseError(nRet);
		CString sLog;
		CRotatingLog Log(LOG_FILE_PATH, DATE_FILE_NAME);
		sLog.Format(_T("[SMTP   ][SENDQUIT][01][X] Return Code : %d !!! ### EXIT ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
					nRet, nRet, this->GetSmtpRetMsg(nRet));
		Log.StartAddLineStop(sLog);
	}
	
	m_bConnected = FALSE;
	
	return (nRet == 221) ? 0 : nRet;
}

//####################################################
int CSmtp::SendHello()
{
	int nRet = 0;
	TCHAR szName[64];
	TCHAR szMsg[MAX_PATH];
	DWORD dwSize = 64;
	
	//원래는 해당 클라이언트 컴퓨터이름을 사용해야 하는데
	//컴퓨터 이름이 한글인 경우는 SMTP 서버에서 인식되지 않는다.
	//따라서 그냥 KOLAS2로 설정하여 사용한다.
	//GetComputerName(szName,&dwSize);
	_tcscpy(szName, _T("KOLAS2"));

	CString sLog;
	CRotatingLog Log(LOG_FILE_PATH, DATE_FILE_NAME);
	
	// First try a EHLO if we're using ESMTP
	wsprintf(szMsg,_T("EHLO %s\r\n"),szName);
	if (m_bExtensions) {
		//sLog.Format("[SMTP   ][HELLO   ][01][-] Send command : %s !!!", szMsg);
		//Log.StartAddLineStop(sLog);
		nRet = SendCmd(szMsg);
	} 

	// If we got a 250 response, we're using ESMTP, otherwise revert to regular SMTP
	if (nRet != 250) {
		m_bUsingExtensions = FALSE;
		Log.StartAddLine("[SMTP   ][HELLO   ][02][-] Return Cod is NOT 250, So Using Extensions = FALSE !!!");

		szMsg[0] = 'H';
		szMsg[1] = 'E';

		sLog.Format(_T("[SMTP   ][HELLO   ][03][-] Send command : %s !!!"), szMsg);
		Log.AddLineStop(sLog);
		
		nRet = SendCmd(szMsg);
		if (nRet != 250) {
			//RaiseError(nRet);
			sLog.Format(_T("[SMTP   ][HELLO   ][03][X] Return Cod is NOT 250(%d) !!! ### EXIT ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
						nRet, nRet, this->GetSmtpRetMsg(nRet));
			Log.StartAddLineStop(sLog);
			return nRet;
		}
	} else {
		m_bUsingExtensions = TRUE;
		Log.StartAddLineStop("[SMTP   ][HELLO   ][02][-] Using Extensions = TRUE !!!");
	}

	// We're connected!
	m_bConnected = TRUE;
	Log.StartAddLineStop("[SMTP   ][HELLO   ][03][-] Connected = TRUE !!!");

	// Send authentication if we have any.
	// We don't fail just because authentication failed, however.
	if (m_bUsingExtensions || m_strAccountYN ==_T("Y")) {
		Log.StartAddLineStop("[SMTP   ][HELLO   ][04][-] Now get authentication !!! !!!");
		nRet = SendAuthentication();
		if ( nRet ) return nRet;
	}
	
	return 0;
}
//####################################################


//#######################################################################
int CSmtp::SendAuthentication()
{
	USES_CONVERSION;
	int nRet = 0;
	CBase64 cvt;
	LPCSTR pszTemp;
	TCHAR szMsg[MAX_PATH];
	CHAR szAuthType[MAX_PATH];
	
	CString sLog;
	CRotatingLog Log(LOG_FILE_PATH, DATE_FILE_NAME);

	// This is an authentication loop, we can authenticate multiple times in case of failure.
	while (1) 
	{
		// If we don't have a username, skip authentication
		if (!m_strUser.length()) {
			Log.StartAddLineStop("[SMTP   ][AUTH    ][01][X] User name is EMPTY !!! ### EXIT ###");
			return 0;
		}
		
		Log.StartAddLineStop("[SMTP   ][AUTH    ][02][-] Send command : AUTH LOGIN !!!");
		// Make the authentication request
		nRet = SendCmd(_T("AUTH LOGIN\r\n"));
		// If it was rejected, we have to abort.
		if (nRet != 334) {
			//RaiseWarning(nRet);
			//sLog.Format("[SMTP   ][AUTH    ][02][X] Return Code : %d !!! ### EXIT ###", nRet);
			sLog.Format(_T("[SMTP   ][AUTH    ][02][X] Return Code : %d !!! ### EXIT ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
						nRet, nRet, this->GetSmtpRetMsg(nRet));
			Log.StartAddLineStop(sLog);
			return nRet;
		}
		
		// Authentication has 2 stages for username and password.
		// It is possible if the authentication fails here that we can
		// resubmit proper credentials.
		while (1) 
		{
			// Decode the authentication string being requested
			_T2A(szAuthType,&(m_strResult.c_str())[4]);
			
//#ifdef _UNICODE
//		    int len;
//			len  =  MultiByteToWideChar(CP_ACP,0,szAuthType,-1,NULL,0);
//			WCHAR* wtmp = new WCHAR[len];
//			MultiByteToWideChar(CP_ACP,0,szAuthType,-1,wtmp,len);
//			cvt.Decode(wtmp);
//			delete []wtmp;
//			
//			len = WideCharToMultiByte(CP_ACP, 0, cvt.DecodedMessage(), -1, NULL, 0, NULL,NULL);
//			char* ctmp = new char[len]; 
//			// 실제 변환
//			WideCharToMultiByte(CP_ACP, 0, cvt.DecodedMessage(), -1, ctmp, len, NULL, NULL);
//			pszTemp = ctmp;
//			
//			if (!lstrcmpiA(pszTemp,"Username:"))
//			{
//				cvt.Encode(m_strUser.c_str());
//			}
//			else if (!lstrcmpiA(pszTemp,"Password:"))
//			{
//				cvt.Encode(m_strPass.c_str());
//			}
//			else 
//			{
//				delete []ctmp;
//				break;
//			}
//			delete []ctmp;
//			wsprintf(szMsg,_T("%s\r\n"),cvt.EncodedMessage());
//#else
			cvt.Decode(szAuthType);
			pszTemp = cvt.DecodedMessage();
			
			if (!lstrcmpiA(pszTemp,"Username:"))
				cvt.Encode(T2CA(m_strUser.c_str()));
			else if (!lstrcmpiA(pszTemp,"Password:"))
				cvt.Encode(T2CA(m_strPass.c_str()));
			else break;
			wsprintf(szMsg,_T("%s\r\n"),A2CT(cvt.EncodedMessage()));
// #endif
			sLog.Format(_T("[SMTP   ][AUTH    ][03][-] Send auth command : %s !!!\n"), szMsg);
			Log.StartAddLineStop(sLog);
			
			nRet = SendCmd(szMsg);
			
			// If we got a failed authentication request, raise a warning.
			// this gives the owner a chance to change the username and password.
			if (nRet == 535) 
			{
				//sLog.Format("[SMTP   ][AUTH    ][03][X] Return Code : %d, Authentication request FAIL !!! ### BREAK ###", nRet);
				sLog.Format(_T("[SMTP   ][AUTH    ][03][X] Return Code : %d, Authentication request FAIL !!! ### BREAK ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
							nRet, nRet, this->GetSmtpRetMsg(nRet));
				Log.StartAddLineStop(sLog);
				// Return FALSE to fail, or TRUE to retry
				//nRet = RaiseWarning(nRet);
				//if (!nRet) {
					// Reset the error back to 535.  It's now an error rather than a warning
					//nRet = 535;
					break;
				//}
			}
			// Break on any response other than 334, which indicates a request for more information
			if (nRet != 334) {
				//sLog.Format("[SMTP   ][AUTH    ][03][X] Return Code : %d !!! ### BREAK ###", nRet);
				sLog.Format(_T("[SMTP   ][AUTH    ][03][X] Return Code : %d !!! ### BREAK ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
							nRet, nRet, this->GetSmtpRetMsg(nRet));
				Log.StartAddLineStop(sLog);
				break;
			}
		}
		// Break if we're not retrying a failed authentication
		if (nRet != TRUE) {
			//sLog.Format("[SMTP   ][AUTH    ][03][X] Return Code : %d !!! ### BREAK ###", nRet);
			sLog.Format(_T("[SMTP   ][AUTH    ][03][X] Return Code : %d !!! ### BREAK ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
						nRet, nRet, this->GetSmtpRetMsg(nRet));
			Log.StartAddLineStop(sLog);
			break;
		}
	}
	// Raise an error if we failed to authenticate
	if (nRet != 235) {
		//RaiseError(nRet);
		//sLog.Format("[SMTP   ][AUTH    ][03][X] Return Code : %d, Finally Authentication request FAIL !!! ### EXIT ###", nRet);
		sLog.Format(_T("[SMTP   ][AUTH    ][03][X] Return Code : %d, Finally Authentication request FAIL !!! ### EXIT ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
					nRet, nRet, this->GetSmtpRetMsg(nRet));
		Log.StartAddLineStop(sLog);
	} else {
		sLog.Format(_T("[SMTP   ][AUTH    ][03][X] Return Code : %d, Finally Authentication request SUCCESS !!!"), nRet);
		Log.StartAddLineStop(sLog);
	}

	// 2005.12.17 ADD BY PDJ
	// 인증 실패 코드처리
	if ( nRet == 535 ) return nRet;
	return (nRet == 235) ? 0 : nRet;
}
//#######################################################################

// Send a MAIL FROM command to the server
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendFrom(LPTSTR pszFrom)
{
	int nRet = 0;
	TCHAR szMsg[MAX_PATH];
	
	wsprintf(szMsg,_T("MAIL FROM: <%s>\r\n"),pszFrom);
	
	while (1)
	{
		nRet = SendCmd(szMsg);
		// Send authentication if required, and retry the command
		if (nRet == 530) nRet = SendAuthentication();
		else break;
	}
	// Raise an error if we failed
	//if (nRet != 250) RaiseError(nRet);
	return (nRet == 250) ? 0 : nRet;
}

// Send a RCPT TO command to the server
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendTo(LPTSTR pszTo)
{
	int nRet;
	TCHAR szMsg[MAX_PATH];
	
	wsprintf(szMsg,_T("RCPT TO: <%s>\r\n"),pszTo);
	
	nRet = SendCmd(szMsg);
	//if (nRet != 250 && nRet != 251) RaiseWarning(nRet);
	return (nRet == 250 || nRet == 251) ? 0:nRet;
}

// Send the body of an e-mail message to the server
// Returns 0 if successful, or a positive
// error value if the SMTP server gave an error or failure response.
int CSmtp::SendData(CSmtpMessage &msg)
{
  int nRet;
  String strMsg;

  CString sLog;
  CRotatingLog Log(LOG_FILE_PATH, DATE_FILE_NAME);

  Log.StartAddLineStop("[SMTP   ][SENDDATA][01][-] Send the DATA command !!!");

  // Send the DATA command.  We need a 354 to proceed
  nRet = SendCmd(_T("DATA\r\n"));

  if (nRet != 354 && nRet != 250) {
	  //sLog.Format("[SMTP   ][SENDDATA][01][X] Return Code : %d !!! ### EXIT ###", nRet);
	  sLog.Format(_T("[SMTP   ][SENDDATA][01][X] Return Code : %d !!! ### EXIT ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
				  nRet, nRet, this->GetSmtpRetMsg(nRet));
	  Log.StartAddLineStop(sLog);
	  //RaiseError(nRet);
      return nRet;
  }

  sLog.Format(_T("[SMTP   ][SENDDATA][01][O] Return Code : %d !!!"), nRet);
  Log.StartAddLineStop(sLog);
  
  // Parse the body of the email message
  msg.Parse(strMsg);
  strMsg += _T("\r\n.\r\n");

  Log.StartAddLineStop("[SMTP   ][SENDDATA][02][-] Send the MESSAGE BODY...");
  
  // Send the body and expect a 250 OK reply.
  nRet = SendCmd((LPTSTR)strMsg.c_str());
  if (nRet != 250 && nRet != 354 ) {
	  sLog.Format(_T("[SMTP   ][SENDDATA][02][X] Return Code : %d !!! ### EXIT ###\n### SMTP RETURN MESSAGE : %d \n### %s"),
				  nRet, nRet, this->GetSmtpRetMsg(nRet));
	  Log.StartAddLineStop(sLog);
	  //RaiseError(nRet);
  }
  sLog.Format(_T("[SMTP   ][SENDDATA][02][O] Return Code : %d !!!"), nRet);
  Log.StartAddLineStop(sLog);

  return (nRet == 250 || nRet == 354) ? 0:nRet;
}

CString CSmtp::GetErrorMsg(DWORD dwCode)
{
	CString sErrorMsg;
    
    switch ( dwCode ) 
	{
	case 10036 : sErrorMsg = "WSAEINPROGRESS";		break;
	case 10037 : sErrorMsg = "WSAEALREADY";			break;
	case 10038 : sErrorMsg = "WSAENOTSOCK";			break;
	case 10039 : sErrorMsg = "WSAEDESTADDRREQ";		break;
	case 10040 : sErrorMsg = "WSAEMSGSIZE";			break;
	case 10041 : sErrorMsg = "WSAEPROTOTYPE";		break;
	case 10042 : sErrorMsg = "WSAENOPROTOOPT";		break;
	case 10043 : sErrorMsg = "WSAEPROTONOSUPPORT";	break;
	case 10044 : sErrorMsg = "WSAESOCKTNOSUPPORT";	break;
	case 10045 : sErrorMsg = "WSAEOPNOTSUPP";		break;
	case 10046 : sErrorMsg = "WSAEPFNOSUPPORT";		break;
	case 10047 : sErrorMsg = "WSAEAFNOSUPPORT";		break;
	case 10048 : sErrorMsg = "WSAEADDRINUSE";		break;
	case 10049 : sErrorMsg = "WSAEADDRNOTAVAIL";	break;
	case 10050 : sErrorMsg = "WSAENETDOWN";			break;
	case 10051 : sErrorMsg = "WSAENETUNREACH";		break;
	case 10052 : sErrorMsg = "WSAENETRESET";		break;
	case 10053 : sErrorMsg = "WSAECONNABORTED";		break;
	case 10054 : sErrorMsg = "WSAECONNRESET";		break;
	case 10055 : sErrorMsg = "WSAENOBUFS";			break;
	case 10056 : sErrorMsg = "WSAEISCONN";			break;
	case 10057 : sErrorMsg = "WSAENOTCONN";			break;
	case 10058 : sErrorMsg = "WSAESHUTDOWN";		break;
	case 10059 : sErrorMsg = "WSAETOOMANYREFS";		break;
	case 10060 : sErrorMsg = "WSAETIMEDOUT";		break;
	case 10061 : sErrorMsg = "WSAECONNREFUSED";		break;
	case 10062 : sErrorMsg = "WSAELOOP";			break;
	case 10063 : sErrorMsg = "WSAENAMETOOLONG";		break;
	case 10064 : sErrorMsg = "WSAEHOSTDOWN";		break;
	case 10065 : sErrorMsg = "WSAEHOSTUNREACH";		break;
	case 10066 : sErrorMsg = "WSAENOTEMPTY";		break;
	case 10067 : sErrorMsg = "WSAEPROCLIM";			break;
	case 10068 : sErrorMsg = "WSAEUSERS";			break;
	case 10069 : sErrorMsg = "WSAEDQUOT";			break;
	case 10070 : sErrorMsg = "WSAESTALE";			break;
	case 10071 : sErrorMsg = "WSAEREMOTE";			break;
	case 10093 : sErrorMsg = "WSANOTINITIALISED";	break;
	default    : sErrorMsg.Format(_T("%u"), dwCode); break;
    }

	return sErrorMsg;
}


CString CSmtp::GetSmtpRetMsg(int nRet)
{
	CString sSmtpRetMsg;
    
    switch ( nRet ) 
	{
	case 211 : sSmtpRetMsg = "System status, or system help reply"; break;
	case 214 : sSmtpRetMsg = "Help message\n"
							 "  (Information on how to use the receiver or the meaning of a particular non-standard command;\n"
							 "   this reply is useful only to the human user)"; break;
	case 220 : sSmtpRetMsg = "<domain> Service ready"; break;
	case 221 : sSmtpRetMsg = "<domain> Service closing transmission channel"; break;
	case 250 : sSmtpRetMsg = "Requested mail action okay, completed"; break;
	case 251 : sSmtpRetMsg = "User not local; will forward to <forward-path>"; break;
	case 252 : sSmtpRetMsg = "Cannot VRFY user, but will accept message and attempt delivery"; break;
	case 354 : sSmtpRetMsg = "Start mail input; end with <CRLF>.<CRLF>"; break;
	case 421 : sSmtpRetMsg = "<domain> Service not available, closing transmission channel\n"
							 "   (This may be a reply to any command if the service knows it must shut down)"; break;
	case 450 : sSmtpRetMsg = "Requested mail action not taken: mailbox unavailable(e.g., mailbox busy)"; break;
	case 451 : sSmtpRetMsg = "Requested action aborted: local error in processing"; break;
	case 452 : sSmtpRetMsg = "Requested action not taken: insufficient system storage"; break;
	case 500 : sSmtpRetMsg = "Syntax error, command unrecognized(This may include errors such as command line too long)"; break;
	case 501 : sSmtpRetMsg = "Syntax error in parameters or arguments"; break;
	case 502 : sSmtpRetMsg = "Command not implemented"; break;
	case 503 : sSmtpRetMsg = "Bad sequence of commands"; break;
	case 504 : sSmtpRetMsg = "Command parameter not implemented"; break;
	case 550 : sSmtpRetMsg = "Requested action not taken: mailbox unavailable\n"
								"(e.g., mailbox not found, no access, or command rejected for policy reasons)"; break;
	case 551 : sSmtpRetMsg = "User not local; please try <forward-path>"; break;
	case 552 : sSmtpRetMsg = "Requested mail action aborted: exceeded storage allocation"; break;
	case 553 : sSmtpRetMsg = "Requested action not taken: mailbox name not allowed(e.g., mailbox syntax incorrect)"; break;
	case 554 : sSmtpRetMsg = "Transaction failed  (Or, in the case of a connection-opening response, \"No SMTP service here\")"; break;    
	default  : sSmtpRetMsg.Format(_T("%d : No Message description..."), nRet); break;
	}
	
	return sSmtpRetMsg;
}

