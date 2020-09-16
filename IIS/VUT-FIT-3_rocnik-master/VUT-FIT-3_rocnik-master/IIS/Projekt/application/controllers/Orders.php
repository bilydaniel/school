<?php  
class Orders extends CI_Controller
{
	public function index()
	{
		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');
		
		
		$this->load->model("orders_model");

		$this->load->model("users_model");
		$data["logged_in_as"] = $this->session->userdata('username');
		$data["user_id"] = $this->session->userdata('id');

		$id_osoba = $this->users_model->get_id_osoba($this->session->userdata('id'));
		$data["user"] = $this->users_model->get_osoba_data($id_osoba);

		$orders = $this->orders_model->get_orders($id_osoba);
		foreach ($orders as $key => $value) {
			$orders[$key]['order_items'] = $this->orders_model->get_order_items($value['id_objednavky']);

		}

		$items = "";
		$data["orders"] = $orders;
		$tmp_items = $this->orders_model->get_items();
		foreach ($tmp_items as $key => $value) 
		{
			$items[$value['id_polozky']]= $value;
		}
		$data["items"] = $items;

			$this->load->view("orders.html",$data);

		}

		public function delete_order()
		{
			//TODO check data, session etc
			if(!$this->session->userdata('id'))
				redirect(base_url() . 'index.php');
		
			$this->load->model("orders_model");
			if($this->input->get())
			{
				$input = $this->input->get();
				$this->orders_model->remove_order($input['order_id']);
				
			}
			else
			{
				$input = $this->input->post();
				$order_id = $input['delete'];
				$this->orders_model->remove_order($order_id);
				
			}	
			redirect("Orders");
		}
	}

	?>