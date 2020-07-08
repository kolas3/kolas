// ChangeValue.cpp: implementation of the CChangeValue class.
// 2008.07.28 MAKE BY PWR
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChangeValue::CChangeValue()
{

}

CChangeValue::~CChangeValue()
{

}

// ** 해당기능(sAbility)의 설정값의 코드를 설명으로 변경한다.
CString CChangeValue::CodeToDesc(CString sAbility, CString sCode)
{
	CString sDesc;
	// 02Dlg (반납연기&예약환경)
	if( sAbility == _T("반납연기사용여부")			 || sAbility == _T("당일대출자료 반납연기허용")	||
		sAbility == _T("반납연기일 자동부여")		 || sAbility == _T("예약자동취소")				||
		sAbility == _T("예약만기일 자동부여")		 || sAbility == _T("예약만기일부여시 SMS발송여부") ||
		sAbility == _T("예약만기일 휴관일 포함유무") || sAbility == _T("예약취소 안내메일 발송여부") || 
		sAbility == _T("우선대출예약기능 사용여부")	 || sAbility == _T("한줄서기예약기능") )
	{
		if( sCode == _T("Y") )
			sDesc = _T("사용함");
		else if( sCode == _T("N") ) 
			sDesc = _T("사용안함");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("반납연기시 예약우선순위 결정") )
	{
		if( sCode == _T("0") )
			sDesc = _T("반납연기우선");
		else if( sCode == _T("1") ) 
			sDesc = _T("예약우선");
		else
			sDesc = _T("반납연기우선");
	}
	else if( sAbility == _T("반납연기일 적용 기준일") )
	{
		if( sCode == _T("0") )
			sDesc = _T("반납예정일");
		else if( sCode == _T("1") ) 
			sDesc = _T("현재일");
		else
			sDesc = _T("반납예정일");
	}
	else if( sAbility == _T("예약기능 사용여부") )
	{
		if( sCode == _T("0") )
			sDesc = _T("사용안함");
		else if( sCode == _T("1") )
			sDesc = _T("대출중인 자료만 예약가능");
		else if( sCode == _T("2") )
			sDesc = _T("모든자료 예약가능");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("반납연기 가능횟수") )// EditBox
	{
		sDesc = sCode + _T("회");
	}
	else if( sAbility == _T("예약허용인원") )
	{
		sDesc = sCode + _T("명");
	}
	//08Dlg (개인정보보호정책)
	else if( sAbility == _T("주민등록번호 표시여부") || sAbility == _T("대출자번호 표시여부") )
	{
		if( sCode == _T("Y") )
			sDesc = _T("표시함");
		else if( sCode == _T("N") )
			sDesc = _T("표시안함");
		else
			sDesc = _T("표시안함");
	}
	//09Dlg (무인대출예약설정)
	else if( sAbility == _T("대출정지일") )// EditBox
	{
		sDesc = sCode + _T("일");
	}
	//01Dlg (대출/반납형식&이용자)
	else if( sAbility == _T("일반대출사용")		|| sAbility == _T("관내대출사용")		
			 || sAbility == _T("특별대출사용")		|| sAbility == _T("장기대출사용")		
			 || sAbility == _T("일반반납사용")		|| sAbility == _T("일괄반납사용")		
			 || sAbility == _T("소급반납사용")		|| sAbility == _T("가족재대출허용")		
			 || sAbility == _T("개인재대출허용")	|| sAbility == _T("재대출이력보여주기")	
			 || sAbility == _T("부록자료대출")		|| sAbility == _T("부록자료반납")		
			 || sAbility == _T("대출책수에 포함")	|| sAbility == _T("가족회원기능")		
			 || sAbility == _T("대출정지일 자동일괄해제/선정") 
			 || sAbility == _T("주민등록번호 자동중복검사")	
			 || sAbility == _T("대출자번호 사용자ID로 자동입력")			// 2009.04.21 ADD BY KSJ
			 || sAbility == _T("주민등록번호 뒷자리 비밀번호로 자동입력") 	// 2009.04.21 ADD BY KSJ
			 // 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility ==  _T("본인인증을 통한 이용자조회 사용여부")
//*/ END ---------------------------------------------------------------------------
			 //JOB.2019.0033 ADD BY KYJ
			 || sAbility ==  _T("타시스템카드번호사용여부")
			)
	{
		if( sCode == _T("Y") )
			sDesc = _T("사용함");
		else if( sCode == _T("N") ) 
			sDesc = _T("사용안함");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("대출자번호 자동부여") )
	{
		if( sCode == _T("N") )
			sDesc = _T("사용안함");
		else if( sCode == _T("Y") )
			sDesc = _T("마지막번호");
		else if( sCode == _T("A") )
			sDesc = _T("주민등록번호");
		else if( sCode == _T("U") )
			sDesc = _T("사용자ID");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("영수증출력사용") )
	{
		if( sCode == _T("YY") )
			sDesc = _T("대출,반납 모두");
		else if( sCode.GetAt(0) == 'Y' )
			sDesc = _T("대출일때만");
		else if( sCode.GetAt(1) == 'Y' )
			sDesc = _T("반납일때만");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("재대출제한일") )// EditBox
	{
		sDesc = sCode + _T("일");
	}
	//03Dlg (연체자료)
	else if( sAbility == _T("대출정지일부여")	|| sAbility == _T("부록연체시 대출정지일부여") || 
			 sAbility == _T("연체료부여")		|| sAbility == _T("대출정지일 휴관일포함") 
			 // 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility == _T("연체기간 휴관일포함") 			 
//*/ END ---------------------------------------------------------------------------	
			 )
	{
		if( sCode == _T("Y") )
			sDesc = _T("사용함");
		else if( sCode == _T("N") ) 
			sDesc = _T("사용안함");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("대출정지일 부여기준") )
	{
		if( sCode == _T("0") )
			sDesc = _T("연체일중 가장 큰 연체일");
		else if( sCode == _T("1") )
			sDesc = _T("각 자료의 연체일 합");
		else
			sDesc = _T("각 자료의 연체일 합");
	}
	else if( sAbility == _T("연체일가감일1") || sAbility == _T("연체일가감일2") )
	{
		sDesc = sCode;
	}
	else if( sAbility == _T("누적 연체횟수 부여방법") )
	{
		if( sCode == _T("0") )
			sDesc = _T("대출정지부여일을 기준으로");
		else if( sCode == _T("1") )
			sDesc = _T("연체된 책수로 부여");
		else
			sDesc = _T("대출정지부여일을 기준으로");
	}
	else if( sAbility == _T("연체일 가중치") )// EditBox
	{
		sDesc = sCode;
	}
	else if( sAbility == _T("기본연체료") )// EditBox
	{
		sDesc = sCode + _T("원");
	}	
	else if( sAbility == _T("최대대출정지부여일수") )// EditBox
	{
		sDesc = sCode + _T("일");
	}
	//07Dlg (통합대출관리)
	else if( sAbility == _T("이용자검색시 대출정지회원 안내") || sAbility == _T("이용자검색시 제적회원 안내") )
	{
		if( sCode == _T("0") )
			sDesc = _T("사용안함");
		else if( sCode == _T("1") )
			sDesc = _T("경고메시지");
		else if( sCode == _T("2") )
			sDesc = _T("대출제한");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("센터반입시 ID, 비밀번호 반입여부") || sAbility == _T("통합대출책수 사용여부") )
	{
		if( sCode == _T("Y") )
			sDesc = _T("사용함");
		else if( sCode == _T("N") ) 
			sDesc = _T("사용안함");
		else
			sDesc = _T("사용안함");
	}
	//10Dlg (수수료관리)
	else if( sAbility == _T("부록연체시 대출정지일 부여") || sAbility == _T("대출정지일 휴관일 포함유무") 
			// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			|| sAbility == _T("연체기간 휴관일 포함유무")
//*/ END ---------------------------------------------------------------------------
			)
	{
		if( sCode == _T("Y") )
			sDesc = _T("사용함");
		else if( sCode == _T("N") ) 
			sDesc = _T("사용안함");
		else
			sDesc = _T("사용안함");
	}
	else if( sAbility == _T("기본 연체료") )// EditBox
	{
		sDesc = sCode + _T("원");
	}	
	else if( sAbility == _T("도서관별 등록구분 중복여부"))
	{
		if( sCode == _T("Y") )
			sDesc = _T("사용함");
		else if( sCode == _T("N") ) 
			sDesc = _T("사용안함");
		else
			sDesc = _T("사용안함");
	}
	else if(sAbility == _T("기본 대출기한"))
	{
		sDesc = sCode + _T("일");
	}

	// 2012.09.27 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
	//08Dlg (회원정보갱신기간)
	else if( sAbility == _T("회원정보 갱신기간 설정") )// EditBox
	{
		sDesc = sCode + _T("일");
	}
	
	return sDesc;
}

