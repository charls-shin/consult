<?php

function fromUTF($arg)
{
	if( is_array($arg) )
	{
		foreach( $arg as $k=>$v )
		{
			$arg[$k] = fromUTF($v);
		}
	}
	else if( is_object($arg) )
	{
		foreach( get_object_vars($arg) as $k=>$v )
		{
			$arg->$k = fromUTF($v);
		}
	}
	else
	{
		$arg = iconv('UTF-8','CP949',$arg);
	}
	return $arg;
}	//	end of function fromUTF($arg)

function toUTF($arg)
{
	if( is_array($arg) )
	{
		foreach( $arg as $k=>$v )
		{
			$arg[$k] = toUTF($v);
		}
	}
	else if( is_object($arg) )
	{
		foreach( get_object_vars($arg) as $k=>$v )
		{
			$arg->$k = toUTF($v);
		}
	}
	else
	{
		$arg = iconv('CP949','UTF-8',$arg);
	}
	return $arg;
}	//	end of function toUTF($arg)