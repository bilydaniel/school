<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

// T��da poskytuje metody pro spr�vu �l�nk� v redak�n�m syst�mu
class SpravcePrihlaseni
{

    // Vr�t� seznam v�ech zbo�� v datab�zi
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