<?php


namespace core\form;


class Input
{
	const TYPE_TEXT = 'text';
	const TYPE_PASSWORD = 'password';
	const TYPE_FILE = 'file';
	const TYPE_EMAIL = 'email';
	const TYPE_HIDDEN = 'hidden';
	const TYPE_BUTTON = 'button';
	const TYPE_RADIO = 'radio';
	const TYPE_CHECKBOX = 'checkbox';

	public $name;
	public $attributes;
	public $type;

	public function __construct($name,array $attributes=[])
	{
		$this->name = $name;

		$this->attributes=[];
		foreach ($attributes as $key => $value) {
			$this->attributes[] = "$key=\"$value\"";
		}

		$this->type=self::TYPE_TEXT;
	}

	public function __toString()
	{
		return sprintf('<input type="%s" name="%s" value="%s" %s>',
			$this->type,
			$this->name,
			$_POST[$this->name],
			implode(" ", $this->attributes)
        );
	}

	public function setPassword()
	{
		$this->type=self::TYPE_PASSWORD;
		return $this;
	}

	public function setEamil()
	{
		$this->type=self::TYPE_EMAIL;
		return $this;
	}
}