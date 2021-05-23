<?php


namespace core\form;


class Form
{
	public static function begin($name,$action,$method='POST',$options = [])
	{
		$attributes = [];
		foreach ($options as $key => $value) {
			$attributes[] = "$key=\"$value\"";
		}
		echo sprintf('<form name="%s" action="%s" method="%s" %s>',$name, $action, $method, implode(" ", $attributes));
		return new Form();
	}

	public static function end()
	{
		echo '</form>';
	}

	public function select($name,$attributes=[])
	{
		return new Select($name, $attributes);
	}

	public function input($name,$attributes=[])
	{
		return new Input($name,$attributes);
	}

	public function inputRadio($name,$attributes=[])
	{
		return new InputRadio($name,$attributes);
	}

	public function inputCheckbox($name,$attributes=[])
	{
		return new InputCheckbox($name,$attributes);
	}

}