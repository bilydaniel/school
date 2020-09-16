<?php
    if($_POST){
        if(isset($_POST["username"]) && isset($_POST["password"])){
            if(true/*user_logged_in($_POST["username"], $_POST["password"])*/){
                $_SESSION['user'] = $_POST["username"];
                $_SESSION['psw'] = "admin";
                $_SESSION['logged_in'] = true;
                $_SESSION['id'] = get_id_uzivatele($pdo);
                header("Location: index.php");
            }else{
                echo "<h2 class='error_for_user'>©patné pøihla¹ovací údaje!</h2>";
            }

        }
    }
?>

<div id="login">
    <form method="post">
        <h2>Pøihlá¹ení</h2>
        <p class="hint">U¾ivatelské jméno:</p>
        <input type="text" name="username" placeholder="login"/ autofocus>
        <p class="hint">Heslo:</p>
        <input type="password" name="password" placeholder="heslo"/>
        <input type="submit" name="login" value="Pøihlásit">
    </form>
</div>
<div id="no_account">Stále nemáte úèet? <a href="?akce=registrace">Zaregistrovat!</a></div>
