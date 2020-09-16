<?php
    if(isset($_POST) && isset($_POST['save'])){//Ulozit zbozi
        session_start();
        $_SESSION['goods_added'] = true;    
        include '../include/db_func.php';
        $pdo;
        connect($pdo);
        pridat_pohyb_zbozi($pdo, $_POST);    
        //header("Location: ../../index.php?akce=prijem");
    }
?>