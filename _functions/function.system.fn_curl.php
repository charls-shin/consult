<?php
/**
 * Send a POST requst using cURL
 * @param string $url to request
 * @param array $post values to send
 * @param array $options for cURL
 * @return string
 */
function curl_post($url, array $post = array(), array $options = array())
{
	$defaults = array(
		CURLOPT_POST => 1,
		CURLOPT_HEADER => 0,
		CURLOPT_URL => $url,
		CURLOPT_FRESH_CONNECT => 1,
		CURLOPT_RETURNTRANSFER => 1,
		//CURLOPT_FOLLOWLOCATION => TRUE,
		CURLOPT_SSL_VERIFYPEER => 0,
		CURLOPT_FORBID_REUSE => 1,
		CURLOPT_TIMEOUT => 300,
		CURLOPT_POSTFIELDS => http_build_query($post)
	);
	
	$ch = curl_init();
	curl_setopt_array($ch, ($options + $defaults));
	if( ! $result = curl_exec($ch))
	{
		//trigger_error(curl_error($ch));
	}
	curl_close($ch);
	return $result;
}

function curl_post_file($url, array $post = NULL, array $options = array())
{
	$defaults = array(
		CURLOPT_POST => 1,
		CURLOPT_HEADER => 0,
		CURLOPT_URL => $url,
		CURLOPT_FRESH_CONNECT => 1,
		CURLOPT_RETURNTRANSFER => 1,
		//CURLOPT_FOLLOWLOCATION => TRUE,
		CURLOPT_SSL_VERIFYPEER => 0,
		CURLOPT_FORBID_REUSE => 1,
		CURLOPT_CONNECTTIMEOUT => 300,
		CURLOPT_TIMEOUT => 300,
		CURLOPT_POSTFIELDS => $post
	);
	
	$ch = curl_init();
	curl_setopt_array($ch, ($options + $defaults));
	if( ! $result = curl_exec($ch))
	{
		//trigger_error(curl_error($ch));
	}
	curl_close($ch);
	return $result;
}

function curl_post_token($url, array $post = array(),string $token ='',array $options = array())
{
	if( preg_match('/(read.php)/',$url) ) $customrequest="GET";
	else if( preg_match('/(create.php)$/',$url) ) $customrequest="POST";
	else if( preg_match('/(delete.php)$/',$url) ) $customrequest="DELETE";
	else if( preg_match('/(update.php)$/',$url) ) $customrequest="PUT";
	else $customrequest='POST';
	
	$headr = [
		'Content-type: application/json',
		'Authorization: Bearer '.$token,
	];
	
	$defaults = array(
		CURLOPT_URL => $url,
		CURLOPT_FRESH_CONNECT => 1,
		CURLOPT_RETURNTRANSFER => 1,
		CURLOPT_SSL_VERIFYPEER => 0,
		CURLOPT_TIMEOUT => 300,
		//CURLOPT_POST => true,
		CURLOPT_CUSTOMREQUEST => $customrequest,
		CURLOPT_HTTPHEADER => $headr,
		CURLOPT_POSTFIELDS => json_encode($post)
	);
	
	$ch = curl_init();
	curl_setopt_array($ch, ($options + $defaults));
	if( ! $result = curl_exec($ch))
	{
		//trigger_error(curl_error($ch));
	}
	curl_close($ch);
	return $result;
}


/**
 * Send a GET requst using cURL
 * @param string $url to request
 * @param array $get values to send
 * @param array $options for cURL
 * @return string
 */
function curl_get($url,array $get = array(), array $options = array())
{
	$defaults = array(
		CURLOPT_URL => $url. (strpos($url, '?') === FALSE ? '?' : ''). http_build_query($get),
		CURLOPT_HEADER => 0,
		CURLOPT_RETURNTRANSFER => TRUE,
		CURLOPT_SSL_VERIFYPEER => 0,
		CURLOPT_CONNECTTIMEOUT => 300,
		CURLOPT_TIMEOUT => 300
	);
	
	$ch = curl_init();
	curl_setopt_array($ch, ($options + $defaults));
	if( ! $result = curl_exec($ch))
	{
		trigger_error(curl_error($ch));
	}
	curl_close($ch);
	return $result;
}

