<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

// Tøída poskytuje metody pro správu èlánkù v redakèním systému
class SpravcePrihlaseni
{

    // Vrátí seznam v¹ech zbo¾í v databázi
    public function zkontrolujUzivatele($username, $pwd)
    {

        $sql = "SELECT * FROM itu_uzivatel WHERE username=? AND user_password=?";

        $row = Db::dotazJeden($sql, array($username, $pwd));
        if($row){
        	return $row['id_uzivatele'];
        }else{
        	return -1;
        }
    }

} 