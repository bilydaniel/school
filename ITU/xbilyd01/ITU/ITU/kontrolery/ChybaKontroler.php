<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

class ChybaKontroler extends Kontroler
{
    public function zpracuj($parametry)
    {
		// Hlavička požadavku
		header("HTTP/1.0 404 Not Found");
		// Nastavení šablony
		$this->pohled[0] = 'chyba';
    }
}
?>