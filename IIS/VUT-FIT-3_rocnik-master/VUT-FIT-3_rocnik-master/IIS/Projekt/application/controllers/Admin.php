<?php  
class Admin extends CI_Controller
{
	public function index()
	{

		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');
		
		if($this->session->userdata('role') !='admin')
			redirect(base_url() . 'index.php');
		$this->load->model("orders_model");
		$this->load->model("users_model");
		$data["logged_in_as"] = $this->session->userdata('username');
		$data["user_id"] = $this->session->userdata('id');

		$id_osoba = $this->users_model->get_id_osoba($this->session->userdata('id'));
		$data["user"] = $this->users_model->get_osoba_data($id_osoba);

		$tmp_users = $this->users_model->return_all_users();
		foreach ($tmp_users as $key => $value) 
		{
			$users[$value['id_osoby']]= $value;
		}
		$data["users"] = $users;
		$data["registered"] = $this->users_model->return_all_registrated();

		$this->load->view("admin.html",$data);
	}

	public function delete_user()
	{
		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');
		
		if($this->session->userdata('role') !='admin')
			redirect(base_url() . 'index.php');
		//TODO check session and data
		$this->load->model("users_model");
		$input = $this->input->get();
		$id_stravnika = $input['id_stravnika'];
		$id_osoby = $input['id_osoby'];

		$this->users_model->remove_user($id_stravnika);
		$this->users_model->remove_osoba($id_osoby);
		redirect(base_url().'index.php/Admin');			
	}

	public function edit_user()
	{
		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');
		
		if($this->session->userdata('role') !='admin')
			redirect(base_url() . 'index.php');
		
		$this->load->model("users_model");
		
		$input = $this->input->post();
		$id_stravnika = $input['id_stravnika'];
		$data_stravnik['login'] = $input["login:$id_stravnika"];
		$data_stravnik['role'] = $input["role:$id_stravnika"];

		$data_osoba['jmeno'] = $input["first_name:$id_stravnika"];
		$data_osoba['prijmeni'] = $input["last_name:$id_stravnika"];
		$data_osoba['telefon'] = $input["phone:$id_stravnika"];
		$data_osoba['email'] = $input["email:$id_stravnika"];

		$id_osoba = $this->users_model->get_id_osoba($id_stravnika);
		$this->users_model->update_osoba($id_osoba, $data_osoba);
		$this->users_model->update_stravnik($id_stravnika, $data_stravnik);
		redirect(base_url().'index.php/Admin');
	}			
}
?>