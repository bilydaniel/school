<?php

class Users_model extends CI_Model
{
	public function return_all_users()
	{
		$query = $this->db->get('osoba');
		$result = $query->result_array();
		return $result;
	}

	public function can_login($username, $password)
	{
			//TODO hash password
		$this->db->where('login',"$username");	
		$this->db->where('heslo',"$password");
		$query = $this->db->get('stravnik');
		if($query->num_rows() > 0)  
			return True;  

		return False;
		
	}

	public function user_exists($username)
	{
		$this->db->where('login',"$username");	
		$query = $this->db->get('stravnik');
		if($query->num_rows() > 0)  
			return True;  

		return False;
	}

	public function register_user($username, $password,$osoba_id)
	{
			//TODO hash password
		$data = array();
		$data['login'] = $username;
		$data['heslo'] = $password;
		$data['id_osoby'] = $osoba_id;
		$result = $this->db->insert('stravnik',$data);
		return $result;
	}

	public function create_osoba($data)
	{
		$result = $this->db->insert('osoba',$data);
		$insert_id = $this->db->insert_id();
		return $insert_id;
	}
	
	public function get_user_id($username)
	{
		$this->db->where('login', "$username");
		$query = $this->db->get('stravnik');
		$result = $query->result_array();
		if(!$result)
			return null;
		if(!$result[0])
			return null;
		return $result[0]['id_stravnika'];
	}

	public function change_username($id, $new_username)
	{
		$data = array('login' => "$new_username");
		$this->db->where('id_stravnika', $id);
		$this->db->update('stravnik', $data);
	}

	public function get_id_osoba($user_id)
	{
		$this->db->where('id_stravnika', "$user_id");
		$query = $this->db->get('stravnik');
		$result = $query->result_array();
		if(!$result)
			return null;
		if(!$result[0])
			return null;
		return $result[0]['id_osoby'];
	}

	public function get_osoba_data($osoba_id)
	{
		$this->db->where('id_osoby', "$osoba_id");
		$query = $this->db->get('osoba');
		$result = $query->result_array();
		if(!$result)
			return null;
		if(!$result[0])
			return null;
		return $result[0];
	}

	public function update_osoba($osoba_id,$update_data)
	{
		$this->db->where('id_osoby', $osoba_id);
		$this->db->update('osoba', $update_data);
	}

	public function update_stravnik($stravnik_id,$update_data)
	{
		$this->db->where('id_stravnika', $stravnik_id);
		$this->db->update('stravnik', $update_data);
	}

	public function update_password($user_id, $new_password)
	{
		$data = array('heslo' => "$new_password");
		$this->db->where('id_stravnika', $user_id);
		$this->db->update('stravnik', $data);
	}

	public function get_address($osoba_id)
	{
		$this->db->where('id_osoby', "$osoba_id");
		$query = $this->db->get('osoba');
		$result = $query->result_array();
		if(!$result)
			return null;
		if(!$result[0])
			return null;
		return $result[0]['adresa'];
	}

	public function get_phone($osoba_id)
	{
		$this->db->where('id_osoby', "$osoba_id");
		$query = $this->db->get('osoba');
		$result = $query->result_array();
		if(!$result)
			return null;
		if(!$result[0])
			return null;
		return $result[0]['telefon'];
	}

	public function get_role($user_id)
	{
		$this->db->where('id_stravnika', "$user_id");
		$query = $this->db->get('stravnik');
		$result = $query->result_array();
		if(!$result)
			return null;
		if(!$result[0])
			return null;
		return $result[0]['role'];
	}

	public function set_address($osoba_id,$address)
	{
		$data = array('adresa' => "$address");
		$this->db->where('id_osoby', $osoba_id);
		$this->db->update('osoba', $data);
	}

	public function set_phone($osoba_id,$phone)
	{
		$data = array('telefon' => "$phone");
		$this->db->where('id_osoby', $osoba_id);
		$this->db->update('osoba', $data);
	}

	public function return_all_drivers(){
		$this->db->where('role',"driver");	
		$query = $this->db->get('stravnik');
		$result = $query->result_array();
		return $result;
	}

	public function return_all_registrated(){
		$query = $this->db->get('stravnik');
		$result = $query->result_array();
		return $result;
	}

	public function remove_user($id)
	{
		$this->db->delete('stravnik', array('id_stravnika' => $id)); 
	}

	public function remove_osoba($id)
	{
		$this->db->delete('osoba', array('id_osoby' => $id)); 
	}
}


?>