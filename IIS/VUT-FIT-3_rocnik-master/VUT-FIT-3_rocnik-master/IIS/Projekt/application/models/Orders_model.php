<?php

class Orders_model extends CI_Model
{
	function return_all_orders()
	{
		$query = $this->db->get('objednavka');
		$result = $query->result_array();
		return $result;
	}
	
	function get_orders($id)
	{
		$this->db->where('id_osoby',"$id");
		$query = $this->db->get('objednavka');
		$result = $query->result_array();
		return $result;
	}
	
	function get_order_items($id)
	{
		$this->db->where('id_objednavky',"$id");
		$query = $this->db->get('objednana_polozka');
		$result = $query->result_array();
		return $result;
	}

	function get_driver_orders($id)
	{
		$this->db->where('id_ridice',"$id");
		$this->db->where('stav',2);
		$query = $this->db->get('objednavka');
		$result = $query->result_array();
		return $result;
	}

	function get_operator_orders()
	{
		$this->db->where('stav',1);
		$query = $this->db->get('objednavka');
		$result = $query->result_array();
		return $result;
	}

	function order_exists($id)
	{
		$this->db->where('id_objednavky',"$id");	
		$query = $this->db->get('objednavka');
		if($query->num_rows() > 0)  
			return True;  
		return False;
	}

	function make_order($data)
	{
		$this->db->insert('objednavka',$data);
		$insert_id = $this->db->insert_id();

		return  $insert_id;
	}
	
	function get_items()
	{
		$query = $this->db->get('polozka');
		$result = $query->result_array();
		return $result;
	}

	function get_item($id)
	{
		$this->db->where('id_polozky',"$id");	
		$query = $this->db->get('polozka');
		$result = $query->result_array();
		return $result;
	}

	function item_to_order($data)
	{
		$this->db->insert('objednana_polozka',$data);
	}

	function edit_order_state($id,$num)
	{
		$this->db->where('id_objednavky', $id);
		$this->db->update('objednavka', array('stav' =>  $num));
	}

	function edit_order_driver($id,$driver)
	{
		$this->db->where('id_objednavky', $id);
		$this->db->update('objednavka', array('id_ridice' =>  $driver));
	}

	function remove_order($order_id)
	{
		$this->db->delete('objednavka', array('id_objednavky' => $order_id)); 
	}

	function order_price($order_id,$price)
	{
		$this->db->where('id_objednavky', $order_id);
		$this->db->update('objednavka', array('cena' =>  $price));
	}
	
	function edit_order_time($id,$time)
	{
		$this->db->where('id_objednavky', $id);
		$this->db->update('objednavka', array('cas' =>  $time));
	}

	function edit_amount($id,$amount)
	{
		$this->db->where('id_polozky', $id);
		$this->db->update('polozka', array('zbyva' =>  $amount));
	}

	}


	?>