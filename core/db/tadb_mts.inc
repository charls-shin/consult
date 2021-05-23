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
	var $error_check=1;   // $error_check = 0 �̸� ��������!! sql�� ������ ���� rollback ���� ������ ��ũ��Ʈ�� ��� ����ȴ�. 1�̸� rollback�ǰ� script�� die�ȴ�.
	var $error_print=0;     // $error_print = 1 �̸� �����޼��� ���!! ..0�̸� �غ�� ����ȭ���� ����Ѵ�..
	var $email=0;    // 0�̸� �Ⱥ����� 1�̸� ������.
	var $adminmail="ssl93@uwayapply.com; sinjc01@uwayapply.com; seojh@uwayapply.com; bkdelete@uwayapply.com"; // Defalut..������ ����

	var $resultSetBufferSize=50; // 2009/05/20 13:54 ���½�

	//		���� �α׿� ���õ� �κ�
	var $log_slow_query		= TRUE;		// slow query ���� ����
	var $query_start_time		= NULL;		// ���۽ð� ����
	var $long_query_time		= 2;			// slow query ���� �ð�(��)
	var $bind_vars					= NULL;		// bind�� �� ����

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
			// report email ����
			$date=date("s")%3;
			if($date==1){$this->adminmail="ssl93@uwayapply.com";}
			else if($date==2){$this->adminmail="ssl93@uwayapply.com";}
			else {$this->adminmail="ssl93@uwayapply.com";}

			mail("$this->adminmail",$_SERVER['SERVER_NAME']." ������ $comment ���� ����!.","$email_content","Content-type: text/html\r\nFrom: ORALCE REPORT");
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
			OCIRollback($this->conn);  // error �߻��� rollback!!!
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
<td height='4' rowspan='2' bgcolor='#007ccd' align='center'><b><font color='#ffffff'>".$_SERVER['SERVER_NAME']." �� $comment ���� ���� !!</font></b></td>
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
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb' ><B>�����ð� </td><td width=600>".date("Y-m-d h:i:s")." </td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>������ȣ</td><td width=600> ORA-". $this->error['code'] ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>�������� </td><td width=600>$comment</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>�����޼��� </td><td width=600  id='myexample' style='border:5px solid #fffffff'><font color=red>". nl2br($this->error['message']) ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>id </td><td width=600>". $this->id ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>sid </td><td width=600>". $this->sid ."</td></tr>
<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'><td bgcolor='#ecf4fb'><B>Query </td><td width=600>". nl2br($this->bindQry($this->query)) ."</td></tr>\n";

