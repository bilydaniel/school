<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

// Tøída poskytuje metody pro správu èlánkù v redakèním systému
class SpravceZbozi
{

    // Vrátí seznam v¹ech zbo¾í v databázi
    public function vratZbozi($id){

        $sql = "select id_zbozi, nazev, dodavatel, cena_nakupni, cena_prodejni, mj from (select id_zbozi, nazev, dodavatel, cena_nakupni, cena_prodejni, mj from (select * from itu_zbozi order by id_zbozi desc) as s inner join itu_pohyb_zbozi on s.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=? order by id_zbozi desc) as sestupne group by nazev";
        $row = Db::dotazVsechny($sql, array($id));


        for ($i = 0; $i < count($row) ; $i++) {
            $zbozi[$i]['id_zbozi'] = $row[$i]['id_zbozi'];
            $zbozi[$i]['nazev'] = $row[$i]['nazev'];
            $zbozi[$i]['dodavatel'] = $row[$i]['dodavatel'];
            $zbozi[$i]['cena_nakupni'] = $row[$i]['cena_nakupni'];
            $zbozi[$i]['cena_prodejni'] = $row[$i]['cena_prodejni'];
            $zbozi[$i]['mj'] = $row[$i]['mj'];
        }

        for($i = 0; $i < count($zbozi); $i++){
            $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                    FROM itu_zbozi
                    INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                    WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                    AND id_uzivatele = ?
                    AND smer =0), 0) as mnozstvi";

