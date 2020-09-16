<?php  
class Account extends CI_Controller
{
	public function index()
	{
		$this->load->model("users_model");
		
		if(!$this->session->userdata('id') || !$this->session->userdata('username'))
			redirect(base_url() . 'index.php');

		$user_name = $this->session->userdata('username');
		$user_id = $this->session->userdata('id');
		
		if(!$this->users_model->user_exists($user_name))
			redirect(base_url() . 'index.php');
		
		$id_osoba = $this->users_model->get_id_osoba($user_id);
		$data = $this->users_model->get_osoba_data($id_osoba);

		$this->load->view("account.html",$data);
	}

	public function update_user()
	{
		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');

		$this->load->library('form_validation');
		$this->form_validation->set_rules('username','Username','required');
			//TODO finish rules

		if($this->form_validation->run())
		{
			$this->load->model('users_model');

			$username = $this->input->post('username');
			$user_id = $this->session->userdata('id');
			if($username !== $this->session->userdata('username'))
			{
				$this->session->set_userdata('username', $username);
				$this->users_model->change_username($user_id,$username);
			} 
			$user_id = $this->session->userdata('id');
			$osoba_id = $this->users_model->get_id_osoba($user_id);
			$update_data['jmeno'] = $this->input->post('jmeno'); 	
			$update_data['prijmeni'] = $this->input->post('prijmeni'); 
			$update_data['telefon'] = $this->input->post('telefon'); 
			$update_data['email'] = $this->input->post('email');
			$update_data['adresa'] = $this->input->post('adresa');

			$this->users_model->update_osoba($osoba_id,$update_data);

			$address = $this->users_model->get_address($osoba_id);
			$phone = $this->users_model->get_phone($osoba_id);
			$session_data = array('address'=>$address,'phone'=>$phone);  
			$this->session->set_userdata($session_data);
				//redirect(base_url() . 'index.php/Account');
				//redirect($this->index());
			redirect("");
		}
		else
		{

			redirect(base_url() . 'index.php');
				//TODO print out warning
		}
	}

	public function change_password()
	{
		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');

		$this->load->library('form_validation');
		$this->load->model('users_model');
		
		$this->form_validation->set_rules('password','Password','required');
		$this->form_validation->set_rules('new_password', 'Password', 'required');
		$this->form_validation->set_rules('new_password_conf', 'Confirm Password', 'required|matches[new_password]');


		if($this->form_validation->run())
		{
			$username = $this->session->userdata('username');
			$password = $this->input->post('password');
			$new_password = $this->input->post('new_password');
			if($this->users_model->can_login($username, $password))
			{
				$id = $this->session->userdata('id');
				$this->users_model->update_password($id, $new_password);
				redirect("");
			}
			else
			{
				$this->session->set_flashdata('error', 'Change of password not successful');
				redirect("");
			}
		}
		else
		{
			$this->session->set_flashdata('error', 'Change of password not successful');
			redirect("");
		}
	}
}

?>