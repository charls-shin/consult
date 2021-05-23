<?php
if( headers_sent() == FALSE )
{
	header( 'Content-Type: text/html; charset=euc-kr' );
}

$dir = PATH_SUB_ROOT."_functions/";

// Open a known directory, and proceed to read its contents
if (is_dir($dir))
{
	if ($dh = opendir($dir))
	{
		while (($file = readdir($dh)) !== false) {
			if( filetype($dir . $file) == 'file' && preg_match('/^(function\..+\.php)$/',$file) ) {
				include_once($dir . $file);
			}
		}
		closedir($dh);
	}
}

require $_SERVER['DOCUMENT_ROOT']."vendor/autoload.php";