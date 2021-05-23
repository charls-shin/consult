<?php


namespace consult\core;


class Common
{
	public $http='';
	public $webserver='';
	
	function __construct()
	{
		if ( $this->isLcl()) {
			$this->webserver = 'ta';
		} else if ( $this->isRe() ) {
			$this->webserver = 'tadev';
		} else if ( $this->isDev() ) {
			$this->webserver = 'ta';
		} else {
			//TODO webserver
		}
		
		if (preg_match('/(lcl)/', $_SERVER["SERVER_NAME"])) {
			$this->http = 'http://';
		} else {
			$this->http = 'https://';
		}
	}
	
	/**
	 * 개발서버인지 여부
	 * @return boolean
	 */
	function isDev()
	{
		$bool=FALSE;
		if( preg_match('/(re|dev)/',$_SERVER['SERVER_NAME']) ){
			$bool=TRUE;
		}
		return $bool;
	}
	
	function isRe()
	{
		$bool=FALSE;
		if( preg_match('/(re)/',$_SERVER['SERVER_NAME']) ){
			$bool=TRUE;
		}
		return $bool;
	}
	
	function isLcl()
	{
		$bool=FALSE;
		if( preg_match('/(lcl)/',$_SERVER['SERVER_NAME']) ){
			$bool=TRUE;
		}
		return $bool;
	}
}