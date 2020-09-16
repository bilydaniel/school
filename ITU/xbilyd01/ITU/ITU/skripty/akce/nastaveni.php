<main id="nastaveni">

<?php
    if(isset($_SESSION) && isset($_SESSION['settings_saved']) && $_SESSION['settings_saved'] == true){
        unset($_SESSION['settings_saved']);
        echo "<h2 style='color:green'>Zmìny byli ulo¾eny</h2>";
    }
    $nastaveni = get_nastaveni_array($pdo);
    $nazev = $nastaveni['nazev']? "checked" : "";
    $dodavatel = $nastaveni['dodavatel']? "checked" : "";
    $expirace = $nastaveni['expirace']? "checked" : "";
    $mj = $nastaveni['mj']? "checked" : "";
    $mnozstvi = $nastaveni['mnozstvi']? "checked" : "";
?>
    <h1>Nastavení</h1>

    <form method="post"  action="php/scripts/save_settings.php">
        <h2>Vyberte, které vlastnosti zbo¾í chcete ukládat:</h2>
        <table id="properties_table">
            <tr>
                <td>Název</td>
                <td>Dodavatel</td>
                <td>Expirace</td>
                <td>Mìrná jednotka</td>
                <td>Mno¾ství</td>
            </tr> 
            <tr>
                <td><input type="checkbox" name="nazev" value="checked" <?php echo $nazev ?>></td>
                <td><input type="checkbox" name="dodavatel" value="checked" <?php echo $dodavatel ?>></td>
                <td><input type="checkbox" name="expirace" value="checked" <?php echo $expirace ?>></td>
                <td><input type="checkbox" name="mj" value="checked" <?php echo $mj ?>></td>
                <td><input type="checkbox" name="mnozstvi" value="checked" <?php echo $mnozstvi ?>></td>
            </tr>  
        </table>
        
        <input type="submit" name="save" value="Ulo¾it">
    </form>

</main>