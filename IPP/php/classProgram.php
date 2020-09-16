<?php
#stores all the instuctions as an array 
class Program
{
	private $instructionArray;

	public function __construct($instructionArray)
	{
		$this->instructionArray = $instructionArray;
	}

	public function toXml()
	{
		#create new document
		$tree = new DOMDocument("1.0", "UTF-8");
		$tree->formatOutput = true;
		#create root of the tree
		$root =$tree->createElement("program");
		$root->setAttribute("language","IPPcode19");
		#add instruction into the tree
		foreach($this->instructionArray as $instruction)
		{
			$instruction->checkArgs();
			$instruction->toXml($tree, $root);
		}

		$tree->appendChild($root);

		return $tree;
	}

}



?>