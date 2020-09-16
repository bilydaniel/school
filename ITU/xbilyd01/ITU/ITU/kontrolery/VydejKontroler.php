<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

class VydejKontroler extends Kontroler
{
	// Instance controlleru
	protected $kontroler;
	
    public $parametry = null;

    // Naparsování URL adresy a vytvoření příslušného controlleru
    public function zpracuj($parametry)
    {
        $this->parametry = $parametry;

        $spravceNastaveni = new SpravceNastaveni();
        $spravceZbozi = new SpravceZbozi();

        $this->data['zmeny_ulozeny'] = 0;
	    if(isset($_POST) && isset($_POST['save'])){//Ulozit 
	        $spravceZbozi->ulozZbozi($_SESSION['id'], $_POST);
	        $_SESSION['zmeny_ulozeny'] = 1;
			//$this->presmeruj('?akce=prijem');
	    }

	    if(isset($_SESSION['zmeny_ulozeny']) && $_SESSION['zmeny_ulozeny'] == 1){
	    	$this->data['zmeny_ulozeny'] = 1;
	    	unset($_SESSION['zmeny_ulozeny']);
	    	
	    }
        $nastaveni = $spravceNastaveni->vratNastaveni($_SESSION['id']);
        $this->data['nastaveni'] = $nastaveni;

        $nazvy = $spravceZbozi->vratVsechnyNazvy($parametry['id']);
        $this->data['nazvy'] = $nazvy;

        $dodavatele = $spravceZbozi->vratVsechnyDodavatele($parametry['id']);
        $this->data['dodavatele'] = $dodavatele;

        $mj = $spravceZbozi->vratVsechnyMJ($parametry['id']);
        $this->data['mj'] = $mj;
        
		$this->pohled[0] = 'vydej';
		
    }

   

}
?>