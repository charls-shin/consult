<?php


namespace consult\core;


class Session
{
	protected const SUB_KEY = 'cs_';
	
	public function __construct()
	{
		if(!isset($_SESSION))
		{
			session_start();
		}
	}
	
	public function set($key, $value)
	{
		$_SESSION[self::SUB_KEY.$key] = $value;
	}
	
	public function get($key)
	{
		return $_SESSION[self::SUB_KEY.$key] ?? false;
	}
	
	public function remove($key)
	{
		unset($_SESSION[self::SUB_KEY.$key]);
	}
	
	public function destroy()
	{
		foreach ($_SESSION as $key =>$value)
		{
			if(preg_match('/^('.self::SUB_KEY.')/', $key)){
				$this->remove($key);
			}
		}
	}
}