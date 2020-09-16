<?php
require_once("./classParser.php");
require_once("./classProgram.php");
require_once("./classInstruction.php");
require_once("./classArgChecker.php");
require_once("./classArgument.php");

#checks header of the file and takes the input, works as a generator
function Input()
{
	
	$codeName = strtolower(fgets(STDIN));
	#remove comment
	if(($position = strpos($codeName, "#")) !== false)
	{
		$codeName = substr($codeName, 0, $position );

	}
	#check for code name
	if(trim($codeName) !== ".ippcode19")
	{
		fwrite(STDERR, "chybi hlavicka souboru\n");
		exit(21);	
	}

	#start loading lines
	while($line = fgets(STDIN))
	{
		#check for comments, remove them
		if(($position = strpos($line, "#")) !== false)
		{
			$line = substr($line, 0, $position );

		}

		#remove multiple spaces and tabs
		$line = trim($line);
		$line = preg_replace('/(\s|\t)+/',' ', $line);

		#remove empty lines
		if(strlen($line) == 0)
			continue;

		#yield insead of return so the function doesent stop

		yield $line;
	} 
}
#prints help if asked for
function PrintHelp($argc, $argv)
{
	if($argc !== 1)
	{
		if($argc <= 2 && $argv[1] == "--help")
		{
			echo "Skript typu filtr (parse.php v jazyce PHP 7.3) nacte ze standardniho vstupu zdrojovy kod v IPPcode19, zkontroluje lexikalni a syntaktickou spravnost kodu a vypise na standardni vystup XML reprezentaci programu.\n";
			exit(0);
		}
		else
		{
			fwrite(STDERR, "spatne parametry\n");
			exit(10);	
		}
	}
}
#//MAIN
$order = 1;

#print help if needed
PrintHelp($argc, $argv);

#read the input using generator
$generator = Input();

#create new parser object
$parser = new Parser();

#new program created in parser
$program = $parser->Parse($generator);

#convert the program to xml
$xmlTree = $program->toXml();

#write out xml
fwrite(STDOUT,$xmlTree->saveXML());

#success
return 0;

?>