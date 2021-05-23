<?
//error_reporting(1);

class Oracle_OCI
{
	var $conn = FALSE;
	var $stmt = 0;
	var $stmt2 = 0;
	var $abstractData= FALSE;
	var $error= FALSE;
	var $query;
	var $error_check=1;   // $error_check = 0 이면 에러무시!! sql이 에러가 나도 rollback 되지 않으면 스크립트가 계속 진행된다. 1이면 rollback되고 script도 die된다.
	var $error_print=0;     // $error_print = 1 이면 에러메세지 출력!! ..0이면 준비된 에러화면을 출력한다..
	var $email=0;    // 0이면 안보내고 1이면 보낸다.
	var $adminmail="ssl93@uwayapply.com; sinjc01@uwayapply.com; seojh@uwayapply.com; bkdelete@uwayapply.com"; // Defalut..관리자 메일

	var $resultSetBufferSize=50; // 2009/05/20 13:54 김태승

	//		쿼리 로그와 관련된 부분
	var $log_slow_query		= TRUE;		// slow query 저장 여부
	var $query_start_time		= NULL;		// 시작시간 저장
	var $long_query_time		= 2;			// slow query 기준 시간(초)
	var $bind_vars					= NULL;		// bind된 값 저장

	var $id					= NULL;		// id
	var $pw					= NULL;		// pw
	var $sid					= NULL;		// sid
	var $fn					= NULL;		// sid


	function connect_view($id,$passwd,$oracle_sid,$fn)
	{
		$this->id=$id;
		$this->pw=$passwd;
		$this->sid=$oracle_sid;
		$this->fn=$fn;
	}

	function sendmail($email_content)
	{
		//global $SERVER_NAME;
		if($this->email)
		{
			// report email 전송
			$date=date("s")%3;
			if($date==1){$this->adminmail="ssl93@uwayapply.com";}
			else if($date==2){$this->adminmail="ssl93@uwayapply.com";}
			else {$this->adminmail="ssl93@uwayapply.com";}

			mail("$this->adminmail",$_SERVER['SERVER_NAME']." 서버의 $comment 에러 보고!.","$email_content","Content-type: text/html\r\nFrom: ORALCE REPORT");
		}
	}

	function showerror($comment,$code="1")
	{
		//global $PHP_SELF,$SERVER_NAME,$REMOTE_ADDR,$HTTP_SERVER_VARS;
		global $HTTP_SERVER_VARS;

		if( $this->ckIntraIp() ){
			$this->error_print=1;
			$this->email=1;
		}

		if($this->error_check)
		{
			OCIRollback($this->conn);  // error 발생시 rollback!!!
			$email_content="
<script language='JavaScript1.2'>
<!--
function flashit()
{
	if (!document.all)
	return
	if (myexample.style.borderColor=='#273f63')
		myexample.style.borderColor='#ffffff'
	else
		myexample.style.borderColor='#273f63'
}
setInterval('flashit()', 500)
//-->
</script>

<div id='Layer1' style='position:absolute; left:20px; top:10px;  z-index:1'>
<STYLE type=text/css>td {  font-size: 9pt; line-height:13pt;}</STYLE>
<table width='600' border='0' cellspacing='0' cellpadding='0' align=center >
<tr>    <td height='4' rowspan='2' bgcolor='#007ccd'></td>
<td height='4' rowspan='2' bgcolor='#007ccd' align='center'><b><font color='#ffffff'>".$_SERVER['SERVER_NAME']." 의 $comment 에러 보고 !!</font></b></td>
<td width='4' height='4'></td>
</tr>
<tr>
<td width='4' bgcolor='#b0dcff' height='16'></td>
</tr>
<tr>
<td width='4' height='4'></td>
<td colspan='2' height='4' bgcolor='#b0dcff' ></td>
</tr>
</table>
<table border=1 cellspacing='0' cellpadding='3' align='center' border='1' bgcolor='#ffffff' bordercolordark='#ffffff' bordercolorlight='#273f63' >
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb' ><B>오류시간 </td><td width=600>".date("Y-m-d h:i:s")." </td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>오류번호</td><td width=600> ORA-". $this->error['code'] ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>오류내용 </td><td width=600>$comment</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>오류메세지 </td><td width=600  id='myexample' style='border:5px solid #fffffff'><font color=red>". nl2br($this->error['message']) ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>id </td><td width=600>". $this->id ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>sid </td><td width=600>". $this->sid ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>Query </td><td width=600>". nl2br($this->bindQry($this->query)) ."</td></tr>\n";

// 			while(list($key,$value) = each($HTTP_SERVER_VARS))
// 			{  // 서버 변수들..
// 				if($key!='PATH')
// 				{
// 					$email_content.="<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'>
// 					<td bgcolor='#ecf4fb'><B>$key  </td>
// 					<td width=600>". nl2br($value) . "</td>
// 					</tr>\n";
// 				}
// 			}
			$email_content.="</TABLE></div>";
			$this->sendmail($email_content); // mail 전송
			if($this->error_print==1)
			{ // 화면에 report 를 찍어준다.
				echo $email_content;
			} else if($this->error_print==2) {
				$this->Error_Chk($this->error[code]);
			} else
			{ // 준비된 화면을 뿌려준다.
//				echo "<script>window.location='http://$SERVER_NAME/error/servicefail.html?code=$code';</script>";                       // 실패화면~~
					echo "
						<div style='position:absolute;top:10px;z-index:1;width:100%;background-color:#FFFFFF'><br><br>
						<table cellpadding='2' cellspacing='1' border='0' align='center' bgcolor='#CCCCCC' style='font-family:tahoma;font-size:8pt' width='500'>
							<tr height='25' bgcolor='#31639C'>
								<td align='center' style='color:#FFFFFF'><b>DB ERROR : ".$_SERVER['SERVER_NAME']."</b></td>
							</tr>
							<tr bgcolor='#FFFFFF'>
								<td height='180' style='padding-left:20px'>
									현재 사용자가 많아 요청을 처리하지 못하였습니다.<br>
									<br>
									잠시 후, 다시 시도하여 주십시오<br>
									<br>
									동일한 문제가 계속 발생할 경우, 담당자에게 문의하시길 바랍니다. 죄송합니다.<br>
									<br>
								</td>
							</tr>
							<tr height='25' bgcolor='#31639C'>
								<td align='center' style='color:#FFFFFF'><b>(주)유웨이어플라이</b></td>
							</tr>
						</table>
						</div>";
			}

			OCILogoff($this->conn);
			die();  // script 종료
		}
	}


