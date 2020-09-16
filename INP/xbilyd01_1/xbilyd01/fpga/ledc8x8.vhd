library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu.
	SMCLK : in std_logic;--vstupni hodinovy signal
	RESET : in std_logic;--asynchronni inicializaci hodnot
	
	ROW : out std_logic_vector(7 downto 0);--radky displeje
	LED : out std_logic_vector(7 downto 0) --sloupce displeje
);
end ledc8x8;

architecture main of ledc8x8 is

    -- Sem doplnte definice vnitrnich signalu.
	
	signal clock_enable : std_logic := '0';--povolovaci signal pro upravene smclk
	
	signal counter1 : std_logic_vector(11 downto 0) := (others => '0');--signal pro vystup citace(deleni smclk),8 bitu = 256, vynulovan
	signal counter2 : std_logic_vector(20 downto 0) := (others => '0');--signal pro rozdeleni jedne vteriny na ctyri casti
	signal state : std_logic_vector(1 downto 0) := (others => '0');--promena pro ctyri stavy

	
	signal row_order : std_logic_vector(7 downto 0) :=(others => '0');--znaceni vyberu radku
	signal led_active : std_logic_vector(7 downto 0) :=(others => '1');
begin

    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.

    -- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
    -- v souboru ledc8x8.ucf.
	
	
	--citac pro deleni smclk/256/8
	citac1: process(SMCLK, RESET)
	begin
		if RESET = '1' then
			counter1 <= (others => '0');
		elsif rising_edge(SMCLK) then
			counter1 <= counter1 + 1;
		if counter1 = "111000010000" then
			clock_enable <= '1';
			counter1 <= (others => '0');
		else
			clock_enable <=  '0';
		end if;
		end if;
	end process citac1;
	
	--citac pro deleni smclk na ctyri casti(J nic P nic => 4 periody)
	citac2: process(SMCLK, RESET)
	begin
		if RESET = '1' then
			counter2 <= (others => '0');
		elsif rising_edge(SMCLK) then
			counter2 <= counter2 + 1;
		if counter2 = "111000010000000000000" then
			state <= state + 1;--nemusime nulovat, pretece
			counter2 <= (others => '0');	
		end if;
		end if;
	end process citac2;
	
	
	
	
	row_shift: process (SMCLK, RESET, clock_enable)
	 begin
		if RESET = '1' then
			row_order <= "10000000";
		elsif rising_edge(smclk) then
			if clock_enable = '1' then
				row_order <= row_order(0) & row_order(7 downto 1);--shiftovani(posun) 
			end if;
		end if;
	 end process row_shift;
	 
	 display:process (row_order)
	 begin
		if state = "00" then
			case row_order is
			when "10000000" => led_active <= "00011111";
			when "01000000" => led_active <= "01101111";
			when "00100000" => led_active <= "01101111";
			when "00010000" => led_active <= "01101111";
			when "00001000" => led_active <= "01101111";
			when "00000100" => led_active <= "00011111";
			when "00000010" => led_active <= "11111111";
			when "00000001" => led_active <= "11111111";
			when others => led_active <= "11111111";
		end case;
		
		elsif state = "01" or state = "11" then
			case row_order is
			when "10000000" => led_active <= "11111111";
			when "01000000" => led_active <= "11111111";
			when "00100000" => led_active <= "11111111";
			when "00010000" => led_active <= "11111111";
			when "00001000" => led_active <= "11111111";
			when "00000100" => led_active <= "11111111";
			when "00000010" => led_active <= "11111111";
			when "00000001" => led_active <= "11111111";
			when others => led_active <= "11111111";
		end case;
		
		elsif state = "10" then
			case row_order is
			when "10000000" => led_active <= "00011111";
			when "01000000" => led_active <= "01101111";
			when "00100000" => led_active <= "00011111";
			when "00010000" => led_active <= "01101111";
			when "00001000" => led_active <= "01101111";
			when "00000100" => led_active <= "00011111";
			when "00000010" => led_active <= "11111111";
			when "00000001" => led_active <= "11111111";
			when others => led_active <= "11111111";
		end case;
		end if;
		
	 end process display;

	--vystupni signaly 
	LED <= led_active;
	ROW <= row_order;
		

end main;
