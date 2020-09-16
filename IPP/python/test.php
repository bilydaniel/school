<?php
	
	
	class html_generator
	{
		function __construct()
		{
			$this->output= "";
			$this->results="";
		}
		public function combine()
		{
			$this->output .= $this->results;
		}
		public function add_header($count_correct, $count_incorrect)
		{
			$this->output .= "<!DOCTYPE html><html><html><head><meta charset=\"UTF-8\"><style>
        .box {border: 3px solid black;margin: 10px;background-color:white;}
        .ok {border:3px solid green;}
        .err {border:3px solid red;}
        </style><title>Tests</title></head><body><h1>Test results </h1>
        <h2> Correct: $count_correct Incorrect: $count_incorrect</h2>";
		}
		public function add_correct($test_name)
		{
			$this->results .= "<div class=\"box ok\"><h1>$test_name</h1></div>";;
		}
		public function add_incorrect_rc($test_name, $expected_rc, $returned_rc)
		{
			$this->results .= "<div class=\"box err\"><h1>$test_name</h1>Expected ERR code:  $expected_rc <br> Returned ERR code: $returned_rc</div>";
		}
		public function add_incorrect_output($test_name, $message)
		{
			$message = implode("|",$message);
        	$this->results .= "<div class=\"box err\"><h1>$test_name</h1> $message</div>";
		}
		public function add_tail()
		{
			$this->output .= "</body>
        </html>";
		}

	}

	class file_searcher
	{
		function __construct($recursive)
		{
			$this->recursive = $recursive;
		}

		public function files($directory)
		{
			if($this->recursive)
			{
				$iterator = new RecursiveDirectoryIterator("$directory");
				foreach(new RecursiveIteratorIterator($iterator) as $file)
        		{
		            if (strcmp(".src", strstr($file, '.', false)) != 0)
		                continue;
		            yield $file;
        		}
			}
			else
			{
				foreach (glob("$directory/*.src") as $file)
		        {
		            yield $file;
		        }
			}
		}
	}

	class tester
	{
		function __construct($file,$parser, $interpreter)
		{
			$this->file = $file;
			$this->parser = $parser;
			$this->interpreter = $interpreter;

			$this->parser_result = "";
			$this->interpreter_result = "";

			$this->parser_expected ="";
			$this->intepreter_expected = "";
		}

		public function parse()
		{
			$output;
			$test_name = strstr($this->file, ".src", true);
			if (! file_exists("$this->parser"))
				exit(11);
			exec("php7.3 $this->parser <$test_name 2> /dev/null", $output, $this->parser_result);
			$output_str = implode(" ",$output); 
			$this->parser_expected = $this->check_rc($test_name);
			
			return $output;
		}

		public function interpret()
		{
			$test_name = strstr($this->file, ".src", true);
			$output=array();
			if (! file_exists("$this->interpreter"))
				exit(11);
        	exec("python3.6 $this->interpreter --source=$test_name.temp < $test_name.in 2> /dev/null", $output, $this->interpreter_result);
        	
        	
        	
        	$this->interpreter_expected =$this->check_rc($test_name);
        	
        	$diff=false;
        	#if intepret was successful
        	if(!$this->interpreter_result)
        	{
        		$output_file = implode("\n",$output);
        		file_put_contents("$test_name.temp", $output_file);
        		
        		$diff_result = "";
	            exec("diff $test_name.out $test_name.temp", $diff, $diff_result);
	            if ($diff_result != 0)
	            {
	                $this->interpreter_result = "ERROR"; 
	            }

        	}
        	

        	#$this->interpreter_result = $this->interpreter_result == 0;

        	return $diff;
		}

		function check_rc($test_name)
		{
			$expected_rc = file_get_contents("$test_name.rc");
			return $expected_rc;
		}
	}




	#check for in, out and rc
	function check_all_files($file)
	{
		#slice the file name
		$test_name = strstr($file, ".src", true);

		#create files that dont exist
		if (!file_exists("$test_name.in"))
		{
			$f = fopen("$test_name.in", 'w+')or exit(12);
			fclose($f);
		}
		if (!file_exists("$test_name.out"))
		{
			$f = fopen("$test_name.out", 'w+')or exit(12);
			fclose($f);
		}
		if (!file_exists("$test_name.rc"))
		{
			$f = fopen("$test_name.rc", 'w+')or exit(12);
			fwrite($f, "0");
			fclose($f);
		}
	}
	
	function create_temp($file, $xml)
	{
		$test_name = strstr($file, ".src", true);
		file_put_contents("$test_name.temp", $xml) or exit(12);
	}


	#takes in arguments
	$arguments = getopt("", array("help","directory::","recursive","parse-script::","int-script::","parse-only","int-only"));

	#variables
	$recursive = false;
	$parse_only = false;
	$int_only = false;


	


	#HELLP
	if(array_key_exists("help", $arguments))
	{
		if(count($arguments) != 1)
		{
			exit(10);
		}
		print("Skript projde zadany adresar s testy a vyuzije je pro automaticke otestování spravne funkcnosti");
	}
	
	#DIRECTORY
	if(array_key_exists("directory", $arguments))
	{
		$test_directory = $arguments["directory"];
	}
	else
	{
		$test_directory = getcwd();
	}

	#RECURSIVE
	if(array_key_exists("recursive", $arguments))
	{
		$recursive = true; 
	}
	
	#PARSER
	if(array_key_exists("parse-script", $arguments))
	{
		$parser = $arguments["parse-script"];
	}
	else
	{
		$parser ="parse.php";
	}
	#INTERPRET
	if(array_key_exists("int-script", $arguments))
	{
		$interpret = $arguments["int-script"];
	}
	else
	{
		$interpret ="interpret.py";
	}

	if(array_key_exists("parse-only", $arguments))
	{
		$parse_only = TRUE;
	}

	if(array_key_exists("int-only", $arguments))
	{
		$int_only = TRUE;	
	}

	if($int_only && $parse_only)
	{
		exit(10);
	}

	if($int_only && array_key_exists("parse-script", $arguments) || $parse_only && array_key_exists("int-script", $arguments))
	{
		exit(10);
	}

	$html = new html_generator();
	

	$count_correct = 0;
	$count_incorrect =0;
	#create file_searcher(recursive or not)
	if(!file_exists($test_directory))
		exit(11);
	$file_searcher = new file_searcher($recursive);
	
	
	#each returned file
	foreach($file_searcher->files($test_directory) as $file)
	{	
		#create missing files
		check_all_files($file);
		$tester = new tester($file,$parser,$interpret);
		$test_name = strstr($file, ".src", true);
		if($parse_only)
		{
			$xml = $tester->parse();
			
			file_put_contents("$test_name.xml", $xml) or exit(12);
			$diff = exec("java -jar /pub/courses/ipp/jexamxml/jexamxml.jar $test_name.xml $test_name.out delta.xml /pub/courses/ipp/jexamxml/options");

			if($tester->parser_result == 0 )
			{
				#create xml file and send it to the interpretr
           		if($diff == FALSE)
           		{
           			$count_correct++;
            		$test_name = preg_replace('/^.*\//','',$file);
           			$html->add_correct($test_name);
           		}
           		else
           		{
           			$count_incorrect++;
	                $test_name = preg_replace('/^.*\//','',$file);
	                $html->add_incorrect_output($test_name, $diff);	
           		}
            	
			}
			else
			{
				$count_incorrect++;
				$test_name = preg_replace('/^.*\//','',$file);
				$html->add_incorrect_rc($test_name, $tester->parser_expected,$tester->parser_result);
			}
		}
		elseif($int_only)
		{
			$xml = " ";
			create_temp($file,$xml);
			$test_name = strstr($file, ".src", true);
			copy("$test_name.src","$test_name.temp");
			$diff = $tester->interpret();

			if($tester->interpreter_result == 0 )
			{
				#create xml file and send it to the interpretr
           		if($diff == FALSE)
           		{
           			$count_correct++;
            		$test_name = preg_replace('/^.*\//','',$file);
           			$html->add_correct($test_name);
           		}
           		else
           		{
           			$count_incorrect++;
	                $test_name = preg_replace('/^.*\//','',$file);
	                $html->add_incorrect_output($test_name, $diff);	
           		}
            	
			}
			else
			{
				$count_incorrect++;
				$test_name = preg_replace('/^.*\//','',$file);
				$html->add_incorrect_rc($test_name, $tester->interpreter_expected,$tester->interpreter_result);
			}
		}
		else
		{

			$xml = $tester->parse();

			#parser was successful
			if($tester->parser_result == 0)
			{
				#create xml file and send it to the interpretr
				create_temp($file,$xml);
				$diff = $tester->interpret();
				if (strcmp($tester->interpreter_result, "ERROR") == 0)
            	{
	                $count_incorrect++;
	                $test_name = preg_replace('/^.*\//','',$file);
	                $html->add_incorrect_output($test_name, $diff);
           		}
           		else if(!$tester->interpreter_result)
            	{
                	$count_correct++;
                	$test_name = preg_replace('/^.*\//','',$file);
               		$html->add_correct($test_name);
            	}
            	else if(($tester->interpreter_result))
            	{
                	$count_incorrect++;
					$test_name = preg_replace('/^.*\//','',$file);
					$html->add_incorrect_rc($test_name, $tester->interpreter_expected,$tester->interpreter_result);
            	}
			}
			else
			{
				$count_incorrect++;
				$test_name = preg_replace('/^.*\//','',$file);
				$html->add_incorrect_rc($test_name, $tester->parser_expected,$tester->parser_result);
			}
			
		}
		if (file_exists("$test_name.temp"))
        	{
            	unlink("$test_name.temp");
        	}
		
	}
	$html->add_header($count_correct, $count_incorrect);
	$html->combine();
	$html->add_tail();
	fwrite(STDOUT, $html->output);


	exit(0);


?>