<?


function make_query ($oci ,$research_key,$strdata)
{
	
	
//	GLOBAL $research_key ;
	
	$query = checktype($oci ,$research_key,$strdata) ;

	
	return $query ;
}

function checktype($oci ,$research_key,$strdata)
{
	$qeury ;
   
	$nCount = 0 ;
	$token  = strtok($strdata, ";");
	while ($token !== false) 
	{
	
		$arrData[$nCount] = $token ;
		$token = strtok(";");
		$nCount ++ ;
	}
	

	//$rec_key = make_reckey() ;
	
	
	// Rec_key 생성 *************************************************************
	
	$qry = "SELECT ESL_SEQ.nextval from dual";
	$oci->select($qry);
	if ($oci->fetch())
		$rec_key=$oci->result(1);
	
	if (empty($rec_key)) {
		//MessageAlertPage(" CIP Primary Key 생성오류 ");
		exit;
	}
	$oci->parseFree();
	

	// **************************************************************************
	


	$create_Date = $today = date("Y/m/d");                           
	//echo(" create_date = $create_Date <br> ") ;
	//echo("$token<br>");


	if(trim($arrData[0]) == '단일선택형'  )
	{
		$query = "insert into research_result_tbl (rec_key,research_key,question_info_key,type,selectvalue,createdate) values (	$rec_key ,$research_key,$arrData[1],'단일선택형','$arrData[2]','$create_Date' )" ;
	}
	else if ( trim($arrData[0]) == '복수선택형')
	{
			$query = "insert into research_result_tbl (rec_key,research_key,question_info_key,type,selectvalue,createdate) values (	$rec_key ,$research_key,$arrData[1],'복수선택형','$arrData[2]','$create_Date' )" ;
	}
	else if(trim($arrData[0]) == '서술형' )
	{
		$query = "insert into research_result_tbl (rec_key,research_key,question_info_key,type,disc,createdate) values ($rec_key ,$research_key,$arrData[1],'서술형','$arrData[2]','$create_Date' )";
		
	}
	else if(trim($arrData[0]) == '절대점수형' )
	{
		$query = "insert into research_result_tbl (rec_key,research_key,question_info_key,type,selectvalue,createdate,SAMPLEKEY ) values ($rec_key ,$research_key,$arrData[1],'절대점수형','$arrData[2]','$create_Date',$arrData[3] )";
		
	}
	else if(trim($arrData[0]) == '우선순위형' )
	{
		$query = "insert into research_result_tbl (rec_key,research_key,question_info_key,type,samplekey,samplevalue,createdate) values ($rec_key ,$research_key,$arrData[1],'우선순위형',$arrData[2],'$arrData[3]','$create_Date' )";
		//echo("우선순위형 쿼리 $query<br>");
	}
	else if (trim($arrData[0]) == '매트릭스형')
	{
		$query = "insert into research_result_tbl (rec_key,research_key,question_info_key,type,samplekey,samplevalue,createdate) values ($rec_key ,$research_key,$arrData[1],'매트릭스형',$arrData[2],'$arrData[3]','$create_Date' )";
	}
  

	return $query ;
}

function make_reckey()
{
	//$rec_key ='000001';
	
	//$conn = connect() ;
	//$query = "SELECT ESL_SEQ.NEXTVAL  FROM DUAL " ;
	
	//return $rec_key ;	
	
}


?>