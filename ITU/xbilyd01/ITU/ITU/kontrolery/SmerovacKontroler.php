<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php

class SmerovacKontroler extends Kontroler
{
	// Instance controlleru
	protected $kontroler;


    // Naparsování URL adresy a vytvoøení pøíslu¹ného controlleru
    public function zpracuj($parametry)
    {

    	$this->show_success_login();
		
		$tridaKontroleru = $this->vrat_nazev_kontroleru($parametry);
		
    	if(isset($_SESSION) && isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == true){ // Je li uzivatel prihlasen
			$this->pohled[$this->pohledy_index++] = 'header'; //Zobrazime hlavicku

			if (file_exists('kontrolery/' . $tridaKontroleru . '.php')){// A zavolame prislusny kontroler
				$this->kontroler = new $tridaKontroleru();
        		$this->kontroler->data['id'] = $parametry['id'];
        		$this->kontroler->id = $parametry['id'];
			}else{
				$this->presmeruj('?akce=chyba');
			}
        }else{ // Jinak zavolame kontroler pro prihlaseni
            $this->kontroler = new LoginKontroler;
        }



		// Volání controlleru
        $this->kontroler->zpracuj($parametry);
		
		// Nastavení promìnných pro ¹ablonu
		/*$this->data['titulek'] = $this->kontroler->hlavicka['titulek'];
		$this->data['popis'] = $this->kontroler->hlavicka['popis'];
		$this->data['klicova_slova'] = $this->kontroler->hlavicka['klicova_slova'];*/
		
		// Nastavení hlavní ¹ablony
		$this->pohled[$this->pohledy_index++] = 'rozlozeni';
		
    }

    private function show_success_login(){    	
        //Pokud se uzivatel spravne vyplnil registracni formular, zobrazime hlasku o uspechu            
        if(isset($_SESSION) && isset($_SESSION['reg']) && $_SESSION['reg'] == true){
            unset($_SESSION['reg']);
            echo "<h2 class='message_for_user'>Úspì¹nì jste se zaregistroval, nyní se prosím pøihla¹te:</h2>";
        }
	}

	private function vrat_nazev_kontroleru($parametry){
		if(isset($parametry['akce'])){
			$pole = explode("_", $parametry['akce']);
			$kontroler = "";
			for($i = 0; $i < count($pole); $i++){
				$kontroler .= ucfirst($pole[$i]);
			}
			$kontroler .= 'Kontroler';
		}else{
			$kontroler = "SkladKontroler";
		}
		return $kontroler;
	}

}
?>