// 			while(list($key,$value) = each($HTTP_SERVER_VARS))
// 			{  // ���� ������..
// 				if($key!='PATH')
// 				{
// 					$email_content.="<tr onmouseover=this.style.background='#F7EEE6' onmouseout=this.style.background='white'>
// 					<td bgcolor='#ecf4fb'><B>$key  </td>
// 					<td width=600>". nl2br($value) . "</td>
// 					</tr>\n";
// 				}
// 			}
			$email_content.="</TABLE></div>";
			$this->sendmail($email_content); // mail ����
			if($this->error_print==1)
			{ // ȭ�鿡 report �� ����ش�.
				echo $email_content;
			} else if($this->error_print==2) {
				$this->Error_Chk($this->error[code]);
			} else
			{ // �غ�� ȭ���� �ѷ��ش�.
//				echo "<script>window.location='http://$SERVER_NAME/error/servicefail.html?code=$code';</script>";                       // ����ȭ��~~
					echo "
						<div style='position:absolute;top:10px;z-index:1;width:100%;background-color:#FFFFFF'><br><br>
						<table cellpadding='2' cellspacing='1' border='0' align='center' bgcolor='#CCCCCC' style='font-family:tahoma;font-size:8pt' width='500'>
							<tr height='25' bgcolor='#31639C'>
								<td align='center' style='color:#FFFFFF'><b>DB ERROR : ".$_SERVER['SERVER_NAME']."</b></td>
							</tr>
							<tr bgcolor='#FFFFFF'>
								<td height='180' style='padding-left:20px'>
									���� ����ڰ� ���� ��û�� ó������ ���Ͽ����ϴ�.<br>
									<br>
									��� ��, �ٽ� �õ��Ͽ� �ֽʽÿ�<br>
									<br>
									������ ������ ��� �߻��� ���, ����ڿ��� �����Ͻñ� �ٶ��ϴ�. �˼��մϴ�.<br>
									<br>
								</td>
							</tr>
							<tr height='25' bgcolor='#31639C'>
								<td align='center' style='color:#FFFFFF'><b>(��)�����̾��ö���</b></td>
							</tr>
						</table>
						</div>";
			}

			OCILogoff($this->conn);
			die();  // script ����
		}
	}


	function login($id,$passwd,$oracle_sid,$fn='login')
	{
		
		// DB ���� ���� 2009/12/18 02:00 �̻��
		include 'tadb_ch.php';

		$this->connect_view($id,$passwd,$oracle_sid,$fn);
		
		$this->conn = OCILogon($id,$passwd,$oracle_sid) or $this->showerror(" $oracle_sid - ".$_SESSION['sa_term']." - ".$_SESSION['sa_year']." db connect error",2);
	}

	function connect($id="ipsi",$passwd="",$oracle_sid="ora7")
	{
		$this->login($id,$passwd,$oracle_sid,'connect');
	}
	
	//uway.com ���� ����ϴ� �Խ��� ������ ���� ����
	function connect_uwaywww($id="uwaywww",$passwd="",$oracle_sid="ibm06")
	{
		if(preg_match('/(re|dev)/',$_SERVER['SERVER_NAME'])){
			$oracle_sid='dev3';
		}

		$this->login($id,$passwd,$oracle_sid,'connect_uwaywww');
	}
	
	//�����ϴ� �� ����
	function connect_board($id="board",$passwd="",$oracle_sid="ibm06")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_board');
	}
	// ����Ÿ ����
	function connect_dbsend($id="ipsi",$passwd="",$oracle_sid="ipsietc")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_dbsend');
	}

	// ta.uwayapply.com
	function connect_ta($id="ipsi",$passwd="",$oracle_sid="ora7")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_ta');
	}

	// ���ߺ� ( ���� && APPLYGR!=0
	function connect_applygrchk($id="ipsi",$passwd="",$oracle_sid="us401")
	{
		$this->login($id,$passwd,$oracle_sid,'connect_applygrchk');
	}
	// ������(���ǿ�������,���а�����,�����̰�����) ���� �����ϱ� ���� �Լ�...
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
		
		// DB ���� ���� 2009/12/18 02:00 �̻��
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
	{      // �ε����� 0�� �ƴ� 1���� ����
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
				alert('�ʼ����� Null �Դϴ�. \\nȮ�� �� �ٽõ���ϼſ�~~\\nȮ�� �� �ȵǽø�(�������� ����)');
				history.back();
				";
			break;
			case '1401':
				echo "
				alert('�ʵ尪�� �ʹ� Ů�ϴ�. \\nȮ�� �� �ٽõ���ϼſ�~~\\nȮ�� �� �ȵǽø�(�������� ����)');
				history.back();
				";
			break;
			case '24337':
				echo "
				alert('����Ŭ SQL Error �Դϴ�. \\nȮ�� �� �ȵǽø�(�������� ����)');
				history.back();
				";
			break;
			default:
				echo "
				alert('�����ڰ� üũ ���� �����Դϴ�. \\n   �ٽ� �ѹ� �� ���ñ���~~ \\nȮ�� �� �ȵǽø�(�������� ����)');
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

	// $index �� 1 ���� ����
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

		 	echo "<a href='#' onclick='document.all.view_chk".$div.".style.display=\"block\"'>qry [����</a>|<a href='#' onclick='document.all.view_chk".$div.".style.display=\"none\"'> ����]</a>";
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
	
	

	// �系 ������ üũ
	function ckIntraIp()
	{
		return $GLOBALS['inCompanyIn'];
	}
} // end class

//////// Object ���� ////////////////////////////////


//////////////////////////////////////////////////////////////
?>