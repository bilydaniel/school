<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

// Tøída poskytuje metody pro správu èlánkù v redakèním systému
class SpravceNastaveni
{

    // Vrátí seznam v¹ech zbo¾í v databázi
    public function vratNastaveni($id)
    {

        $sql = "SELECT * FROM itu_nastaveni WHERE id_uzivatele =?";

        $row = Db::dotazJeden($sql, array($id));

        $nastaveni['nazev'] = $row['nazev'];
        $nastaveni['dodavatel'] = $row['dodavatel'];
        $nastaveni['cena_nakupni'] = $row['cena_nakupni'];
        $nastaveni['cena_prodejni'] = $row['cena_prodejni'];
        $nastaveni['mj'] = $row['mj'];
        $nastaveni['mnozstvi'] = $row['mnozstvi'];

        return $nastaveni;
    }

    // Vrátí seznam v¹ech zbo¾í v databázi
    public function ulozNastaveni($nastaveni, $id)
    {

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
        if(isset($nastaveni['cena_nakupni'])){
            $exec_array[$count++] = 1;
        }else{
            $exec_array[$count++] = 0;
        }
        if(isset($nastaveni['cena_prodejni'])){
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

        $exec_array[$count] = $id;

        $sql = "UPDATE itu_nastaveni SET nazev = ?, dodavatel = ?, cena_nakupni = ?, cena_prodejni = ?, mj = ?, mnozstvi = ? WHERE id_uzivatele = ?";
        $row = Db::updateOrInsert($sql, $exec_array);

        return $nastaveni;
    }

} 