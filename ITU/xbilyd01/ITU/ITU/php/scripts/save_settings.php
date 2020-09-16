<?php
    if(isset($_POST) && isset($_POST['save'])){//Ulozit nastaveni
        session_start();
        $_SESSION['settings_saved'] = true;        
        include '../include/db_func.php';
        $pdo;
        connect($pdo);
        ulozit_nastaveni($pdo, $_POST);  
        header("Location: ../../index.php?akce=nastaveni");
    }
?>