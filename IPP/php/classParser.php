<?php
#creates and stores instructions from input
class Parser
{
	public function Parse($generator)
	{
		#array of instruction objects
		$instructions = array();
		
		foreach($generator as $line)
		{
			array_push($instructions, $this->PutInstruction($line));
		}

		return new Program($instructions);
	}

	#lexical analysis
	private function PutInstruction($line)
	{
		#split the line
		$words = explode(" ", $line);

		#put the instruction to uppercase
		$instruction = strtoupper($words[0]);

		#create instructions 
		if(!empty($words[3]))
			return new Instruction($instruction, $words[1], $words[2], $words[3]);
		elseif(!empty($words[2]))
			return new Instruction($instruction, $words[1], $words[2]);
		else if(!empty($words[1]))
			return new Instruction($instruction, $words[1]);
		else
			return new Instruction($instruction);


	}



} 


?>