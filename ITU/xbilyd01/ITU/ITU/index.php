<?php
    mb_internal_encoding("ISO-8859-2");
    // Callback pro automatické načítání tříd controllerů a modelů
    function autoloadFunkce($trida)
    {

        // Končí název třídy řetězcem "Kontroler" ?
        if (preg_match('/Kontroler$/', $trida)) 
            require("kontrolery/" . $trida . ".php");
        else
            require("modely/" . $trida . ".php");
    }

    // Registrace callbacku (Pod starým PHP 5.2 je nutné nahradit fcí __autoload())
    spl_autoload_register("autoloadFunkce");

    ini_set('session.save_path', 'tmp');
    if(session_id() == '') {
        session_start();
    }

    // Připojení k databázi
    Db::pripoj("localhost", "xmarek69", "8afunope", "xmarek69");
    
    // Vytvoření routeru a zpracování parametrů od uživatele z URL
    $smerovac = new SmerovacKontroler();
    $parametry = array_merge($_GET, $_SESSION);
    $smerovac->zpracuj($parametry);//array($_SERVER['REQUEST_URI']));

    // Vyrenderování šablony
    $smerovac->vypisPohled();

    $pdo = null;
    include_once 'php/include/db_func.php';
    connect($pdo); 

?>
        <?php
/*
            if(isset($_GET) && isset($_GET['akce']) && $_GET['akce'] == "registrace"){
                include_once "php/include/registration.php";
            }else if(isset($_SESSION) && isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == true){
                if(isset($_GET) && isset($_GET['akce'])){
                    include_once "php/akce/" . $_GET['akce'] . ".php";
                }else{
                    include_once "php/akce/sklad.php";
                }
            }else{
                include_once "php/include/login.php";
            }*/
        ?>
