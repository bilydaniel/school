<?php  
class Operator extends CI_Controller
{
	public function index()
	{
		$this->load->model("orders_model");
		$this->load->model("users_model");

		if(!$this->session->userdata('id'))
			redirect(base_url() . 'index.php');
		
		if(!in_array($this->session->userdata('role'), array('admin','operator')))
			redirect(base_url() . 'index.php');

		$data["logged_in_as"] = $this->session->userdata('username');
		$data["user_id"] = $this->session->userdata('id');

		$id_osoba = $this->users_model->get_id_osoba($this->session->userdata('id'));
		$data["user"] = $this->users_model->get_osoba_data($id_osoba);

		$orders = $this->orders_model->get_operator_orders();
		$data["orders"] = $orders;
		$items = null;
		foreach ($orders as $key => $value) 
		{
			$tmp1 = $this->orders_model->get_order_items($value['id_objednavky']);
			$tmp2 = $this->orders_model->get_item($tmp1[0]['id_polozky']);
			$items[$value['id_objednavky']] = $tmp1[0]['mnozstvi'].'x '.$tmp2[0]['nazev'];
			array_shift($tmp1);
			foreach ($tmp1 as $key => $item)
			{
				$tmp2 = $this->orders_model->get_item($item['id_polozky']);
				$items[$value['id_objednavky']] .= ', '.$item['mnozstvi'].'x '.$tmp2[0]['nazev'];
			}
		}
		$data["items"] = $items;

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

		$data["drivers"] = $this->users_model->return_all_drivers();
/*			echo "<pre>";
			print_r($items);
*/


			$this->load->view("operator.html",$data);
		}

		function resolve_assignment(){
			if(!$this->session->userdata('id'))
				redirect(base_url() . 'index.php');
		
			if(!in_array($this->session->userdata('role'), array('admin','operator')))
				redirect(base_url() . 'index.php');

			$this->load->model("orders_model");
			$post = $this->input->post();
			$id_driver = $post['id_driver'];
			unset($post['id_driver']);
			$selected = array_keys($post);

			foreach ($selected as $key => $value) {
				$this->orders_model->edit_order_state($value,2);
				$this->orders_model->edit_order_driver($value,$id_driver);
			}
			redirect(base_url() . 'index.php/operator');
		}
	}
	?>