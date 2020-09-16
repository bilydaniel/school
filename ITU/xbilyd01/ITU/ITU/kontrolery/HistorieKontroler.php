<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

class HistorieKontroler extends Kontroler
{
	// Instance controlleru
	protected $kontroler;
	


    // Naparsování URL adresy a vytvoření příslušného controlleru
    public function zpracuj($parametry)
    {
        $spravceZbozi = new SpravceZbozi();

        $spravceNastaveni = new SpravceNastaveni();
        $nastaveni = $spravceNastaveni->vratNastaveni($parametry['id']);
        $this->data['nastaveni'] = $nastaveni;

        $dodavky = $spravceZbozi->vratDodavkuPodlePodminky($parametry['id'], "", "");
        $this->data['dodavky'] = $dodavky;
        
		$this->pohled[0] = 'historie';
		
    }

   

}
?>