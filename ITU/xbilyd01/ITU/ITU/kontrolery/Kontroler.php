<!--Autori: xmarek69, xgajdo24, xbilyd01 -->
<?php
//Výchozí kontroler
abstract class Kontroler
{

	// Pole, jehož indexy jsou poté viditelné v šabloně jako běžné proměnné
    protected $data = array();
	// Název šablony bez přípony
    protected $pohled = array();
	// Hlavička HTML stránky
	protected $hlavicka = array('titulek' => '', 'klicova_slova' => '', 'popis' => '');
	
	protected $pohledy_index = 0;

	public $id = 0;

	// Vyrenderuje pohled
    public function vypisPohled()
    {
        if ($this->pohled)
        {
            extract($this->data);
            for($i = 0; $i < count($this->pohled); $i++)
            	require("pohledy/" . $this->pohled[$i] . ".phtml");
        }
    }
	
	// Přesměruje na dané URL
	public function presmeruj($url)
	{
		header("Location: $url");
		header("Connection: close");
        exit;
	}

	// Hlavní metoda controlleru
    abstract function zpracuj($parametry);

}

?>