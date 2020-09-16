<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

class NastaveniKontroler extends Kontroler
{
	// Instance controlleru
	protected $kontroler;
	
    public function zpracuj($parametry)
    {

        $spravceNastaveni = new SpravceNastaveni();

        $this->data['zmeny_ulozeny'] = 0;
	    if(isset($_POST) && isset($_POST['save'])){//Ulozit nastaveni
	        $spravceNastaveni->ulozNastaveni($_POST, $_SESSION['id']);
	        $this->data['zmeny_ulozeny'] = 1;
	    }

        $nastaveni = $spravceNastaveni->vratNastaveni($_SESSION['id']);
        $this->data['nastaveni'] = $nastaveni;
        $this->data['checked'] = $this->vratPoleCheckedSlov($nastaveni);
		$this->pohled[$this->pohledy_index++] = 'nastaveni';
		
    }

    private function vratPoleCheckedSlov($nastaveni){
    	$checked = array();
    	if($nastaveni['nazev']){
    		$checked['nazev'] = "checked";
    	}else{
    		$checked['nazev'] = "";
    	}
    	if($nastaveni['dodavatel']){
    		$checked['dodavatel'] = "checked";
    	}else{
    		$checked['dodavatel'] = "";
    	}
    	if($nastaveni['cena_nakupni']){
    		$checked['cena_nakupni'] = "checked";
    	}else{
    		$checked['cena_nakupni'] = "";
    	}
    	if($nastaveni['cena_prodejni']){
    		$checked['cena_prodejni'] = "checked";
    	}else{
    		$checked['cena_prodejni'] = "";
    	}
    	if($nastaveni['mj']){
    		$checked['mj'] = "checked";
    	}else{
    		$checked['mj'] = "";
    	}
    	if($nastaveni['mnozstvi']){
    		$checked['mnozstvi'] = "checked";
    	}else{
    		$checked['mnozstvi'] = "";
    	}
    	return $checked;
    }
   

}
?>