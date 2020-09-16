<?php

include_once 'config.php';

function disconnect(&$pdo) {
    $pdo = null;
}

function connect(&$pdo){
    $dsn = "mysql:host=localhost;dbname=xmarek69";
    $user = "xmarek69";
    $passwd = "8afunope";

    try{
        $pdo = new PDO($dsn, $user, $passwd);
    }catch(PDOException $e){
        echo "Connection failed: " . $e->getMessage();
    }
    
}


function get_last_nth_goods($pdo, $count){
    $zbozi;
    $sql = "SELECT DISTINCT nazev, dodavatel, expirace, mj from itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele = (SELECT id_uzivatele from itu_uzivatel where username='" . $_SESSION['user'] . "')";
    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    $row = $stmt->fetch();
    for ($i = 0; $row ; $i++) {
        $zbozi[$i]['nazev'] = $row['nazev'];
        $zbozi[$i]['dodavatel'] = $row['dodavatel'];
        $zbozi[$i]['expirace'] = $row['expirace'];
        $zbozi[$i]['mj'] = $row['mj'];
        $row = $stmt->fetch();
    }

    for($i = 0; $i < count($zbozi); $i++){
        $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                FROM itu_zbozi
                INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                AND id_uzivatele = (SELECT id_uzivatele from itu_uzivatel where username='" . $_SESSION['user'] . "')
                AND smer =0), 0) as mnozstvi";

        $stmt = $pdo->prepare($sql);
        $stmt->execute();
        $row = $stmt->fetch();
        $pocet_prichozich = $row['mnozstvi'];

        $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                FROM itu_zbozi
                INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                AND id_uzivatele = (SELECT id_uzivatele from itu_uzivatel where username='" . $_SESSION['user'] . "')
                AND smer =1), 0) as mnozstvi";

        $stmt = $pdo->prepare($sql);
        $stmt->execute();
        $row = $stmt->fetch();
        $pocet_odchozich = $row['mnozstvi'];




        $zbozi[$i]['mnozstvi'] = $pocet_prichozich - $pocet_odchozich;

    }


    return $zbozi;
}

//Vrati asociativni pole nastaveni pro daneho uzivatele
function get_nastaveni_array($pdo){
    $nastaveni;

    $sql = "SELECT * FROM itu_nastaveni";
    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    $row = $stmt->fetch();

    $nastaveni['nazev'] = $row['nazev'];
    $nastaveni['dodavatel'] = $row['dodavatel'];
    $nastaveni['expirace'] = $row['expirace'];
    $nastaveni['mj'] = $row['mj'];
    $nastaveni['mnozstvi'] = $row['mnozstvi'];

    return $nastaveni;
}


function get_nastaveni_cell($pdo, $i, $typ){
    $nastaveni = get_nastaveni_array($pdo);
    $zbozi = get_last_nth_goods($pdo, 10);
    $typ2 = get_type_without_diacritics($typ);
    if($nastaveni[$typ2] == 1){
        return "<td>{$zbozi[$i][$typ2]}</td>";
    }else{
        return '';
    }
}

function get_nastaveni_header($pdo, $typ){
    $nastaveni = get_nastaveni_array($pdo);
    $typ2 = get_type_without_diacritics($typ);
    if($nastaveni[$typ2] == 1){
        return "<th>$typ</th>";
    }else{
        return '';
    }

}

function get_type_without_diacritics($type){
    if($type == "Název"){
        $typ2 = "nazev";
    }else if($type == "Mno¾ství"){
        $typ2 = "mnozstvi";
    }else{
        $typ2 = strtolower($type);
    }
    return $typ2;

}

function get_id_uzivatele($pdo){
    $sql = "SELECT id_uzivatele from itu_uzivatel where username='" . $_SESSION['user'] . "'";    
    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    $row = $stmt->fetch();
    return $row['id_uzivatele'];
}


/* Vlozi do tabulky 'itu_pohyb_zbozi' pohyb zbozi
 */
function pridat_pohyb_zbozi($pdo, $zbozi_array){


    $id_uzivatele = get_id_uzivatele($pdo);
    $datum = $zbozi_array['datum_pohybu'];
    $poznamka = $zbozi_array['poznamka'];
    $smer = $zbozi_array['smer'];

    $sql = "INSERT INTO itu_pohyb_zbozi (id_uzivatele, datum, poznamka, smer) VALUES (?, ?, ?, ?)";
    $stmt = $pdo->prepare($sql);
    $stmt->execute(array($id_uzivatele, $datum, $poznamka, $smer));
    $row = $stmt->fetch();

    $id_pohybu = $pdo->lastInsertId();

    for($i = 0; $i < ((count($zbozi_array)-4)/5); $i++){
        pridat_zbozi($pdo, array_slice($zbozi_array,5 * $i,5), $i, $id_pohybu);
    }

}
/* Vlozi do tabulky 'itu_zbozi' zbozi
 */
