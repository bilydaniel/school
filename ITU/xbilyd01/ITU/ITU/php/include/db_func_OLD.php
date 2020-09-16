<?php

include_once 'config.php';

function disconnect(&$conn) {
    $conn->close();
}
function connect(&$conn){
    global $servername;                      
    global $username;
    global $password;
    global $databasename;

    // Create connection
    $conn = new mysqli($servername, $username, $password);
    
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
    
    $sql = "USE `" . $databasename . "`";
    
    if ($conn->query($sql) === TRUE) {
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
    
    $conn->query("SET CHARACTER SET utf8");
    
}

/* Vytvori tabulku uzivatelu (itu_users) se sloupci: id_user, username, user_password
 * POZN: TUTO FUNKCI NIKDE V KODU NEVOLAT, JE VOLANA SAMA Z JINE FUNKCE!
 */
function create_table_itu_users_if_not_exist(){

    $conn = null;    
    connect($conn);
    
    $sql = 'CREATE TABLE IF NOT EXISTS `itu_users` (
        `id_user` int(11) NOT NULL AUTO_INCREMENT,
        `username` varchar(100) COLLATE latin2_czech_cs NOT NULL,
        `user_password` varchar(100) COLLATE latin2_czech_cs NOT NULL,
        PRIMARY KEY (`id_user`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=1 ;';

    $result = $conn->query($sql);
    
    disconnect($conn);
}

/* Vytvori tabulku zbozi (itu_zbozi) 
 * POZN: TUTO FUNKCI NIKDE V KODU NEVOLAT, JE VOLANA SAMA Z JINE FUNKCE!
 */
function create_table_itu_zbozi_if_not_exist(){

    $conn = null;    
    connect($conn);
    
    $sql = 'CREATE TABLE IF NOT EXISTS `itu_zbozi` (
        `id_zbozi` int(11) NOT NULL AUTO_INCREMENT,
        `id_pohybu` int(11) NOT NULL,
        `nazev` varchar(200) COLLATE latin2_czech_cs DEFAULT NULL,
        `dodavatel` int(200) DEFAULT NULL,
        `expirace` date DEFAULT NULL,
        `mj` int(11) NOT NULL,
        `pocet` int(11) NOT NULL,
        PRIMARY KEY (`id_zbozi`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=1 ;';

    $result = $conn->query($sql);
    
    disconnect($conn);
}

/* Vytvori tabulku pohybu zbozi (itu_pohyb_zbozi) 
 * POZN: TUTO FUNKCI NIKDE V KODU NEVOLAT, JE VOLANA SAMA Z JINE FUNKCE!
 */
function create_table_itu_pohyb_zbozi_if_not_exist(){

    $conn = null;    
    connect($conn);
    
    $sql = 'CREATE TABLE IF NOT EXISTS `itu_pohyb_zbozi` (
        `id_pohybu` int(11) NOT NULL AUTO_INCREMENT,
        `id_uzivatele` int(11) NOT NULL,
        `datum` date NOT NULL,
        `poznamka` varchar(500) COLLATE latin2_czech_cs DEFAULT NULL,
        `smer` int(11) NOT NULL,
        PRIMARY KEY (`id_dodavky`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=1 ;';

    $result = $conn->query($sql);
    
    disconnect($conn);
}

/* Vytvori vsechny potrebne tabulky
 * POZN: TUTO FUNKCI NIKDE V KODU NEVOLAT, JE VOLANA PRI POKUS O PRIHLASENI A ROVNEZ PRI REGISTRACI UZIVATELE
 */
function create_all_tables(){
    create_table_itu_users_if_not_exist();
    create_table_itu_zbozi_if_not_exist();
    create_table_itu_pohyb_zbozi_if_not_exist();
}

/* Vlozi do tabulky 'itu_users' noveho uzivatele podle predanych parametru funkce.
 * Tato funkce v sobe vola funkci 'create_table_itu_users_if_not_exist()' pro 
 * zajisteni, ze tabulka existuje (a pripadne ji i vytvori, pokud neexistuje)
 */
function create_new_user_in_table_itu_users($username, $password){

    //Zajistime ze tabulka itu_users existuje
    create_all_tables();

    $conn = null;
    connect($conn);

    //Zde by jeste mela byt kontrola, zda uz uzivatel se stejnym username neexistuje

    $sql = "INSERT INTO itu_users (username, user_password) VALUES (?, ?)";
    $stmt = $conn->prepare($sql);
    
    
    $stmt->bind_param("ss", $username, $password);
    $stmt->execute();
    
    $stmt->close();
    disconnect($conn);
}

/* Vraci true pokud uzivatel zadal spravne prihlasovaci udaje
 * Jako parametry bere username a password co zadal uzivatel pri prohlasovani.
 * Tato funkce v sobe vola funkci 'create_table_itu_users_if_not_exist()' pro 
 * zajisteni, ze tabulka uzivatelu existuje (a pripadne ji i vytvori, pokud neexistuje)
 */
function user_logged_in($username, $password){

    //Zajistime ze tabulka itu_users existuje
    create_all_tables();

    $conn = null;    
    connect($conn);
    
    $sql = "SELECT user_password FROM itu_users WHERE username='$username'";

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


/* Vlozi do tabulky 'itu_zbozi' zbozi
 */
function add_goods($goods_array){

    $conn = null;
    connect($conn);

    $id_pohybu = 0;
    $nazev = 0;
    $dodavatel = 0;
    $expirace = 0;
    $mj = 0;
    $pocet = 0;

    for($i = 0; $i < ((count($goods_array)-1)/4); $i++){
        
    }

    $sql = "INSERT INTO itu_zbozi (id_pohybu, mj, pocet) VALUES (?, ?, ?)";
        $stmt = $conn->prepare($sql);
        
        
        $stmt->bind_param("sss", $id_pohybu, $mj, $pocet);
        $stmt->execute();
        
        $stmt->close();
    disconnect($conn);
}


?>



