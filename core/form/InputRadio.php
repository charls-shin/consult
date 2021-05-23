<?php


namespace core\form;


class InputRadio extends Input
{
	public $aValue;

	public function __construct($name,array $attributes=[])
	{
		parent::__construct($name,$attributes);

		$this->type=self::TYPE_RADIO;
		$this->aValue=[];
	}

	public function __toString()
	{
		$render='';
		foreach ($this->aValue as $value=>$nm)
		{
			$render.=$this->renderInput( $value , $nm );
		}
		return $render;
	}

	public function setRadio(array $aValue=[])
	{
		$this->aValue=$aValue;
		return $this;
	}

	public function renderInput($value , $nm)
	{
		return sprintf('<input type="%s" name="%s" id="%s" value="%s" %s><label for="%s">%s</label>',
			$this->type,
			$this->name,
			'id_radio_'.$this->name.'_'.$value,
			$value,
			implode(" ", $this->attributes),
			'id_radio_'.$this->name.'_'.$value,
			$nm
        );
	}
}