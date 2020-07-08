<html>
<body>

<?php

// Include ********************************************
require ("dbConn.inc");
include "function.inc";

//echo(" <script>
//       window.alert('$output')
//       </script>");
       
       
    $arrData ;
    $nCount = 0;   
	$string = $output;
	$token  = strtok($string, "[,]");
	while ($token !== false) {
	  //echo("$token<br>");
	  
	  $arrData[$nCount] = $token ;
	  
	  $token = strtok("[,]");
	  $nCount ++ ;
	  
	  //
	  
	}

	//GLOBAL $research_key;
	//$research_key = $arrData[0] ;
	
	
// oci class 인스턴스 *********************************
	if (class_exists(DBCONN)) {
		$oci = new DBCONN();
	}
	else {
		MessageAlertPage(" DBCONN class 초기화 실패 ");
		exit;
	}
// **************************************************************************
	
// query 만들기 *************************************************************	
	$nQueryCnt = 0 ;
	if($nCount > 1) 
	{
		for ($i = 1 ; $i < $nCount ; $i++)
		{
			$arrQuery[$i-1] = make_query($oci,$arrData[0],$arrData[$i]) ;		
			//echo("<p><p>arrQuery : $arrQuery[$i]") ;
			$nQueryCnt++ ;
		}
	}


// **************************************************************************       
       
// **************************************************************************       
// query 실행 하기        
	
	for ($k = 0 ;$k < $nQueryCnt ; $k++)
	{
		//echo("<p><p>arrQuery : $arrQuery[$k]") ;
		if (!$oci->insert($arrQuery[$k])) {
			//MessagePage("$qry");
			//MessageAlertPage(" CIP 정보 DB Insert 오류 ");
			echo(" <script>
       		window.alert('DB Insert 오류')
       		</script>");
			exit(1);
		}
		
		//echo("<p><p>arrQuery : $arrQuery[$k]") ;
	}

	$oci->commit();
    $oci->parseFree();
       
       
// **************************************************************************       
       

// **************************************************************************       
// Disconnect         
       
// **************************************************************************              
	echo("<p><p><p>설문 등록이 정상적으로 처리 되었습니다.") ;       
    echo("<p>설문에 참여하여 주셔서 감사 합니다..") ;          
php?>

</body>
</html>