	function login($id,$passwd,$oracle_sid,$fn='login')
	{
		
		// DB 연결 변경 2009/12/18 02:00 이상수
		include 'tadb_ch.php';

		$this->connect_view($id,$passwd,$oracle_sid,$fn);
		
		$this->conn = OCILogon($id,$passwd,$oracle_sid) or $this->showerror(" $oracle_sid - ".$_SESSION['sa_term']." - ".$_SESSION['sa_year']." db connect error",2);
	}

	function connect($id="ipsi",$passwd="",$oracle_sid="ora7")
	{
		$this->login($id,$passwd,$oracle_sid,'connect');
	}
	
	//uway.com 에서 사용하는 게시판 지금은 빠진 상태
	function connect_uwaywww($id="uwaywww",$passwd="",$oracle_sid="ibm06")
	{
		if(preg_match('/(re|dev)/',$_SERVER['SERVER_NAME'])){
			$oracle_sid='dev3';
		}

		$this->login($id,$passwd,$oracle_sid,'connect_uwaywww');
	}
	
	//사용안하는 것 같음
	function connect_board($id="board",$passwd="",$oracle_sid="ibm06")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_board');
	}
	// 데이타 전송
	function connect_dbsend($id="ipsi",$passwd="",$oracle_sid="ipsietc")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_dbsend');
	}

	// ta.uwayapply.com
	function connect_ta($id="ipsi",$passwd="",$oracle_sid="ora7")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_ta');
	}

	// 군중복 ( 정시 && APPLYGR!=0
	function connect_applygrchk($id="ipsi",$passwd="",$oracle_sid="us401")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_applygrchk');
	}
	// 관리자(나의원서관리,대학관리자,유웨이관리자) 에서 접속하기 위한 함수...
	function connect_myapply($id="ipsi",$passwd="",$oracle_sid="ora7")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_myapply');
	}
	function connect_admin($id="ipsi",$passwd="",$oracle_sid="ibm690")
	{
		if(preg_match('/(re)/',$_SERVER["SERVER_NAME"])){
			$id="dipsi";
			$oracle_sid="ora7";
		}else if(preg_match('/(dev)/',$_SERVER["SERVER_NAME"])){
			$oracle_sid="ora7";
		}
		
		// DB 연결 변경 2009/12/18 02:00 이상수
		$this->login($id,$passwd,$oracle_sid,'connect_admin');
	}
	/*function connect_ora7($id="ipsi",$passwd="",$oracle_sid="ora7")
	{
		$this->login($id,$passwd,$oracle_sid.'connect_ora7');
	}*/
	function connect_login($id="ipsi",$passwd="",$oracle_sid="ibm690")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_login');
	}
	function connect_search($id="ipsi",$passwd="",$oracle_sid="ora7")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_search');
	}

	function serverVersion()
	{
		return OCIServerVersion($this->conn);
	}

	function parse($qry,$stmt="stmt")
	{
		$this->query=$qry;
		$this->$stmt=OCIParse($this->conn,$qry);
	}

	function parseExec($qry,$stmt="stmt")
	{
		$this->query=$qry;
		$this->$stmt=OCIParse($this->conn,$qry);
		//$this->exec("",$stmt);
		$this->exec(OCI_DEFAULT,$stmt);
	}

	function SeJongExec($qry,$stmt="stmt")
	{
		echo nl2br($qry)."<br>";
		$this->query=$qry;
		$this->$stmt=OCIParse($this->conn,$qry);
		$this->exec("",$stmt);
	}

	function exec($mode=OCI_DEFAULT, $stmt="stmt")
	{
		$this->slow_query_start();
		@OCIExecute( $this->{$stmt}, $mode );
		$this->slow_query_end();

		$this->error = @OCIError( $this->{$stmt} );
		if( empty( $this->error ) == FALSE ) return $this->showerror( "Exec() Error" );

		@OCISetPrefetch( $this->{$stmt}, $this->resultSetBufferSize );
	}


	function total($board,$condition,$stmt="stmt")
	{
		$str="select count(*) from $board where $condition";
		$this->parseExec($str,$stmt);
		$this->fetch($stmt);
		$total=$this->result(1,$stmt);
		return $total;
	}

	function newDescriptor($type)
	{
		$this->abstractData = OCINewDescriptor($this->conn,$type);
	}

	function freeDescriptor()
	{
		OCIFreeDescriptor($this->abstractData);
	}

	function defineByName($upper,&$var)
	{
		OCIDefineByName($this->stmt,$upper,$var);
	}


	function bindByName($place_holder,&$var,$length, $stmt="stmt")
	{
		if( strstr( $this->query, $place_holder ) )
		{
			if( @OCIBindByName( $this->{$stmt}, $place_holder, $var, $length ) == FALSE )
			{
				return $this->showerror( "bindByName() Error : {$place_holder} - {$var}" );
			}
			else
			{
				$this->add_bind_vars( $place_holder, $var );
			}
		}
	}

	function bindByNameAbstract($place_holder,$type)
	{
		OCIBindByName($this->stmt,$place_holder,$this->abstractData,-1,$type);
		return $this->abstractData;
		// return &$this->abstractData; PARSE ERROR!
	}

	function fetch($stmt="stmt")
	{
		return OCIFetch($this->$stmt);
	}

	function result($i,$stmt="stmt")
	{      // 인덱스는 0이 아닌 1부터 시작
		return OCIresult($this->$stmt,$i);
	}


	function fetchInto( &$col, $mode = NULL, $stmt = 'stmt' )
	{
		if( empty( $mode ) ) $mode = OCI_RETURN_NULLS + OCI_ASSOC + OCI_RETURN_LOBS;
		return @OCIFetchInto( $this->{$stmt}, $col, $mode );
	}


	function fetchStatement( &$arr, $stmt = 'stmt' )
	{
		return OCIFetchStatement( $this->{$stmt}, $arr, 0, -1, OCI_FETCHSTATEMENT_BY_COLUMN +  OCI_RETURN_NULLS + OCI_ASSOC );
	}

	function Error_Chk($mod)
	{
		echo "<script language=javascript>";
		switch($mod) {
			case '1400':
				echo "
				alert('필수값이 Null 입니다. \\n확인 후 다시등록하셔요~~\\n확인 후 안되시면(개발팀에 문의)');
				history.back();
				";
			break;
			case '1401':
				echo "
				alert('필드값이 너무 큽니다. \\n확인 후 다시등록하셔요~~\\n확인 후 안되시면(개발팀에 문의)');
				history.back();
				";
			break;
			case '24337':
				echo "
				alert('오라클 SQL Error 입니다. \\n확인 후 안되시면(개발팀에 문의)');
				history.back();
				";
			break;
			default:
				echo "
				alert('관리자가 체크 못한 에러입니다. \\n   다시 한번 해 보시구요~~ \\n확인 후 안되시면(개발팀에 문의)');
				history.back();
				";
			break;

		}
		echo "</script>";
	}

	function slow_query_start()
	{
		if( $this->log_slow_query !== TRUE || function_exists( 'log_message' ) == FALSE ) return;

		$this->query_start_time = array_sum( explode( ' ', microtime() ) );
	}

	function slow_query_end()
	{
		if( $this->log_slow_query !== TRUE || function_exists( 'log_message' ) == FALSE ) return;

		$query_end_time = array_sum( explode( ' ', microtime() ) );

		$query_time = round( $query_end_time - $this->query_start_time, 4 );

		if( $query_time > $this->long_query_time )
		{
			log_message( 'WARNING', array(
				'title' =>'slow query',
				'msg' => $query_time,
				'idx1' => md5( $this->query ),
				'class_mode' => $this->classMode,
				'conn_info' => $this->connInfo,
				'sql' => $this->query,
				'bind_vars' => $this->bind_vars
			) );
		}
	}

	function add_bind_vars( $column, $value )
	{
		if( $this->log_slow_query !== TRUE ) return;

		$this->bind_vars[$column] = $value;
	}
	
	function getAddWhere($column,$value,$alias)
	{
	   $this->add_bind_vars($column,$value);
	    
	   return <<<EOF
	          AND {$alias}.{$column} = :{$column}
EOF;
	}

