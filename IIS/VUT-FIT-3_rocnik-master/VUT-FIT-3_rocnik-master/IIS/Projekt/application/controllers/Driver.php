<?php  
class Driver extends CI_Controller
{
	public function index()
	{
		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');
		
		if(!in_array($this->session->userdata('role'), array('admin','driver')))
			redirect(base_url() . 'index.php');

		$this->load->model("orders_model");

		$this->load->model("users_model");
		$data["logged_in_as"] = $this->session->userdata('username');
		$data["user_id"] = $this->session->userdata('id');

		$id_osoba = $this->users_model->get_id_osoba($this->session->userdata('id'));
		$data["user"] = $this->users_model->get_osoba_data($id_osoba);

		$orders = $this->orders_model->get_driver_orders($id_osoba);
		$data["orders"] = $orders;

		$restaurants = "";
		$this->load->model("restaurants_model");
		$tmp_restaurants = $this->restaurants_model->return_all_restaurants();
		foreach ($tmp_restaurants as $key => $value) 
		{
			$restaurants[$value['id_provozovny']]= $value;
		}
		$data["restaurants"] = $restaurants;

		$this->load->model("users_model");
		$tmp_users = $this->users_model->return_all_users();
		foreach ($tmp_users as $key => $value) 
		{
			$users[$value['id_osoby']]= $value;
		}
		$data["users"] = $users;
/*			echo "<pre>";
			print_r($items);
*/


			$this->load->view("driver.html",$data);

		}

		function resolve_order(){
			if(!$this->session->userdata('id'))
				redirect(base_url() . 'index.php');
		
			if(!in_array($this->session->userdata('role'), array('admin','driver')))
				redirect(base_url() . 'index.php');
			$id = $this->input->post('resolve_order');

			$this->load->model("orders_model");

			/*if($this->session->userdata('role') != 'admin' || $this->session->userdata('role') != 'driver'){

			}*/ //TODO
			$this->orders_model->edit_order_state($id,3);
			redirect(base_url() . 'index.php/driver');
		}


	}

	?>