function pridat_zbozi($pdo, $zbozi_array, $i, $id_poh){

    $id_pohybu = $id_poh;
    $nazev = $zbozi_array['nazev' . $i];
    $dodavatel = $zbozi_array['dodavatel' . $i];
    $expirace = $zbozi_array['expirace' . $i];
    $mj = $zbozi_array['mj' . $i];
    $mnozstvi = $zbozi_array['mnozstvi' . $i];

    $sql = "INSERT INTO itu_zbozi (id_pohybu, nazev, dodavatel, expirace, mj, mnozstvi) VALUES (?, ?, ?, ?, ?, ?)";
    $stmt = $pdo->prepare($sql);
    $stmt->execute(array($id_pohybu, $nazev, $dodavatel, $expirace, $mj, $mnozstvi));
    $row = $stmt->fetch();

}

function ulozit_nastaveni($pdo, $nastaveni){   
    $field_string = "";
    $exec_array = array();
    $count = 0;
    if(isset($nastaveni['nazev'])){
        $exec_array[$count++] = 1;
    }else{
        $exec_array[$count++] = 0;
    }
    if(isset($nastaveni['dodavatel'])){
        $exec_array[$count++] = 1;
    }else{
        $exec_array[$count++] = 0;
    }
    if(isset($nastaveni['expirace'])){
        $exec_array[$count++] = 1;
    }else{
        $exec_array[$count++] = 0;
    }
    if(isset($nastaveni['mj'])){
        $exec_array[$count++] = 1;
    }else{
        $exec_array[$count++] = 0;
    }
    if(isset($nastaveni['mnozstvi'])){
        $exec_array[$count++] = 1;
    }else{
        $exec_array[$count++] = 0;
    }

    $exec_array[$count] = $_SESSION['id'];

    $sql = "UPDATE itu_nastaveni SET nazev = ?, dodavatel = ?, expirace = ?, mj = ?, mnozstvi = ? WHERE id_uzivatele = ?";
    $stmt = $pdo->prepare($sql);
    $stmt->execute($exec_array);
    $row = $stmt->fetch();
}



function get_poslednich_n_pohybu_zbozi($pdo, $count){
    $zbozi;
    $sql = "SELECT DISTINCT nazev, dodavatel, expirace, mj from itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele = (SELECT id_uzivatele from itu_uzivatel where username='" . $_SESSION['user'] . "')";
    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    $row = $stmt->fetch();
    for ($i = 0; $row ; $i++) {
        $zbozi[$i]['nazev'] = $row['nazev'];
        $zbozi[$i]['dodavatel'] = $row['dodavatel'];
        $zbozi[$i]['expirace'] = $row['expirace'];
        $zbozi[$i]['mj'] = $row['mj'];
        $row = $stmt->fetch();
    }

    for($i = 0; $i < count($zbozi); $i++){
        $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                FROM itu_zbozi
                INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                AND id_uzivatele = (SELECT id_uzivatele from itu_uzivatel where username='" . $_SESSION['user'] . "')
                AND smer =0), 0) as mnozstvi";

        $stmt = $pdo->prepare($sql);
        $stmt->execute();
        $row = $stmt->fetch();
        $pocet_prichozich = $row['mnozstvi'];

        $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                FROM itu_zbozi
                INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                AND id_uzivatele = (SELECT id_uzivatele from itu_uzivatel where username='" . $_SESSION['user'] . "')
                AND smer =1), 0) as mnozstvi";

        $stmt = $pdo->prepare($sql);
        $stmt->execute();
        $row = $stmt->fetch();
        $pocet_odchozich = $row['mnozstvi'];




        $zbozi[$i]['mnozstvi'] = $pocet_prichozich - $pocet_odchozich;

    }


    return $zbozi;
}













































/* Vlozi do tabulky 'itu_uzivatel' noveho uzivatele podle predanych parametru funkce.
 */
function create_new_user_in_table_itu_uzivatel($username, $password){

    $conn = null;
    connect($conn);

    //Zde by jeste mela byt kontrola, zda uz uzivatel se stejnym username neexistuje

    $sql = "INSERT INTO itu_uzivatel(username, user_password) VALUES (?, ?)";
    $stmt = $conn->prepare($sql);
    
    
    $stmt->bind_param("ss", $username, $password);
    $stmt->execute();
    
    $stmt->close();
    disconnect($conn);
}

/* Vraci true pokud uzivatel zadal spravne prihlasovaci udaje
 * Jako parametry bere username a password co zadal uzivatel pri prohlasovani.
 */
function user_logged_in($username, $password){

    $conn = null;    
    connect($conn);
    
    $sql = "SELECT user_password FROM itu_uzivatel WHERE username='$username'";

    $result = $conn->query($sql);
    
    $row = "";
    if (!empty($result) && $result->num_rows > 0) {
        $row = $result->fetch_assoc();
        if($row['user_password'] == $password){
            return true;
        }
    }

    disconnect($conn);
    return false;
}

?>



