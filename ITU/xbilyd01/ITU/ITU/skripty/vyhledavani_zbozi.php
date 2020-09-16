<?php
ini_set("default_charset", "ISO-8859-2");

//Jelikoz se tento php skript spousti mimo index.php, je potreba ziskat prislusne tridy a pripojit se  k databazi
require("../modely/Db.php");
require("../modely/SpravceZbozi.php");
Db::pripoj("localhost", "xmarek69", "8afunope", "xmarek69");

$spravceZbozi = new SpravceZbozi();
$id = $_POST['id'];

if(isset($_POST['fraze'])){
	$fraze = $_POST['fraze'];	
	$fraze = iconv("UTF-8", "ISO-8859-2", $fraze);
	$zbozi = $spravceZbozi->vratSeznamZboziPodleFraze($id, "nazev like ? or dodavatel like ? or mj like ?", array("%" . $fraze . "%", "%" . $fraze . "%", "%" . $fraze . "%"));
}else{
	$nazev = $_POST['nazev'];	
	$nazev = iconv("UTF-8", "ISO-8859-2", $nazev);
	$dodavatel = $_POST['dodavatel'];	
	$dodavatel = iconv("UTF-8", "ISO-8859-2", $dodavatel);
	$mj = $_POST['mj'];	
	$mj = iconv("UTF-8", "ISO-8859-2", $mj);
	$zbozi = $spravceZbozi->vratSeznamZboziPodleFraze($id, "nazev like ? and dodavatel like ? and mj like ?", array("%" . $nazev . "%", "%" . $dodavatel . "%", "%" . $mj . "%"));
}

$prvni_vlastnost = 1;
$object = "{";
for($i=0; $i < count($zbozi); $i++){
	if($i > 0){
		$object .= ", ";
	}
	$object .= "\"$i\": ";
	$object .= "{";
	foreach ($zbozi[$i] as $key => $value) {
		if($prvni_vlastnost == 0){
			$object .= ", ";
		}
		$object .= "\"$key\": \"" . $zbozi[$i][$key] . "\"";
		if($prvni_vlastnost == 1){
			$prvni_vlastnost = 0;
		}
	}
	$object .= "}";
	$prvni_vlastnost = 1;
}
$object .= "}";
echo $object;

?>