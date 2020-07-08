#include <stdafx.h>
#include "string_fn.h"

INT GetToken(_TCHAR* line, INT pos, _TCHAR* token)
{
	if ( pos < 0 ) {
		token[0]=NULL;
		return -1;
	} else if ( line[pos] == _T('\n') ) {
		token[0]=NULL;
		return -1;
	}

	INT end;

	// 처음의 공백과 tab을 제거한다
	while ( true ) {
		if ( line[pos] == _T(' ') ) {

		} else if ( line[pos] == _T('\t') ) {

		} else if ( line[pos] == _T('\r') ) {
			token[0]=NULL;
			return -1;

		} else if ( line[pos] == NULL ) {
			token[0]=NULL;
			return -1;
		
		} else {
			break;
		}
		pos++;
	}

	// token의 끝을 찾는다
	end = pos;

	while ( true ) {
		if ( line[end] == _T(' ') ) {
			_tcsncpy(token, line+pos, end-pos);
			token[end-pos] = 0;
			break;

		} else if ( line[end] == _T('\t') ) {
			_tcsncpy(token, line+pos, end-pos);
			token[end-pos] = 0;
			break;

		} else if ( line[end] == _T('\r') ) {
			_tcsncpy(token, line+pos, end-pos);
			token[end-pos] = 0;
			break;

		} else if ( line[end] == NULL ) {
			_tcsncpy(token, line+pos, end-pos);
			token[end-pos] = 0;
			break;
		}
		end++;
	}

	return end+1;
}


INT DevideString(CString str, CString pattern, CStringArray& array)
{
	array.RemoveAll();
	if ( str.IsEmpty() ) return 0;
	if ( pattern.IsEmpty() ) return -1;

	CString piece;
	INT left = 0;
	INT pos;
	pos = str.Find(pattern);
	while ( pos != -1 ) {

		piece = str.Mid(left, pos-left);
		array.Add(piece);

		left = pos + pattern.GetLength();
		pos = str.Find(pattern, left);
	}

	INT len = str.GetLength();
	if ( left < len + 1) {
		piece = str.Mid(left, len-left);
		array.Add(piece);
	}

	return 0;
}
