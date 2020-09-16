<?php
#checks the syntax of arguments
class ArgChecker
{
	public function checkSymb($symb)
	{
		return ($this->checkVar($symb) or $this->checkConst($symb));

	}
	public function checkVar($var)
	{
		$result = preg_match("/^(TF|LF|GF)@((\p{L}|-|[_$&%*!?])(\p{L}|-|[_$&%*!?]|[0-9])*)$/u", $var->value);

		if($result)
		{
			$var->type = "var";
		}

		return $result;
	}
	private function checkConst($const)
	{
		$result = (preg_match("/^nil@nil$/u", $const->value)or #nil@nil
		 	  preg_match("/^bool@(true|false)$/u", $const->value)or #bool@true|false
		 	   preg_match("/^int@([\+\-])?([0-9])+$/u", $const->value)or #int@
		 	   preg_match("/^string@(\p{L}|_|[^(\w\\\)]|\d|\\\([0-9][0-9][0-9]))*$/u", $const->value)); 
		 	   
		 	   
		
		if($result)
		{
			$parts = explode("@",$const->value);
			$const->type = $parts[0];
			$const->value = $parts[1];
		} 	   
		
		return $result;

	}

	public function checkLabel($label)
	{
		$result = preg_match("/^((\p{L}|-|[_$&%*!?])(\p{L}|-|[_$&%*!?]|[0-9])*)$/u", $label->value);
		
		if($result)
		{
			$label->type = "label";
			$label->value = $label->value;
		} 	   
		
		return $result;
	}

	public function checkType($type)
	{
		$result = preg_match("/^(int|string|bool)$/u", $type->value);
		
		if($result)
		{
			$type->type = "type";
			$type->value = $type->value;
		} 	   
		
		return $result;
	}


}


?>