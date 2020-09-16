<?php
ini_set("default_charset", "ISO-8859-2");

//Jelikoz se tento php skript spousti mimo index.php, je potreba ziskat prislusne tridy a pripojit se  k databazi
require("../modely/Db.php");
require("../modely/SpravceZbozi.php");
Db::pripoj("localhost", "xmarek69", "8afunope", "xmarek69");

$spravceZbozi = new SpravceZbozi();
$id = $_POST['id'];

if(isset($_POST['nazev'])){
	$nazev = $_POST['nazev'];	
	$nazev = iconv("UTF-8", "ISO-8859-2", $nazev);
	$zbozi = $spravceZbozi->vratZboziPodlePodminkyAHodnot($id, "nazev=?", array($nazev));
}

$prvni_vlastnost = 1;
$object = "{";
	foreach ($zbozi as $key => $value) {
		if($prvni_vlastnost == 0){
			$object .= ", ";
		}
		$object .= "\"$key\": \"" . $value . "\"";
		if($prvni_vlastnost == 1){
			$prvni_vlastnost = 0;
		}
	}

$object .= "}";
echo $object;

?>