<?php
#class for the arguments data(value and type)
class Argument
{
	public $type, $value;
	
	public function __construct($value)
	{
		$this->value = $value;
	}

	public function setType($type)
	{
		$this->type = $type;
	}


}
?>