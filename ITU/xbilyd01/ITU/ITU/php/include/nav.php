<?php

    $sklad="";
    $prijem="";
    $vydej="";
    $historie="";
    $sprava="";
    if(isset($_GET) && isset($_GET['akce'])){
        if($_GET['akce'] == "sklad")
            $sklad = "active";
        if($_GET['akce'] == "prijem")
            $prijem = "active";
        if($_GET['akce'] == "vydej")
            $vydej = "active";
        if($_GET['akce'] == "historie")
            $historie = "active";
        if($_GET['akce'] == "spravovat_zbozi")
            $sprava = "active";
    }else{
        $sklad = "active";
    }
?>
<nav>
    <a href="?akce=sklad" id="<?php echo $sklad;?>">Sklad</a>
    <a href="?akce=prijem" id="<?php echo $prijem;?>">Pøíjem zbo¾í</a>
    <a href="?akce=vydej" id="<?php echo $vydej;?>">Výdej zbo¾í</a>
    <a href="?akce=historie" id="<?php echo $historie;?>">Historie</a>
    <a href="?akce=spravovat_zbozi" id="<?php echo $sprava;?>">Spravovat zbo¾í</a>
</nav>