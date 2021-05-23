<?php


namespace core\form;


class Select
{
	public $name;
	public $attributes;
	public $options;

	public function __construct($name,array $attributes=[])
	{
		$this->name = $name;

		$this->attributes=[];
		foreach ($attributes as $key => $value) {
			$this->attributes[] = "$key=\"$value\"";
		}

		$this->options=[];
	}

	public function __toString()
	{
		return sprintf('
		<select name="%s" %s >
			%s
		</select>
		',$this->name, implode(" ", $this->attributes)
		, $this->renderOptions($this->options)
		);
	}

	public function renderOptions(array $options)
	{
		$render='';
		foreach ($options as $value=>$name){
			$render.=sprintf('
			 <option value="%s">%s</option>
			 ',$value,$name
			);
		}
		return $render;
	}

	public function setOptions(array $options=[])
	{
		$this->options=$options;
		return $this;
	}
}