-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
 
 --signaly pro pc
 signal pc_out: std_logic_vector(11 downto 0); --4KB
 signal pc_inc: std_logic;
 signal pc_dec: std_logic;
 
 --signaly pro cnt
 signal cnt_out:std_logic_vector(7 downto 0);
 signal cnt_inc: std_logic;
 signal cnt_dec: std_logic;
 
 --signaly pro ptr
 signal ptr_out: std_logic_vector(9 downto 0); --1KB
 signal ptr_inc: std_logic;
 signal ptr_dec: std_logic;
 
 --signaly pro mx
 signal mx_switch: std_logic_vector(1 downto 0);
 signal mx_in: std_logic_vector(7 downto 0);
 
 
--signaly pro FSM
type fsm_state is(
sidle, sfetch, sdecode, spointer_inc, spointer_dec, 
svalue_inc_read, svalue_inc_write, svalue_dec_read, svalue_dec_write,
sprint, sprint2, sload, scomment, scomment1, scomment2, scomment3, swhile, swhile2, swhile3,
swhile4, swhile_end, swhile_end2, swhile_end3, swhile_end4, swhile_end5,
swhile_end6, snumber, sletter, send, sothers
	


);

signal pstate: fsm_state;
signal nstate: fsm_state;
--TODO

 