function curl_exec_follow(/*resource*/ $ch, /*int*/ &$maxredirect = null)
{
	$mr = $maxredirect === null ? 5 : intval($maxredirect);
	if (ini_get('open_basedir') == '' && ini_get('safe_mode' == 'Off')) {
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, $mr > 0);
		curl_setopt($ch, CURLOPT_MAXREDIRS, $mr);
	} else {
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, false);
		if ($mr > 0) {
			$newurl = curl_getinfo($ch, CURLINFO_EFFECTIVE_URL);
			
			$rch = curl_copy_handle($ch);
			curl_setopt($rch, CURLOPT_HEADER, true);
			curl_setopt($rch, CURLOPT_NOBODY, true);
			curl_setopt($rch, CURLOPT_FORBID_REUSE, false);
			curl_setopt($rch, CURLOPT_RETURNTRANSFER, true);
			do {
				curl_setopt($rch, CURLOPT_URL, $newurl);
				$header = curl_exec($rch);
				if (curl_errno($rch)) {
					$code = 0;
				} else {
					$code = curl_getinfo($rch, CURLINFO_HTTP_CODE);
					if ($code == 301 || $code == 302) {
						preg_match('/Location:(.*?)\n/', $header, $matches);
						$newurl = trim(array_pop($matches));
					} else {
						$code = 0;
					}
				}
			} while ($code && --$mr);
			curl_close($rch);
			if (!$mr) {
				if ($maxredirect === null) {
					trigger_error('Too many redirects. When following redirects, libcurl hit the maximum amount.', E_USER_WARNING);
				} else {
					$maxredirect = 0;
				}
				return false;
			}
			curl_setopt($ch, CURLOPT_URL, $newurl);
		}
	}
	return curl_exec($ch);
}

// $res[$i]=array(
// 		CURLOPT_URL => "https://".$_SERVER["SERVER_NAME"]."/apply/db_layout/lib/multi_exec.php",
// 		CURLOPT_POSTFIELDS => http_build_query($post)
// );
function multiCurl($res=array())
{
	if(count($res)<=0) return False;
	
	$handles = array();
	
	$defaults = array(
		CURLOPT_POST => 1,
		CURLOPT_HEADER => 0,
		CURLOPT_FRESH_CONNECT => 1,
		CURLOPT_RETURNTRANSFER => 1,
		CURLOPT_SSL_VERIFYPEER => 0,
		//CURLOPT_FOLLOWLOCATION => TRUE,
		CURLOPT_FORBID_REUSE => 1,
		CURLOPT_TIMEOUT => 4000
	);
	
	// add curl options to each handle
	foreach($res as $k=>$options){
		$ch{$k} = curl_init();
		$opt = curl_setopt_array($ch{$k}, $defaults+$options);
		//var_dump($opt);
		$handles[$k] = $ch{$k};
	}
	
	$mh = curl_multi_init();
	
	// add handles
	foreach($handles as $k => $handle){
		$err = curl_multi_add_handle($mh, $handle);
	}
	
	$running_handles = null;
	
	do {
		curl_multi_exec($mh, $running_handles);
		curl_multi_select($mh);
	} while ($running_handles > 0);
	
	foreach($res as $k=>$row)
	{
		$res[$k]['error'] = curl_error($handles[$k]);
		if(!empty($res[$k]['error']))
			$res[$k]['data']  = '';
		else{
			$res[$k]['data']  = curl_multi_getcontent( $handles[$k] );  // get results
		}
		$res[$k]['info']=curl_getinfo($handles[$k]);
		// close current handler
		curl_multi_remove_handle($mh, $handles[$k] );
	}
	
	curl_multi_close($mh);
	return $res; // return response
}


if (!function_exists('curl_file_create')) {
	function curl_file_create($filename, $mimetype = '', $postname = '') {
		return "@$filename;filename="
			. ($postname ? basename($filename) : '')
			. ($mimetype ? ";type=$mimetype" : '');
	}
}