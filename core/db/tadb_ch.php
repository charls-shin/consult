<?

	//2011-05-03 오후 4:47 내부관리자 edit_mail_view 땜시...
	$ss_r_dbserver=($_REQUEST['ss_dbserver']!='' ? $_REQUEST['ss_dbserver'] : $_SESSION['sa_dbserver']);
	$ss_r_year=($_REQUEST['year']!='' ? $_REQUEST['year'] : $_SESSION['sa_year']);
	$ss_r_term=($_REQUEST['term']!='' ? $_REQUEST['term'] : $_SESSION['sa_term']);


	if( preg_match ('/(lcl)/',$_SERVER["SERVER_NAME"]) && $oracle_sid=="ora7" && $id=='ipsi' )
	{
		$id="ipsi"; $passwd=""; $oracle_sid="ora7";
	}
	else if( preg_match ('/(dev)/',$_SERVER["SERVER_NAME"]) && $oracle_sid=="ora7" && $id=='ipsi' ) 
	{
		$id="ipsi"; $passwd=""; $oracle_sid="ora7";
	}
	else if( preg_match ('/(re)/',$_SERVER["SERVER_NAME"]) && $oracle_sid=="ora7" ) 
	{
		$id="dipsi"; $passwd=""; $oracle_sid="ora7";
	}
	
	include_once PATH_COMMON.'class.Cipher.inc';
	$cipher = new Cipher();
	
// 				$pwd='CIPSIUAC12$';
// 				echo $pwd.' = '.$cipher->encrypt($pwd).'<br/>';
// 				$res = $cipher->decrypt('E27A2CD15FD2875D4F09968F377E0375');
// 				echo $res.'<hr/>';
// 	die();
	switch($id)
	{
		case 'dipsi': $passwd = 'E27A2CD15FD2875D4F09968F377E0375'; break;
		case 'reipsi': $passwd = 'D41A95E8A565FC5C5AEFC8B56F31BDAE'; break;
		case 'ipsi': $passwd = '24834F36B9A78ECA2DBBE4EF179992C2'; break;
		case 'ipsi2c': $passwd = '12EA5E9D8EA7C56B927F184CE1FF5810'; break;
		case 'upublic': $passwd = 'EAF26AE96A64501499FB1C2C6CB121C1'; break;
		case 'board': $passwd = '9E864DF3B84B3F34B2E7F9AEE1CFFE6C'; break;
		case 'upa': $passwd = 'E1B1A75D34D020DA1FC84FFD80CDAC17'; break;
		case 'reupa': $passwd = '2B0F330071D79EB5D1E5DAF90762EE75'; break;
		case 'lclupa': $passwd = '31F2A5514CA6FC02A438B94E064EC75E'; break;
		case 'cash': $passwd = 'DC50937D7FA398E3343F4921D6F4DFB5'; break;
		case 'uwaymain': $passwd = '32C78CF4EF7116D6B4E4E5D10553AE48'; break;
		case 'cmg': $passwd = '974EB22D4CC57E2C833ACE3C33FECB55'; break;
		case 'share1': $passwd = 'AE6EC2F72422F1819D148680253CDB28'; break;
		case 'dcipsi': $passwd = '06B278AA874D55E5B08EA8838B22B69C'; break;
		case 'cipsi': $passwd = 'E7763874C1459F7E6736134485A6D538'; break;
		default: $passwd = ''; $this->showerror('ID({$id}) is not exists'); break;
	}
		
	if( $passwd!= '' )
	{
		$passwd = $cipher->decrypt($passwd);
	}

	unset($cipher);
?>