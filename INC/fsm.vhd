-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (WAITING,S1,S2,S3,S4,S5,S6A,S6B,S7A,S7B,S8A,S8B,
   S9A,S9B,S10A,S10B,OK,CHYBA,BAD,ENDING);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= WAITING;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
	when WAITING =>
	next_state <= WAITING;
	  if(KEY(3)='1') then
		next_state <= S1;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	   elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	
	when S1 =>
	next_state <=S1;
	  if(KEY(5)='1') then
		next_state <= S2;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	
	
	when S2 =>
	next_state <=S2;
	  if(KEY(1)='1') then
		next_state <= S3;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	
	
	when S3 =>
	next_state <=S3;
	  if(KEY(7)='1') then
		next_state <= S4;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	
	
	when S4 =>
	next_state <=S4;
	  if(KEY(9)='1') then
		next_state <= S5;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	  
	when S5 =>
	next_state <=S5;
	  if(KEY(7)='1') then
		next_state <= S6A;
	  elsif(KEY(8)='1') then
		next_state <= S6B;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	  
	when S6A =>
	next_state <=S6A;
	  if(KEY(5)='1') then
		next_state <= S7A;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;  
	  
	when S6B =>
	next_state <=S6B;
	  if(KEY(7)='1') then
		next_state <= S7B;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	  
	when S7A =>
	next_state <=S7A;
	  if(KEY(4)='1') then
		next_state <= S8A;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;   
	  
	when S7B =>
	next_state <=S7B;
	  if(KEY(7)='1') then
		next_state <= S8B;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	
	when S8A =>
	next_state <=S8A;
	  if(KEY(9)='1') then
		next_state <= S9A;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;   
	  
	when S8B =>
	next_state <=S8B;
	  if(KEY(4)='1') then
		next_state <= S9B;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if;
	  
	when S9A =>
	next_state <=S9A;
	  if(KEY(6)='1') then
		next_state <= S10A;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if; 
	  
	when S9B =>
	next_state <=S9B;
	  if(KEY(8)='1') then
		next_state <= S10B;
      elsif(KEY(15) = '1') then
        next_state <= BAD; 
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <=CHYBA;
	  end if; 
	  
	when S10A  =>
	next_state <=S10A;
	  if(KEY(15)='1') then
		next_state <= OK;
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <= CHYBA;
	  end if;
	  
	when  S10B =>
	next_state <=S10B;
	  if(KEY(15)='1') then
		next_state <= OK;
	  elsif(KEY(14 downto 0) /="000000000000000") then
		next_state <= CHYBA;
	  end if;
		
	when OK =>
	next_state <=OK;
	  if(CNT_OF='1') then
		next_state <= ENDING;
	  end if;
	
	when CHYBA =>
	next_state <=CHYBA;
	  if(KEY(15) ='1') then
		next_state <= BAD;
	  end if;
	  
	when BAD =>
	next_state <=BAD;
	  if(CNT_OF='1') then
		next_state <= ENDING;
	  end if;
	  
	when ENDING =>
	next_state <=ENDING;
	  if(KEY(15)='1') then
		next_state <= WAITING;
	  end if;
	
	when others =>
	  next_state <= WAITING;
	  
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WAITING  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S1  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S2  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S3  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S4  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S5  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S6A  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S6B =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S7A =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S7B =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S8A  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S8B =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S9A =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S9B =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S10A  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
		
	when S10B  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CHYBA=>
	   if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
			end if;
	   if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
		
      
      end if;   
	
   
   when OK=>
	   FSM_MX_MEM	 <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	  
	when BAD=>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	  
	when ENDING=>
	  if (KEY(15)='1') then
		FSM_LCD_CLR <='1';
	  end if;
  
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

