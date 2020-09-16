<?php
#stores instruction name and its arguments
class Instruction
{
	protected $instr;
	public $arg1, $arg2, $arg3;

	public function __construct($instr,$arg1 = NULL, $arg2 = NULL, $arg3 = NULL)
	{
		$this->instr = $instr;
		$this->arg1 = new Argument($arg1);
		$this->arg2 = new Argument($arg2);
		$this->arg3 = new Argument($arg3); 
		
	}

	public function checkArgs()
	{
		$argChecker = new ArgChecker();

		switch ($this->instr) {
			#var symb 
			case "MOVE":
			case "INT2CHAR":
			case "STRLEN":
			case "TYPE":
				if( !$argChecker->checkVar($this->arg1) or 
					!$argChecker->checkSymb($this->arg2) or 
					!is_null($this->arg3->value))
				{
					$this->argError($this->instr);
				}
			break;
			
			#no arguments
			case "CREATEFRAME":
			case "PUSHFRAME":
			case "POPFRAME":
			case "RETURN":
			case "BREAK":
				if( !is_null($this->arg1->value) or 
					!is_null($this->arg2->value) or 
					!is_null($this->arg3->value))
				{
					$this->argError($this->instr);
				}
			break; 

			#var
			case "DEFVAR":
			case "POPS":
				if( !$argChecker->checkVar($this->arg1) or 
					!is_null($this->arg2->value) or 
					!is_null($this->arg3->value))
				{
					$this->argError($this->instr);
				}
			break;

			#label
			case "CALL":
			case "LABEL":
			case "JUMP":
				if( !$argChecker->checkLabel($this->arg1) or 
					!is_null($this->arg2->value) or 
					!is_null($this->arg3->value))
				{
					$this->argError($this->instr);
				}
			break;  
			
			#symbol
			case "PUSHS":
			case "WRITE":
			case "EXIT":
			case "DPRINT":
				if( !$argChecker->checkSymb($this->arg1) or 
					!is_null($this->arg2->value) or 
					!is_null($this->arg3->value))
				{
					$this->argError($this->instr);
				}
			break;  
			
			#var symbol symbol
			case "ADD":
			case "SUB":
			case "MUL":
			case "IDIV":
			case "LT":
			case "GT":
			case "EQ":
			case "AND":
			case "OR":
			case "NOT":
			case "STRI2INT":
			case "CONCAT":
			case "GETCHAR":
			case "SETCHAR":
				if( !$argChecker->checkVar($this->arg1) or 
					!$argChecker->checkSymb($this->arg2) or 
					!$argChecker->checkSymb($this->arg3))
				{
					$this->argError($this->instr);
				}
			break;  

			#var type
			case "READ":
				if( !$argChecker->checkVar($this->arg1) or 
					!$argChecker->checkType($this->arg2) or 
					!is_null($this->arg3->value))
				{
					$this->argError($this->instr);
				}
			break;  
		
			#label symbol symbol
			case "JUMPIFEQ":
			case "JUMPIFNEQ":
				if( !$argChecker->checkLabel($this->arg1) or 
					!$argChecker->checkSymb($this->arg2) or 
					!$argChecker->checkSymb($this->arg3))
				{
					$this->argError($this->instr);
				}
			break;  


			default:
				fwrite(STDERR,"Neznama instrukce\n");
				exit(22);
				break;
		}
	}

	public function toXml($tree, $root)
	{
		#instruction
		$instruction = $tree->createElement("instruction");
		$instruction->setAttribute("order",$GLOBALS["order"]++);
		$instruction->setAttribute("opcode",$this->instr);

		#arguments
		if($this->arg1->value !== null)
		{
			$arg1 = $tree->createElement("arg1", htmlspecialchars($this->arg1->value));	
			$arg1->setAttribute("type", $this->arg1->type);
			$instruction->appendChild($arg1);
		}	
		if($this->arg2->value !== null)
		{
			$arg2 = $tree->createElement("arg2",htmlspecialchars($this->arg2->value) );	
			$arg2->setAttribute("type", $this->arg2->type);
			$instruction->appendChild($arg2);
		}	
		if($this->arg3->value !== null)
		{
			$arg3 = $tree->createElement("arg3",htmlspecialchars($this->arg3->value) );	
			$arg3->setAttribute("type", $this->arg3->type);
			$instruction->appendChild($arg3);
		}	
		$root->appendChild($instruction);
	}

	public function argError($instr)
	{
		fwrite(STDERR, "wrong arguments in ");
		fwrite(STDERR, $instr);
		fwrite(STDERR,"\n");
		exit(23);
	}


}


?>