<html>
<head>

<style>
/* 기본 폰트 */
body, td, form, input, select, option {font-family:돋움; font-size:12px; color:#6a6a6a; line-height:18px}
.txts						{font-family:돋움; font-size:11px; text-decoration:none; line-height:14px; color:#777777;}
.txts-blk					{font-family:돋움; font-size:11px; text-decoration:none; line-height:14px; color:#404040;}
.txts-orange			{font-family:돋움; font-size:11px; text-decoration:none; line-height:14px; color:#F66B00;}

/* 기본 롤오버 */
a:active		{font-size:12px; color:#6a6a6a; text-decoration:none; line-height:14px;}
a:link			{font-size:12px; color:#6a6a6a; text-decoration:none; line-height:14px;}
a:visited		{font-size:12px; color:#6a6a6a; text-decoration:none; line-height:14px;}
a:hover			{font-size:12px; color:#347fd2; text-decoration:underline; line-height:14px;}

/* 도서 제목 스타일 #1 */
.BookTitle1							{text-decoration:none; line-height:14px;}
.BookTitle1 a:active		{color:#2d2d2d; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle1 a:link			{color:#2d2d2d; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle1 a:visited		{color:#2d2d2d; font-weight:bold; text-decoration:none; line-height:14px;}	
.BookTitle1 a:hover			{color:#51b74a; font-weight:bold; text-decoration:underline; line-height:14px;}

/* 도서 제목 스타일 #2 */
.BookTitle2							{text-decoration:none; line-height:14px;}
.BookTitle2 a:active		{color:#2d2d2d; text-decoration:none; line-height:14px;}
.BookTitle2 a:link			{color:#2d2d2d; text-decoration:none; line-height:14px;}
.BookTitle2 a:visited		{color:#2d2d2d; text-decoration:none; line-height:14px;}	
.BookTitle2 a:hover			{color:#51b74a; text-decoration:underline; line-height:14px;}

/* 가격 , 할인율, 적립금, 출판사 */
.Price				 	{color:#ff620d; font-weight:bold; line-height:14px;}
.PriceThin		 	{color:#ff620d; font-weight:normal; line-height:14px;}
.Discount				{color:509AE5; font-weight:normal; line-height:14px;}
.Bonus					{color:#69a3d3; font-weight:bold; line-height:14px;}
.Publisher			{color:#ababab; font-weight:normal; line-height:14px;}

/* 웰컴 공지 사항 */
.Notice								{color:#6e93bd; text-decoration:none; line-height:14px;}
.Notice a:active			{color:#6e93bd; text-decoration:none; line-height:14px;}
.Notice a:link				{color:#6e93bd; text-decoration:none; line-height:14px;}
.Notice a:visited			{color:#6e93bd; text-decoration:none; line-height:14px;}	
.Notice a:hover				{color:#385776; text-decoration:none; line-height:14px;}

/* 웰컴 오늘의 책소식 */
.BookNews							{color:#9e9180; text-decoration:none; line-height:14px;}
.BookNews a:active		{color:#9e9180; text-decoration:none; line-height:14px;}
.BookNews a:link			{color:#9e9180; text-decoration:none; line-height:14px;}
.BookNews a:visited		{color:#9e9180; text-decoration:none; line-height:14px;}	
.BookNews a:hover			{color:#706353; text-decoration:none; line-height:14px;}

/* 국내도서 메인 이벤트세상 */
.BookEventWorld						{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:active	{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:link		{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:visited	{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:hover		{font-size:11px; color:#347fd2; letter-spacing:-1px; line-height:14px;}

/* 국내도서메인 특가도서 제목 스타일 */
.BookTitle_SP							{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle_SP a:active		{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle_SP a:link			{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle_SP a:visited		{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}	
.BookTitle_SP a:hover			{color:#51b74a; font-weight:bold; text-decoration:underline; line-height:14px;}

/* 검색결과 카테고리 박스 스타일 */
.SearchCate							{color:#7e7e7e; text-decoration:none; line-height:14px;}
.SearchCate a:active		{color:#7e7e7e; text-decoration:none; line-height:14px;}
.SearchCate a:link			{color:#7e7e7e; text-decoration:none; line-height:14px;}
.SearchCate a:visited		{color:#7e7e7e; text-decoration:none; line-height:14px;}	
.SearchCate a:hover			{color:#347fd2; text-decoration:underline; line-height:14px;}

/* 검색결과 카테고리 펼침레이어*/
.SearchCateClass				{display:none;}

/* 페이지 네비게이션 */
.PageNavi							{color:#a8a8a8; font-weight:bold; text-decoration:none; line-height:14px; letter-spacing:-1px;}
.PageNavi a:active		{color:#a8a8a8; font-weight:Normal; text-decoration:none; line-height:14px; letter-spacing:-1px;}
.PageNavi a:link			{color:#a8a8a8; font-weight:Normal; text-decoration:none; line-height:14px; letter-spacing:-1px;}
.PageNavi a:visited		{color:#a8a8a8; font-weight:Normal; text-decoration:none; line-height:14px; letter-spacing:-1px;}	
.PageNavi a:hover			{color:#797979; font-weight:bold; text-decoration:none; line-height:14px; letter-spacing:-1px;}

/* 검색결과 도서 목록*/
.Publisher2			{color:#2d2d2d; font-weight:normal; line-height:14px;}

/* 분류별 리스트 레이어 롤오버 1 */
.GenreList1							{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}
.GenreList1 a:active		{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}
.GenreList1 a:link			{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}
.GenreList1 a:visited		{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}	
.GenreList1 a:hover			{font-size:11px; color:#305b14; text-decoration:none; line-height:14px;}

/* 분류별 리스트 레이어 롤오버 2 */
.GenreList2							{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}
.GenreList2 a:active		{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}
.GenreList2 a:link			{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}
.GenreList2 a:visited		{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}	
.GenreList2 a:hover			{font-size:11px; color:#000000; text-decoration:none; line-height:14px;}

/* 로그인 새창 폼박스 */
.LoginForm		{border:1px #bdbdbd solid;}

/* 회원가입 폼박스 */
.JoinForm		{border:1px #bdbdbd solid;}

/* 주문결제 폼박스 */
.PayForm		{border:1px #dedede solid;}

/* 검색 폼박스 */
.SearchForm		{border:1px #dedede solid;}

/* 쇼케이스 DB Box 부록정보 */
.ShowcaseGift			{color:#ababab; line-height:12px;}

/* 분류 리스트 리뷰 텍스트 */
.ReviewTextMain			{color:#999999;}
.ReviewTextAuthor		{color:#696969;}
.ReviewTextvBar			{color:#d9d9d9;}

/* 단품 상세 바로가기 롤오버 */
.Detail_Link							{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}
.Detail_Link a:active			{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}
.Detail_Link a:link				{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}
.Detail_Link a:visited		{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}	
.Detail_Link a:hover			{font-size:12px; color:#777068; text-decoration:underline; line-height:14px;}

/* 검색결과 소팅버튼 롤오버 */
.Search_Sort							{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}
.Search_Sort a:active			{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}
.Search_Sort a:link				{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}
.Search_Sort a:visited		{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}	
.Search_Sort a:hover			{font-size:12px; color:#ffffff; text-decoration:none; line-height:14px;}

/* 알림 메세지 */
.AlertText				{color:#ff7f31;}

/* 장바구니담기 레이어 롤오버 */
.InCartOn					{font-size:12px; color:#585858; background-color:#f5f5f5; text-decoration:none; line-height:14px; cursor:hand;}
.InCartOff				{font-size:12px; color:#949494; background-color:#ffffff; text-decoration:none; line-height:14px; cursor:hand;}

/* 장바구니담기 레이어 롤오버 작은것 */
.InCartThinOn					{font-size:11px; letter-spacing:-2px; color:#585858; background-color:#f5f5f5; text-decoration:none; line-height:14px; cursor:hand;}
.InCartThinOff				{font-size:11px; letter-spacing:-2px; color:#949494; background-color:#ffffff; text-decoration:none; line-height:14px; cursor:hand;}

/* 마이룸 눈에 띄는 상품 롤오버 */
.Myroom_HotGoods							{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}
.Myroom_HotGoods a:active			{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}
.Myroom_HotGoods a:link				{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}
.Myroom_HotGoods a:visited		{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}	
.Myroom_HotGoods a:hover			{font-size:11px; color:#ffee62; text-decoration:none; line-height:14px;}

/* 회원가입약관 */
.member                 {font-family:돋움; font-size:12px; color:#6a6a6a; line-height:18px}


.reviewText P {MARGIN-TOP: 2px; MARGIN-BOTTOM: 2px;}

P {MARGIN-TOP: 2px; MARGIN-BOTTOM: 2px;}

</style>


</head>

<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">

			<table width="100%" align="center" cellspacing="0" cellpadding="0" border="0">
				<tr><td><img src="http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/tit_todayBest.gif" border="0"></td></tr>
				<tr><td style="padding:3 0 10 14; border-left:#DDDDDD solid 1px;border-right:#DDDDDD solid 1px;">

<SCRIPT language=javascript>
//Math.random()
var scrollerheight=119;  // 스크롤러의 세로 
var html,total_area=0,wait_flag=true; 
var bMouseOver = 1;
var scrollspeed = 1;  // Scrolling 속도         
var waitingtime = 9000;  // 멈추는 시간
var s_tmp = 0, s_amount = 119;
var scroll_content=new Array();
var startPanel=0, n_panel=0, i=0;


function startscroll()
{ // 스크롤 시작
  i=0;
  for (i in scroll_content)
   n_panel++;

   
  n_panel = n_panel -1 ;
  startPanel = Math.round(Math.random()*n_panel);
  if(startPanel == 0)
  {
   i=0;
   for (i in scroll_content) 
    insert_area(total_area, total_area++); // area 삽입
  }
  else if(startPanel == n_panel)
  {
   insert_area(startPanel, total_area);
   total_area++;
   for (i=0; i<startPanel; i++) 
   {
    insert_area(i, total_area); // area 삽입
    total_area++;
   }
  }
  else if((startPanel > 0) || (startPanel < n_panel))
  {
   insert_area(startPanel, total_area);
   total_area++;
   for (i=startPanel+1; i<=n_panel; i++) 
   {
    insert_area(i, total_area); // area 삽입
    total_area++;
   }
   for (i=0; i<startPanel; i++) 
   {
    insert_area(i, total_area); // area 삽입
    total_area++;
   }
  }
  window.setTimeout("scrolling()",waitingtime);
}
function scrolling(){ // 실제로 스크롤 하는 부분
  if (bMouseOver && wait_flag)
  {
   for (i=0;i<total_area;i++){
    tmp = document.getElementById('scroll_area'+i).style;
    tmp.top = parseInt(tmp.top)-scrollspeed;
    if (parseInt(tmp.top) <= -scrollerheight){
     tmp.top = scrollerheight*(total_area-1);
    }
    if (s_tmp++ > (s_amount-1)*scroll_content.length){
     wait_flag=false;
     window.setTimeout("wait_flag=true;s_tmp=0;",waitingtime);
    }
   }
  }
  window.setTimeout("scrolling()",1);
}

function insert_area(idx, n){ // area 삽입
  html='<div style="left: 0px; width: 100%; position: absolute; top: '+(scrollerheight*n)+'px" id="scroll_area'+n+'">';
  html+=scroll_content[idx];
  html+='</div>';
  document.write(html);
}

function getStringByByte(str, limit_length) {
    if (str == "") {
        return  str;
    }
    var len = 0;
    for (var i = 0; i < str.length; i++) {
        if (str.charCodeAt(i) > 128) {
            len++;
        }
        len++;
        if(len >= limit_length) {
        	return (str.substring(0,(i+1))+"..");
        } 
    }
    return  str;
}
</SCRIPT>

<SCRIPT language=javascript>
ː
var scroll_content = new Array();
scroll_content[0]="<img src='"+my_array[0].img+"' align=absmiddle><a href='"+my_array[0].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[0].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[1].img+"' align=absmiddle><a href='"+my_array[1].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[1].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[2].img+"' align=absmiddle><a href='"+my_array[2].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[2].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[3].img+"' align=absmiddle><a href='"+my_array[3].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[3].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[4].img+"' align=absmiddle><a href='"+my_array[4].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[4].txt,12)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[5].img+"' align=absmiddle><a href='"+my_array[5].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[5].txt,15)+"</a><br>";
scroll_content[1]="<img src='"+my_array[6].img+"' align=absmiddle><a href='"+my_array[6].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[6].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[7].img+"' align=absmiddle><a href='"+my_array[7].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[7].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[8].img+"' align=absmiddle><a href='"+my_array[8].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[8].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[9].img+"' align=absmiddle><a href='"+my_array[9].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[9].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[10].img+"' align=absmiddle><a href='"+my_array[10].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[10].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[11].img+"' align=absmiddle><a href='"+my_array[11].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[11].txt,15)+"</a><br>";

</SCRIPT>
<DIV id=scroll_image onmouseover=bMouseOver=0 style="OVERFLOW: hidden; WIDTH: 147px; HEIGHT: 119px" onmouseout=bMouseOver=1>
<div style="position:relative; WIDTH: 147px; HEIGHT: 119px"> <SCRIPT>startscroll();</SCRIPT></div>
</DIV> 
</td></tr>
				 <tr><td><img src="http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/dot_line01.gif" border="0"></td></tr>

			</table>
			</div>

</body>
</html>