//////////////////////////////////////////////////////////
	function numCols($stmt="stmt") {
	   return OCINumCols($this->$stmt);
	}

	// $index 는 1 부터 시작
	function columnName($index,$stmt="stmt") {
	   return OCIColumnName($this->$stmt,$index);
	}

	function columnisNULL($index,$stmt="stmt") {
	   return OCIColumnIsNULL($this->$stmt,$index);
	}

	function columnType($index,$stmt="stmt") {
	   return OCIColumnType($this->$stmt,$index);
	}

	function columnSize($index,$stmt="stmt") {
	   return OCIColumnSize($this->$stmt,$index);
	}

	function StatementType() {
	   return OCIStatementType($this->stmt);
	}

	function rowCount($stmt="stmt") {
	   return OCIRowCount($this->$stmt);
	}
	///////////////////////////////////////////////

	function parseFree($stmt="stmt") {
	   OCIFreeStatement($this->$stmt);
	}


	 function rollback(){
		  OCIRollback($this->conn);

		  }
	  function commit(){
		  OCICommit($this->conn);
		  }


	function disconnect() {

	   if($this->error && $this->error_check) {

		  $this->showerror();
	   }
	   else {

		  OCICommit($this->conn);
	   }
	   OCILogoff($this->conn);
	}

	function viewQry($qry,$div='')
	{
		 if( $this->ckIntraIp() && !$GLOBALS['excel_mode'] )
		 {
		 	$qry=$this->bindQry($qry);

		 	echo "<a href='#' onclick='document.all.view_chk".$div.".style.display=\"block\"'>qry [보기</a>|<a href='#' onclick='document.all.view_chk".$div.".style.display=\"none\"'> 접기]</a>";
		 	echo "	<div id=view_chk".$div." style='display:none;background: #BFBFFF;font-family: verdana;width: 100%;overflow:auto;text-align:left'>";
		 	echo 'ID : '.$this->id.', SID : '.$this->sid.'<br>';
		 	echo "		<xmp>", print_r($qry), "</xmp>";
		 	echo "	</div>";
		 }
	}
	
	function bind_preg_callback($matches)
	{
		foreach($this->bind_vars as $key => $value){
			$bind_vars[str_replace(':','',$key)]=$value;
		}
	
		return '\''.($bind_vars{$matches[1]}).'\'';
	}
	
	function table_preg_callback($matches)
	{
		return  $this->id.'.tbs_'.$matches[1].'@'.$this->sid;
	}

	function bindQry($qry)
	{
		if( $qry && $this->bind_vars )
		{
			$qry = preg_replace_callback("/:([a-zA-Z_][\w]+)\b/", array($this, 'bind_preg_callback'), $qry);
		}
		
		$qry = preg_replace_callback("/tbs_([a-zA-Z_][\w]+)\b/",array($this, 'table_preg_callback'), $qry);
		
		return $qry;
	}
	
	

	// 사내 아이피 체크
	function ckIntraIp()
	{
		return $GLOBALS['inCompanyIn'];
	}
} // end class

//////// Object 생성 ////////////////////////////////


//////////////////////////////////////////////////////////////
?>