            $row = Db::dotazJeden($sql, array($id));
            $pocet_prichozich = $row['mnozstvi'];
            $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                    FROM itu_zbozi
                    INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                    WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                    AND id_uzivatele = ?
                    AND smer =1), 0) as mnozstvi";

            $row = Db::dotazJeden($sql, array($id));
            $pocet_odchozich = $row['mnozstvi'];

            $zbozi[$i]['mnozstvi'] = $pocet_prichozich - $pocet_odchozich;

        }


        return $zbozi;
    }

    // Vrátí seznam v¹ech zbo¾í v databázi obsahující zadanou frázi
    public function vratSeznamZboziPodleFraze($id, $sql_podminka, $hodnoty){

        $sql = "select * from (select id_zbozi, nazev, dodavatel, cena_nakupni, cena_prodejni, mj from (select id_zbozi, nazev, dodavatel, cena_nakupni, cena_prodejni, mj from (select * from itu_zbozi order by id_zbozi desc) as s inner join itu_pohyb_zbozi on s.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=? order by id_zbozi desc) as sestupne group by nazev) as tabulka where " . $sql_podminka;
        $row = Db::dotazVsechny($sql, array_merge(array($id), $hodnoty));

        if(!$row)
            return array();
        
        for ($i = 0; $i < count($row) ; $i++) {
            $zbozi[$i]['id_zbozi'] = $row[$i]['id_zbozi'];
            $zbozi[$i]['nazev'] = $row[$i]['nazev'];
            $zbozi[$i]['dodavatel'] = $row[$i]['dodavatel'];
            $zbozi[$i]['cena_nakupni'] = $row[$i]['cena_nakupni'];
            $zbozi[$i]['cena_prodejni'] = $row[$i]['cena_prodejni'];
            $zbozi[$i]['mj'] = $row[$i]['mj'];
        }

        for($i = 0; $i < count($zbozi); $i++){
            $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                    FROM itu_zbozi
                    INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                    WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                    AND id_uzivatele = ?
                    AND smer =0), 0) as mnozstvi";

            $row = Db::dotazJeden($sql, array($id));
            $pocet_prichozich = $row['mnozstvi'];
            $sql = "SELECT IFNULL((SELECT sum(mnozstvi)
                    FROM itu_zbozi
                    INNER JOIN itu_pohyb_zbozi ON itu_zbozi.`id_pohybu` = itu_pohyb_zbozi.`id_pohybu`
                    WHERE nazev = '" . $zbozi[$i]['nazev'] . "'
                    AND id_uzivatele = ?
                    AND smer =1), 0) as mnozstvi";

            $row = Db::dotazJeden($sql, array($id));
            $pocet_odchozich = $row['mnozstvi'];

            $zbozi[$i]['mnozstvi'] = $pocet_prichozich - $pocet_odchozich;

        }


        return $zbozi;
    }

    function vratZboziPodlePodminkyAHodnot($id, $sql_podminka, $hodnoty){        
        $sql = "select * from (select * from (select id_zbozi, nazev, dodavatel, cena_nakupni, cena_prodejni, mj, mnozstvi from (select * from itu_zbozi order by id_zbozi desc) as s inner join itu_pohyb_zbozi on s.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=? order by id_zbozi desc) as sestupne group by nazev) as tabulka where " . $sql_podminka;
        $row = Db::dotazVsechny($sql, array_merge(array($id), $hodnoty));

        $zbozi = array();

        if(!$row){
            return $zbozi;
        }
        $zbozi['dodavatel'] = $row[0]['dodavatel'];
        $zbozi['cena_nakupni'] = $row[0]['cena_nakupni'];
        $zbozi['cena_prodejni'] = $row[0]['cena_prodejni'];
        $zbozi['mj'] = $row[0]['mj'];
        $zbozi['mnozstvi'] = $row[0]['mnozstvi'];

        return $zbozi;

    }

    // Ulo¾í zbo¾í do databáze
    public function ulozZbozi($id, $zbozi_array){

        $datum = $zbozi_array['datum_pohybu'];
        $poznamka = $zbozi_array['poznamka'];
        $smer = $zbozi_array['smer'];

        $sql = "INSERT INTO itu_pohyb_zbozi (id_uzivatele, datum, poznamka, smer) VALUES (?, ?, ?, ?)";        
        $last_id_pohybu = Db::updateOrInsert($sql, array($id, $datum, $poznamka, $smer));

        $pocet_poli = $this->spocitejZaznamenavanePolozky($zbozi_array);

        for($i = 0; $i < ((count($zbozi_array)-4)/$pocet_poli); $i++){
            $this->pridejZbozi(array_slice($zbozi_array,$pocet_poli * $i,$pocet_poli), $i, $last_id_pohybu);
        }

    }

    // Ulo¾í zbo¾í do databáze
    public function pridejZbozi($zbozi_array, $i, $id_poh){

        $field_string = "";
        $exec_array = array($id_poh);
        $count = 1;
        if(isset($zbozi_array['nazev' . $i])){
            $exec_array[$count++] = $zbozi_array['nazev' . $i];
        }else{
            $exec_array[$count++] = 0;
        }
        if(isset($zbozi_array['dodavatel' . $i])){
            $exec_array[$count++] = $zbozi_array['dodavatel' . $i];
        }else{
            $exec_array[$count++] = 0;
        }
        if(isset($zbozi_array['cena_nakupni' . $i])){
            $exec_array[$count++] = $zbozi_array['cena_nakupni' . $i];
        }else{
            $exec_array[$count++] = 0;
        }
        if(isset($zbozi_array['cena_prodejni' . $i])){
            $exec_array[$count++] = $zbozi_array['cena_prodejni' . $i];
        }else{
            $exec_array[$count++] = 0;
        }
        if(isset($zbozi_array['mj' . $i])){
            $exec_array[$count++] = $zbozi_array['mj' . $i];
        }else{
            $exec_array[$count++] = 0;
        }
        if(isset($zbozi_array['mnozstvi' . $i])){
            $exec_array[$count++] = $zbozi_array['mnozstvi' . $i];
        }else{
            $exec_array[$count++] = 0;
        }
        echo "<br><br>";

        $sql = "INSERT INTO itu_zbozi (id_pohybu, nazev, dodavatel, cena_nakupni, cena_prodejni, mj, mnozstvi) VALUES (?, ?, ?, ?, ?, ?, ?)";
        Db::updateOrInsert($sql, $exec_array);
    }

    // Upraví zbo¾í v databáze
    public function editujZbozi($id, $zbozi_array){
        $pole_identifikatoru = array();
        $pocet_vlastnosti = 0;
        foreach($zbozi_array as $key => $value){
            if(strpos($key, "id_zbozi_") !==  false){
                $pole_identifikatoru[substr($key,strlen("id_zbozi_"))] = $value;
            }
            if(strpos($key, "zbozi_0_") !==  false){
                $pocet_vlastnosti++;
            }
        }
    
        $stary_nazev = "";
        $novy_nazev = "";
        $sql_params = "";
        $exec_array = array();
        for($i = 0; $i < count($pole_identifikatoru); $i++){
            foreach($zbozi_array as $key => $value){
                if((strpos($key, "zbozi_$i" . "_") !==  false) && (strpos($key, "nazev") !==  false)){
                    $pole = array($id, $pole_identifikatoru[$i]);
                    $sql = "SELECT nazev FROM itu_zbozi inner join itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=? and id_zbozi=?";
                    $db_query = Db::dotazJeden($sql, $pole);
                    $stary_nazev = $db_query['nazev'];
                    $novy_nazev = $value;

                    //zmenime nazev
                    $sql = "UPDATE itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu SET nazev = ? WHERE id_uzivatele = ? AND nazev = ?";
                    Db::updateOrInsert($sql, array($value, $id, $stary_nazev));
                }
                if((strpos($key, "zbozi_$i" . "_") !==  false) && (strpos($key, "dodavatel") !==  false)){
                    $sql = "UPDATE itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu SET dodavatel = ? WHERE id_uzivatele = ? AND nazev = ?";
                    Db::updateOrInsert($sql, array($value, $id, $stary_nazev));
                }
                if((strpos($key, "zbozi_$i" . "_") !==  false) && (strpos($key, "cena_nakupni") !==  false)){
                    $sql = "UPDATE itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu SET cena_nakupni = ? WHERE id_uzivatele = ? AND nazev = ?";
                    Db::updateOrInsert($sql, array($value, $id, $stary_nazev));
                }
                if((strpos($key, "zbozi_$i" . "_") !==  false) && (strpos($key, "cena_prodejni") !==  false)){
                    $sql = "UPDATE itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu SET cena_prodejni = ? WHERE id_uzivatele = ? AND nazev = ?";
                    Db::updateOrInsert($sql, array($value, $id, $stary_nazev));
                }
                if((strpos($key, "zbozi_$i" . "_") !==  false) && (strpos($key, "mj") !==  false)){
                    $sql = "UPDATE itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu SET mj = ? WHERE id_uzivatele = ? AND nazev = ?";
                    Db::updateOrInsert($sql, array($value, $id, $stary_nazev));
                }

            }
            /*
            //Jeste vlozime id_uzivatele a id_zbozi
            array_push($exec_array, $id);
            array_push($exec_array, $pole_identifikatoru[$i]);

            $sql = "UPDATE itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu SET " 
                . $sql_params . " WHERE id_uzivatele = ? AND id_zbozi = ?";   
                echo "$sql<br>";
                print_r($exec_array);
            Db::updateOrInsert($sql, $exec_array);
            
            $novy_nazev = $exec_array[0];
            $exec_array = array();
            $sql_params = "";
            

            if(strlen($stary_nazev) != 0){
                array_push($exec_array, $novy_nazev);
                array_push($exec_array, $id);
                array_push($exec_array, $stary_nazev);
                $sql = "UPDATE itu_zbozi INNER JOIN itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu SET nazev = ? WHERE id_uzivatele = ? AND nazev = ?";
                Db::updateOrInsert($sql, $exec_array);
                echo "sql_:<br>$sql<br><br>";
                $exec_array = array();
            }*/
        }

    }

    private function vratPocetPolozekBezID($id_uzivatele, $id_vynechane_polozky){

        //ziskame nazev zbozi z id
        $sql = "SELECT id_zbozi, nazev FROM itu_zbozi inner join itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=? and id_zbozi=?";   
        $db_query = Db::dotazJeden($sql, array($id_uzivatele, $id_vynechane_polozky));
        $nazev = $db_query['nazev'];
        
        $sql = "SELECT id_zbozi, nazev, mnozstvi, smer FROM itu_zbozi inner join itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where nazev=?";   
        $db_query = Db::dotazVsechny($sql, array($nazev));

        $pocet = 0;
        for($i = 0; $i < count($db_query); $i++){
            if($db_query[$i]['id_zbozi'] != $id_vynechane_polozky){
                if($db_query[$i]['smer'] == 0){
                    $pocet += $db_query[$i]['mnozstvi'];
                }else{
                    $pocet -= $db_query[$i]['mnozstvi'];
                }
            }
        }
        return $pocet;

    }
    private function spocitejZaznamenavanePolozky($zbozi_array){
        $pocet = 0;
        if(isset($zbozi_array['nazev0']))
            $pocet++;
        if(isset($zbozi_array['dodavatel0']))
            $pocet++;
        if(isset($zbozi_array['cena_nakupni0']))
            $pocet++;
        if(isset($zbozi_array['cena_prodejni0']))
            $pocet++;
        if(isset($zbozi_array['mj0']))
            $pocet++;
        if(isset($zbozi_array['mnozstvi0']))
            $pocet++;
        return $pocet;
    }

    // vrátí seznam v¹ech slov z databáze (dle id_uzivatele)
    public function vratVsechnaSlova($id){
        $slova = array();
        $index = 0;
        $sql = "SELECT nazev, dodavatel, mj FROM itu_zbozi inner join itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=?";
        $databaze = Db::dotazVsechny($sql, array($id));
        foreach ($databaze as $poradi_zaznamu => $pole_hodnot) {
            foreach ($pole_hodnot as $key => $value) {
                if(!in_array($value, $slova)){
                    $slova[$index++] = $value;
                }
            }
        }
        sort($slova);
        return $slova;
    }  

    // vrátí seznam v¹ech nazvu zbo¾í z databáze (dle id_uzivatele)
    public function vratVsechnyNazvy($id){
        $slova = array();
        $index = 0;
        $sql = "SELECT nazev FROM itu_zbozi inner join itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=?";
        $databaze = Db::dotazVsechny($sql, array($id));
        for($i = 0; $i < count($databaze); $i++){
            if(!in_array($databaze[$i]['nazev'], $slova)){
                $slova[$index++] = $databaze[$i]['nazev'];
            }
        }
        sort($slova);
        return $slova;
    }  

    // vrátí seznam v¹ech dodavatelu z databáze (dle id_uzivatele)
    public function vratVsechnyDodavatele($id){
        $slova = array();
        $index = 0;
        $sql = "SELECT dodavatel FROM itu_zbozi inner join itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=?";
        $databaze = Db::dotazVsechny($sql, array($id));
        for($i = 0; $i < count($databaze); $i++){
            if(!in_array($databaze[$i]['dodavatel'], $slova)){
                $slova[$index++] = $databaze[$i]['dodavatel'];
            }
        }
        sort($slova);
        return $slova;
    }    

    // vrátí seznam v¹ech mìrných jednotek z databáze (dle id_uzivatele)
    public function vratVsechnyMJ($id){
        $slova = array();
        $index = 0;
        $sql = "SELECT mj FROM itu_zbozi inner join itu_pohyb_zbozi on itu_zbozi.id_pohybu=itu_pohyb_zbozi.id_pohybu where id_uzivatele=?";
        $databaze = Db::dotazVsechny($sql, array($id));
        for($i = 0; $i < count($databaze); $i++){
            if(!in_array($databaze[$i]['mj'], $slova)){
                $slova[$index++] = $databaze[$i]['mj'];
            }
        }
        sort($slova);
        return $slova;
    }    

    function vratDodavkuPodlePodminky($id, $sql_podminka, $hodnoty){
        $dodavky = array();
        $sql = "SELECT id_pohybu FROM itu_pohyb_zbozi where id_uzivatele=?";
        $id_relevantnich_pohybu = Db::dotazVsechny($sql, array($id));
        for($i = 0; $i < count($id_relevantnich_pohybu); $i++){
            $dodavky[$i] = array();
            $sql = "SELECT * FROM itu_pohyb_zbozi as pohyb inner join itu_zbozi on pohyb.id_pohybu=itu_zbozi.id_pohybu where pohyb.id_pohybu=?";
            $zbozi_v_pohybu = Db::dotazVsechny($sql, array($id_relevantnich_pohybu[$i]['id_pohybu']));

            $dodavky[$i]['datum'] = $zbozi_v_pohybu[0]['datum'];
            $dodavky[$i]['smer'] = $zbozi_v_pohybu[0]['smer']==0? "Pøíjem" : "Výdej";
            $dodavky[$i]['poznamka'] = $zbozi_v_pohybu[0]['poznamka']==""? "®ádná poznámka k dodávce" : $zbozi_v_pohybu[0]['poznamka'];

            for($j = 0; $j < count($zbozi_v_pohybu); $j++){
                $dodavky[$i][$j]['nazev'] = $zbozi_v_pohybu[$j]['nazev'];
                $dodavky[$i][$j]['dodavatel'] = $zbozi_v_pohybu[$j]['dodavatel'];
                $dodavky[$i][$j]['cena_nakupni'] = $zbozi_v_pohybu[$j]['cena_nakupni'];
                $dodavky[$i][$j]['cena_prodejni'] = $zbozi_v_pohybu[$j]['cena_prodejni'];
                $dodavky[$i][$j]['mj'] = $zbozi_v_pohybu[$j]['mj'];
                $dodavky[$i][$j]['mnozstvi'] = $zbozi_v_pohybu[$j]['mnozstvi'];
            }
        }

        return $dodavky;
    }
    
} 