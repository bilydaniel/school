<?php

class Restaurants_model extends CI_Model
{
	function return_all_restaurants()
	{
		$query = $this->db->get('provozovna');
		$result = $query->result_array();
		return $result;
	}

	function get_restaurant($id)
	{
		$this->db->where('id_provozovny',"$id");
		$query = $this->db->get('provozovna');
		$result = $query->result_array();
		return $result;
	}

	function restaurant_exists($id)
	{
		$this->db->where('id_provozovny',"$id");	
		$query = $this->db->get('provozovna');
		if($query->num_rows() > 0)  
			return True;  

		return False;
	}

	function get_menu($id)
	{
		$this->db->where('id_provozovny',"$id");
		$query = $this->db->get('polozka');
		$result = $query->result_array();
		return $result;
	}

	function get_types($id)
	{
		$this->db->where('id_polozky',"$id");
		$query = $this->db->get('druh_jidla');
		$result = $query->result_array();
		$ret_result = array();
		foreach ($result as $key => $value) 
		{
			array_push($ret_result,$value["druh_polozky"]);
		}
		

		return $ret_result;
	}

	function insert_item($data)
	{
		$this->db->insert('polozka',$data);
		$insert_id = $this->db->insert_id();

		return  $insert_id;
	}

	function insert_type($id,$type)
	{
		$data['id_polozky'] = $id;
		$data['druh_polozky'] = $type;

		$this->db->insert('druh_jidla',$data);
	}

	function delete_item($item_id)
	{
		$this->db->delete('polozka', array('id_polozky' => $item_id));
		$this->db->delete('objednana_polozka', array('id_polozky' => $item_id));
		$this->db->delete('druh_jidla', array('id_polozky' => $item_id));
	}

	function add_restaurant($data)
	{
		$this->db->insert('provozovna',$data);
	}

	function remove_restaurant($id)
	{
		$this->db->delete('provozovna', array('id_provozovny' => $id)); 
	}

	function edit_item($id,$data)
	{
		$this->db->where('id_polozky', $id);
		$this->db->update('polozka', $data);
	}

	function remove_types($id)
	{
		$this->db->delete('druh_jidla', array('id_polozky' => $id));
	}

	function edit_restaurant($id, $data)
	{
		$this->db->where('id_provozovny', $id);
		$this->db->update('provozovna', $data);
	}

	function get_price($item_id)
	{
		$this->db->where('id_polozky',"$item_id");
		$query = $this->db->get('polozka');
		$result = $query->result_array();
		return $result[0]['cena'];
	}
}


?>