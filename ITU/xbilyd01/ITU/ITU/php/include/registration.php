<?php
/*if(isset($_GET) && isset($_GET['new']) && $_GET['new'] == "true"){//Uzovatel se chce zaregistrovat-klikl na "Zaregostrovat"
    
    session_start();
    $_SESSION['registration'] = true;
    header("Location: ../index.php");
}else */
if(isset($_POST) && isset($_POST['username'])){//Uzivatel odeslal registracni formular ke zpracovani
    create_new_user_in_table_itu_users($_POST['username'], $_POST['password']);
    session_start();
    $_SESSION['reg'] = true;
    header("Location: index.php");
}else{//zobraz registracni formular
?>

<div id="login">
    <form method="post">
        <h2>Registrace</h2>
        <p class="hint">U¾ivatelské jméno:</p>
        <input type="text" name="username" placeholder="login"/>
        <p class="hint">Heslo:</p>
        <input type="password" name="password" placeholder="heslo"/>
        <input type="submit" name="registration" value="Zaregistrovat">
    </form>
</div>

<?php
}

?>