<?php


namespace consult\core;


class Router
{
	public static $path;
	public static $fn;
	
	public function __construct()
	{
		self::$path=$this->getPath();
		self::$fn=$this->getFn();
	}
	
	public function getMethod()
	{
		return strtolower($_SERVER['REQUEST_METHOD']);
	}
	
	public function getUrl()
	{
		$path = $_SERVER['REQUEST_URI'];
		$position = strpos($path, '?');
		if ($position !== false) {
			$path = substr($path, 0, $position);
		}
		
		return $path;
	}
	
	public function getAUrl()
	{
		$url=$this->getUrl();
		$aUrl=explode('/',$url);
		array_shift($aUrl); // /
		array_shift($aUrl); // consult
		return $aUrl;
	}
	
	public function getPath()
	{
		$path='';
		$aUrl=$this->getAUrl();
		if( count($aUrl)>1 ) {
			array_pop($aUrl); // $this->>fn
			$path=implode('/',$aUrl);
		}

		return $path;
	}
	
	public function getFn()
	{
		$aUrl=$this->getAUrl();
		if( count($aUrl) == 1){
			$fn=$aUrl[0];
		}else{
			$fn=array_pop($aUrl);
		}
		
		if( $fn == '' ){
			$fn='index';
		}else{
			$fn=preg_replace(['/(\.htm)$/','/(\.html)$/','/(\.php)$/'],['','',''],$fn);
		}
		
		return $fn;
	}
	
	public function isGet()
	{
		return $this->getMethod() === 'get';
	}
	
	public function isPost()
	{
		return $this->getMethod() === 'post';
	}
	
	public function getBody()
	{
		$data = [];
		if ($this->isGet()) {
			foreach ($_GET as $key => $value) {
				$data[$key] = filter_input(INPUT_GET, $key, FILTER_SANITIZE_SPECIAL_CHARS);
			}
		}
		if ($this->isPost()) {
			foreach ($_POST as $key => $value) {
				$data[$key] = filter_input(INPUT_POST, $key, FILTER_SANITIZE_SPECIAL_CHARS);
			}
		}
		return $data;
	}
}