// ** 해당기능(sAbility)의 설정값의 설명을 코드로 변경한다.
CString CChangeValue::DescToCode(CString sAbility, CString sDesc)
{
	CString sCode;
	// 02Dlg (반납연기&예약환경)
	if( sAbility == _T("반납연기사용여부")			 || sAbility == _T("당일대출자료 반납연기허용")	||
		sAbility == _T("반납연기일 자동부여")		 || sAbility == _T("예약자동취소")				||
		sAbility == _T("예약만기일 자동부여")		 || sAbility == _T("예약만기일부여시 SMS발송여부") ||
		sAbility == _T("예약만기일 휴관일 포함유무") || sAbility == _T("예약취소 안내메일 발송여부") || 
		sAbility == _T("한줄서기예약기능") )
	{
		if( sDesc == _T("사용함") )
			sCode = _T("Y");
		else if( sDesc == _T("사용안함") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("반납연기시 예약우선순위 결정") || sAbility == _T("반납연기일 적용 기준일") )
	{
		if( sDesc == _T("반납연기우선") || sDesc == _T("반납예정일") )
			sCode = _T("0");
		else if( sDesc == _T("예약우선") || sDesc == _T("현재일") ) 
			sCode = _T("1");
		else
			sCode = _T("0");
	}
	else if( sAbility == _T("예약기능 사용여부") )
	{
		if( sDesc == _T("사용안함") )
			sCode = _T("0");
		else if( sDesc == _T("대출중인 자료만 예약가능") )
			sCode = _T("1");
		else if( sDesc == _T("모든자료 예약가능") )
			sCode = _T("2");
		else
			sCode = _T("0");
	}
	//08Dlg (개인정보보호정책)
	else if( sAbility == _T("주민등록번호 표시여부") || sAbility == _T("대출자번호 표시여부") )
	{
		if( sDesc == _T("표시함") )
			sCode = _T("Y");
		else if( sDesc == _T("표시안함") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	//01Dlg (대출/반납형식&이용자)
	else if( sAbility == _T("일반대출사용")		|| sAbility == _T("관내대출사용")		
			 || sAbility == _T("특별대출사용")		|| sAbility == _T("장기대출사용")		
			 || sAbility == _T("일반반납사용")		|| sAbility == _T("일괄반납사용")		
			 || sAbility == _T("소급반납사용")		|| sAbility == _T("가족재대출허용")		
			 || sAbility == _T("개인재대출허용")	|| sAbility == _T("재대출이력보여주기")	
			 || sAbility == _T("부록자료대출")		|| sAbility == _T("부록자료반납")		
			 || sAbility == _T("대출책수에 포함")	|| sAbility == _T("가족회원기능")		
			 || sAbility == _T("대출정지일 자동일괄해제/선정")	
			 || sAbility == _T("주민등록번호 자동중복검사")	
			 || sAbility == _T("대출자번호 사용자ID로 자동입력")			// 2009.04.21 ADD BY KSJ
			 || sAbility == _T("주민등록번호 뒷자리 비밀번호로 자동입력")	// 2009.04.21 ADD BY KSJ)
			 // 17/05/18 김혜영 : 회원증 미지참자의 신분 확인을 위하여 본인인증기능 추가
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility ==  _T("본인인증을 통한 이용자조회 사용여부")
//*/ END ---------------------------------------------------------------------------
			 //JOB.2019.0033 ADD BY KYJ
			 || sAbility ==  _T("타시스템카드번호사용여부")
			)
	{
		if( sDesc == _T("사용함") )
			sCode = _T("Y");
		else if( sDesc == _T("사용안함") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("대출자번호 자동부여") )
	{
		if( sDesc == _T("사용안함") )
			sCode = _T("N");
		else if( sDesc == _T("마지막번호") )
			sCode = _T("Y");
		else if( sDesc == _T("주민등록번호") )
			sCode = _T("A");
		else if( sDesc == _T("사용자ID") )
			sCode = _T("U");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("영수증출력사용") )
	{
		if( sDesc == _T("대출,반납 모두") )
			sCode = _T("YY");
		else if( sDesc == _T("대출일때만") )
			sCode = _T("YN");
		else if( sDesc == _T("반납일때만") )
			sCode = _T("NY");
		else
			sCode = _T("NN");
	}
	//03Dlg (연체자료)
	else if( sAbility == _T("대출정지일부여")	|| sAbility == _T("부록연체시 대출정지일부여") || 
			 sAbility == _T("연체료부여")		|| sAbility == _T("대출정지일 휴관일포함") 
			 // 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
		     || sAbility == _T("연체기간 휴관일포함") 			 
//*/ END ---------------------------------------------------------------------------	
			)
	{
		if( sDesc == _T("사용함") )
			sCode = _T("Y");
		else if( sDesc == _T("사용안함") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("대출정지일 부여기준") )
	{
		if( sDesc == _T("연체일중 가장 큰 연체일") )
			sCode = _T("0");
		else if( sDesc == _T("각 자료의 연체일 합") )
			sCode = _T("1");
		else
			sCode = _T("1");
	}
	else if( sAbility == _T("연체일가감일1") || sAbility == _T("연체일가감일2") )
	{
		sCode = sDesc;
	}
	else if( sAbility == _T("누적 연체횟수 부여방법") )
	{
		if( sDesc == _T("대출정지부여일을 기준으로") )
			sCode = _T("0");
		else if( sDesc == _T("연체된 책수로 부여") )
			sCode = _T("1");
		else
			sCode = _T("0");
	}
	//07Dlg (통합대출관리)
	else if( sAbility == _T("이용자검색시 대출정지회원 안내") || sAbility == _T("이용자검색시 제적회원 안내") )
	{
		if( sDesc == _T("사용안함") )
			sCode = _T("0");
		else if( sDesc == _T("경고메시지") )
			sCode = _T("1");
		else if( sDesc == _T("대출제한") )
			sCode = _T("2");
		else
			sCode = _T("0");
	}
	else if( sAbility == _T("센터반입시 ID, 비밀번호 반입여부") || sAbility == _T("통합대출책수 사용여부") )
	{
		if( sDesc == _T("사용함") )
			sCode = _T("Y");
		else if( sDesc == _T("사용안함") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	//10Dlg (수수료관리)
	else if( sAbility == _T("부록연체시 대출정지일 부여") || sAbility == _T("대출정지일 휴관일 포함유무") 
			// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility == _T("연체기간 휴관일 포함유무") 			 
//*/ END ---------------------------------------------------------------------------	
			)
	{
		if( sDesc == _T("사용함") )
			sCode = _T("Y");
		else if( sDesc == _T("사용안함") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("도서관별 등록구분 중복여부"))
	{
		if( sDesc == _T("사용함") )
			sCode = _T("Y");
		else if( sDesc == _T("사용안함") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	return sCode;
}