begin

 -- zde dopiste vlastni VHDL kod dle blokoveho schema

 -- inspirujte se kodem procesoru ze cviceni
 
	--Program Counter
	process(CLK, RESET, pc_inc, pc_dec,pc_out)
	begin
		if(RESET = '1') then
			pc_out <= (others => '0');
		
		elsif(CLK'event and CLK = '1') then
			
			if(pc_inc = '1') then
				pc_out <= pc_out + 1;
			elsif(pc_dec = '1') then
				pc_out <= pc_out - 1;
			end if;
		end if;
	CODE_ADDR <= pc_out;
	end process;
 
	
	--counter zavorek
	process(CLK, RESET, cnt_inc, cnt_dec)
	begin
		if(RESET = '1') then
			cnt_out <=(others => '0');
			
		elsif(CLK'event and CLK = '1') then
			
			if(cnt_inc = '1') then
				cnt_out <= cnt_out + 1;
			elsif(cnt_dec = '1') then
				cnt_out <= cnt_out - 1;
			end if;
		end if;
	end process;
	
	
	--Data pointer
	process(CLK, RESET, ptr_inc, ptr_dec, ptr_out)
	begin
		if(RESET = '1') then
			ptr_out <=(others => '0');
			
		elsif(CLK'event and CLK = '1') then
			
			if(ptr_inc = '1') then
				ptr_out <= ptr_out + 1;
			elsif(ptr_dec = '1') then
				ptr_out <= ptr_out - 1;
			end if;
		end if;
	DATA_ADDR <= ptr_out;
	end process;
 
	
	--Multiplexor
	process(IN_DATA, mx_switch, DATA_RDATA)
	begin
		case(mx_switch) is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= mx_in;
			when "10" => DATA_WDATA <= DATA_RDATA - 1;
			when "11" => DATA_WDATA <= DATA_RDATA + 1;
			when others =>
		end case;
	end process;
	
	
	--fsm_pstate
	process(CLK, RESET)
	begin
		if(RESET = '1') then
			pstate <= sidle;
		elsif(CLK'event and CLK = '1') then
			if(EN = '1') then
				pstate <= nstate;
			end if;
		end if;
	end process;
	
	--fsm_nstate
	process(pstate, OUT_BUSY, IN_VLD, CODE_DATA, cnt_out, DATA_RDATA)
	begin
		
		-- vychozi hodnoty
		OUT_WE <=  '0';
		IN_REQ <= '0';
		CODE_EN <= '0'; 
		
		pc_inc <= '0';
		pc_dec <= '0';
		
		cnt_inc <= '0';
		cnt_dec <= '0';
		
		ptr_inc <= '0';
		ptr_dec <= '0';
	
		mx_switch <= "00";
		mx_in <= "00000000";
		
		DATA_EN <= '0';
		DATA_RDWR <= '0'; 
		

	--automat
		case pstate is 
			when sidle =>
				nstate <= sfetch;

			when sfetch =>
				CODE_EN <= '1';
				nstate <= sdecode;
				
			when sdecode =>
				
				case(CODE_DATA) is
					when x"3E" => nstate <= spointer_inc;
					when x"3C" => nstate <= spointer_dec;
					when x"2B" => nstate <= svalue_inc_read;
					when x"2D" => nstate <= svalue_dec_read;
					when x"5B" => nstate <= swhile;
					when x"5D" => nstate <= swhile_end;
					when x"2E" => nstate <= sprint;
					when x"2C" => nstate <= sload;
					when x"23" => nstate <= scomment;
					--cisla
					when x"30" => nstate <= snumber;
					when x"31" => nstate <= snumber;
					when x"32" => nstate <= snumber;
					when x"33" => nstate <= snumber;
					when x"34" => nstate <= snumber;
					when x"35" => nstate <= snumber;
					when x"36" => nstate <= snumber;
					when x"37" => nstate <= snumber;
					when x"38" => nstate <= snumber;
					when x"39" => nstate <= snumber;
					--pismena
					when x"41" => nstate <= sletter;
					when x"42" => nstate <= sletter;
					when x"43" => nstate <= sletter;
					when x"44" => nstate <= sletter;
					when x"45" => nstate <= sletter;
					when x"46" => nstate <= sletter;
					
					when x"00" => nstate <= send;
					when others => nstate <= sothers;
				end case;	
				
			when spointer_inc =>
				ptr_inc <= '1';
				
				pc_inc <= '1';
				nstate <= sfetch;
			
			when spointer_dec =>
				ptr_dec <= '1';
				
				pc_inc <= '1';
				nstate <= sfetch;
				
			--pro inkrementaci hodnoty ji v jednom stavu nacitam a v druhem ukladam	
			when svalue_inc_read =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
				nstate <= svalue_inc_write;
			
			when svalue_inc_write =>
				mx_switch <= "11";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				

				
				
				pc_inc <= '1';
				nstate <= sfetch;
				
			
			when svalue_dec_read =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
				nstate <= svalue_dec_write;
				
			when svalue_dec_write =>
				mx_switch <= "10";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				
				
				
				
				pc_inc <= '1';
				nstate <= sfetch;
				
			when swhile =>
				--nacteni aktualni hodnoty
				DATA_RDWR <= '1';
				DATA_EN <= '1';
                
				pc_inc <= '1';
				nstate <= swhile2;
				
			when swhile2 =>
				--kontrola aktualni hodnoty
				if(DATA_RDATA = "00000000") then
					cnt_inc <= '1';
					nstate <= swhile3;
				else
					nstate <= sfetch;
				end if;
				
			when swhile3 =>
				--kontrola poctu zavorek
				if(cnt_out = "00000000") then
					nstate <= sfetch;
				else 
					CODE_EN <= '1';
					nstate <= swhile4;
				end if;
			
			
			when swhile4 =>
				if(CODE_DATA = x"5B") then
					cnt_inc <= '1';
				elsif(CODE_DATA = x"5D") then
					cnt_dec <= '1';
				end if;
				
				pc_inc <= '1';
				nstate <= swhile3;
			
			when swhile_end =>
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				
				nstate <= swhile_end2;
			
			when swhile_end2 =>
				 if(DATA_RDATA = "00000000") then
					pc_inc <= '1';
					nstate <= sfetch;
				else
					nstate <= swhile_end3;
				end if;
			
			when swhile_end3 =>
				cnt_inc <= '1';--TODO
				pc_dec <= '1';
				
				nstate <= swhile_end4;
				
			when swhile_end4 =>
				if(cnt_out = "00000000") then
					nstate <= sfetch;
				else
					CODE_EN <= '1';
					nstate <= swhile_end5;
				end if;
				
			 when swhile_end5 =>
				if(CODE_DATA = X"5D") then
					cnt_inc <= '1';
				elsif(CODE_DATA = X"5B") then
					cnt_dec <= '1';
				end if;
					nstate <= swhile_end6;
				
			when swhile_end6 =>
				if(cnt_out = "00000000") then
					pc_inc <= '1';
				else
					pc_dec <= '1';
				end if;
					nstate <= swhile_end4;
			
			
			
			when sprint =>
				if(OUT_BUSY = '0') then
					--"vytazeni" dat z pameti, data ulozena v DATA_RDATA
					
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					nstate <= sprint2;
				else
				--cekani na display
					nstate <= sprint;
				end if;
				
				
			when sprint2 =>
				--vypis DATA_RDATA na vystup
				OUT_WE <= '1';
				OUT_DATA <= DATA_RDATA;
				
				pc_inc <= '1';
				nstate <= sfetch;
			
			when sload =>
				IN_REQ <= '1';
				
				if(IN_VLD = '0') then
					nstate <= sload;
				else				
					--IN_REQ se vynuluje na zacatku automatu
					mx_switch <= "00";	
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					
					pc_inc <= '1';
					nstate <= sfetch;
				end if;
				
				
			when scomment =>
				--posunuti se na dalsi znak
				pc_inc <= '1';
				nstate <= scomment1;
			
			
			when scomment1 =>
				--vycteni hodnoty
				CODE_EN <= '1';--TODO
				nstate <= scomment2;
			
			

			when scomment2 =>
				--kontrola jestli je znak #
				if (CODE_DATA = x"23") then
					pc_inc <= '1';
					nstate <= sfetch;
				else
					nstate <= scomment;
				end if;
				
			when snumber =>
				mx_switch <= "01";
				DATA_EN <= '1';
				--posun
				mx_in <= CODE_DATA(3 downto 0) & "0000";
				
				pc_inc <= '1';
				nstate <= sfetch;
			
			when sletter =>
				mx_switch <= "01";
				DATA_EN <= '1';
				--posun
				mx_in <= (CODE_DATA(3 downto 0) + std_logic_vector(conv_unsigned(9, mx_in'LENGTH )) & "0000");
			
				pc_inc <= '1';
				nstate <= sfetch;
			
			when send =>
				nstate <= send;
				
			when sothers =>
				pc_inc <= '1';
				nstate <= sfetch;
			
			
			when others =>
		end case;
	end process;
end behavioral;
 
