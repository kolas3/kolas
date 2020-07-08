
///////////////////////////////////////////////////////////////////////////////
// Mail Manger Dialog를 띄우고 보낼 메일을 체크하여 보낼 때
///////////////////////////////////////////////////////////////////////////////

VOID CMailMgrTestDlg::OnbMailTest() 
{
	///Mail Manager 객체 생성
	CSendMailMgr MailMgr(this);

	///Mail Record 객체 생성
	SendMailRecord MailRecord;

	///Mail Record 객체 설정
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>첫번째</title></head><body>첫번째 내용</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//전자우편
	MailRecord.m_sRecipientName	 = _T("박길주");	//신청자이름
	MailRecord.m_sSubject = _T("CIP 반송 - 첫번째");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record 객체 추가
	MailMgr.AddMailRecord(&MailRecord);

	///Mail Record 객체 설정
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>두번째</title></head><body>두번째 내용</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//전자우편
	MailRecord.m_sRecipientName	 = _T("박길주");	//신청자이름
	MailRecord.m_sSubject = _T("CIP 반송 - 두번째");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record 객체 추가
	MailMgr.AddMailRecord(&MailRecord);

	///Mail Manager Dialog 뛰우기
	MailMgr.DoModal();

	///결과 확인
	///전송 성공한 메일의 개수
	INT nCntSended = MailMgr.GetCountMailSended();
	///전송 실패한 메일의 개수
	INT nCntFailed = MailMgr.GetCountMailFailed();
}


///////////////////////////////////////////////////////////////////////////////
// Mail Manger Dialog를 띄우지 않고 Background로 메일을 보낼 때
///////////////////////////////////////////////////////////////////////////////

VOID CMailMgrTestDlg::OnbMailTestBackground() 
{
	///Mail Manager 객체 생성
	CSendMailMgr MailMgr(this);
	
	///Mail Record 객체 생성
	SendMailRecord MailRecord;

	///Mail Record 객체 설정
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>첫번째</title></head><body>첫번째 내용</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//전자우편
	MailRecord.m_sRecipientName	 = _T("박길주");	//신청자이름
	MailRecord.m_sSubject = _T("CIP 반송 - 첫번째");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record 객체 추가
	MailMgr.AddMailRecord(&MailRecord);
	
	///Mail Record 객체 설정
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>두번째</title></head><body>두번째 내용</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//전자우편
	MailRecord.m_sRecipientName	 = _T("박길주");	//신청자이름
	MailRecord.m_sSubject = _T("CIP 반송 - 두번째");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record 객체 추가
	MailMgr.AddMailRecord(&MailRecord);

	INT ids;

	///메일 서버와 보내는 사람 메일 주소 설정 >>> 아래 두가지 방식
	{
		///직접 메일서버와 보내는 사람을 설정하는 경우
		MailMgr.SetDefaultMailServer(_T("mail.eco.co.kr"), _T("gjpark@eco.co.kr"));
		
		///Registry 정보에서 메일서버와 보내는 사람을 설정하는 경우
		//ids = MailMgr.GetDefaultMailServerInfo(TRUE);	// <<< 반드시 FALSE로 매개변수를 넘겨야 한다.
		//if (0>ids)
		//{
		//	AfxMessageBox(_T("메일서버를 설정할 수 없습니다."));
		//	return;
		//}
	}
	
	///메일 전송
	ids = MailMgr.SendMail(FALSE, TRUE);	// <<< 반드시 FALSE, TRUE로 매개변수를 넘겨야 한다.
	if (0>ids)
	{
		AfxMessageBox(_T("메일전송 중 내부오류가 발생하였습니다."));
		return;
	}
	
	///결과 확인
	CString sTemp;
	sTemp.Format(_T("메일전송이 완료되었습니다. [전송:%d / 실패:%d]"), MailMgr.GetCountMailSended(), MailMgr.GetCountMailFailed());
	MessageBox(sTemp, _T("확인"), MB_ICONINFORMATION);
}
