<?php  
class Restaurant extends CI_Controller
{
	public function index()
	{
			//TODO check session?
			//TODO nesmi byt restaurace s id = 0
		$input = $this->input->post();
		$this->load->model("restaurants_model");
		$this->load->model("orders_model");
		if(isset($input['remove_restaurant']))
		{
			$this->restaurants_model->remove_restaurant($input['remove_restaurant']);
			$items = $this->restaurants_model->get_menu($input['remove_restaurant']);
			foreach ($items as $key => $value) {
				$this->restaurants_model->delete_item($value['id_polozky']);
			}
			$orders = $this->orders_model->return_all_orders();
			foreach ($orders as $key => $value) {
				if(!$this->orders_model->get_order_items($value['id_objednavky']))
				{
					$this->orders_model->remove_order($value['id_objednavky']);
				}
			}
			redirect(base_url() . 'index.php');
		}
		
		
		$this->load->model("users_model");
		$data["logged_in_as"] = $this->session->userdata('username');
		$data["user_id"] = $this->session->userdata('id');
		
		$id_osoba = $this->users_model->get_id_osoba($this->session->userdata('id'));
		$data["user"] = $this->users_model->get_osoba_data($id_osoba);
		if($this->session->flashdata('restaurant_id'))
		{
			$id = $this->session->flashdata('restaurant_id');
		}
		else
		{
			if(!$this->input->post('id'))
				redirect(base_url() . 'index.php');

			$id = $this->input->post('id');
			if(!$this->restaurants_model->restaurant_exists($id))
				redirect(base_url() . 'index.php');
		}
		


		$restaurant = $this->restaurants_model->get_restaurant($id);
		$menu = $this->restaurants_model->get_menu($id);
		$menu_result = array();
		$searched = array();
		if($this->input->post("filter") && count($this->input->post()) > 2)
		{
			foreach ($menu as $key => $value) 
			{
				$types = $this->restaurants_model->get_types($value["id_polozky"]);
				foreach ($types as $type) 
				{

					if(in_array($type, $this->input->post()))
					{
						array_push($menu_result,$value);
						break;
					}
				}
				
			}
			
			if(isset($input["dia"]))
				array_push($searched, "dia");
			if(isset($input["vegan"]))
				array_push($searched, "vegan");
			if(isset($input["vegetarian"]))
				array_push($searched, "vegetarian");
			if(isset($input["gluten_free"]))
				array_push($searched, "gluten_free");
		}
		else
		{
			$menu_result = $menu;
		}
			//echo "<pre>";
			//print_r($menu);
		foreach ($menu_result as $key =>$item) 
		{
			if(!$item["trvala_polozka"])
			{
				if($item["zbyva"]<10)
				{
					$item["max"] = $item["zbyva"];
				}
				else
				{
					$item["max"] = 10;
				}
			}
			else
			{
				$item["max"] = 10;
			}
			$menu_result[$key] = $item;
		}

		$data["restaurant"] = $restaurant[0];

		foreach ($menu_result as $key => $value) 
		{
			$types = $this->restaurants_model->get_types($value["id_polozky"]);
			$menu_result[$key]['types'] = $types;
		}
		$data["menu"] = $menu_result;
		$data['searched'] = $searched;
			//echo "<pre>";
			//print_r($data);
		$this->load->view("restaurant.html", $data);
	}

	public function order()
	{
			//TODO check data, session etc.
		$input = $this->input->post();
		$this->load->model("restaurants_model");
		$this->load->model("orders_model");
		$this->load->model("users_model");

		$time = date("H:i:sa");
		echo $time;
		if(!isset($input))
		{
			$this->session->set_flashdata('restaurant_id', $input['restaurant']);
			redirect(base_url() . 'index.php/Restaurant');
		}
		if (strpos($input['insert'], 'editItem') !== false) 
		{
			$edit_exploded = explode(":",$input['insert']);
			$item_id = $edit_exploded[1];
			
			$nazev = isset($input["name:$item_id"]) ? $input["name:$item_id"] : "";
			$cena = isset($input["price:$item_id"]) ? $input["price:$item_id"] : "";
			$popis = isset($input["description:$item_id"]) ? $input["description:$item_id"] : "";
			$img = isset($input["img:$item_id"]) ? $input["img:$item_id"] : "";
			$zbyva = isset($input["remains:$item_id"]) ? $input["remains:$item_id"] : "";
			$trvala_polozka = isset($input["permanent:$item_id"]) ? '1' : '0';
			

			$data = array(
				'nazev' => $nazev,
				'cena' => $cena,
				'popis' => $popis,
				'img' => $img,
				'typ_polozky' => "",
				'trvala_polozka' => $trvala_polozka,
				'zbyva' => $zbyva

			);
			
			$this->restaurants_model->edit_item($item_id,$data);
			
			$this->restaurants_model->remove_types($item_id);
			if(isset($input["dia:$item_id"]))
				$this->restaurants_model->insert_type($item_id,"dia");
			if(isset($input["vegan:$item_id"]))
				$this->restaurants_model->insert_type($item_id,"vegan");
			if(isset($input["vegetarian:$item_id"]))
				$this->restaurants_model->insert_type($item_id,"vegetarian");
			if(isset($input["gluten_free:$item_id"]))
				$this->restaurants_model->insert_type($item_id,"gluten_free");

			$this->session->set_flashdata('restaurant_id', $input['restaurant']);
			redirect(base_url() . 'index.php/Restaurant');
		}
		elseif (strpos($input['insert'], 'order') !== false) 
		{
			
			if($this->session->userdata('id'))
			{
				$osoba_id = $this->users_model->get_id_osoba($this->session->userdata('id'));
				$data = array('id_osoby'=>$osoba_id,'id_provozovny' => $input['restaurant']);
				$is_order=false;
				foreach ($input as $key => $value) 
				{
					if (strpos($key, 'item') !== false && $value != 0) 
					{
						$is_order = true;
					}
				}
				if($is_order)
				{
					$order_id = $this->orders_model->make_order($data);
					$price=0;
				}

				
				foreach ($input as $key => $value) 
				{
					if (strpos($key, 'item') !== false && $value != 0) 
					{

						$item_exploded = explode(":",$key);
						$item_id = $item_exploded[1];
						$item_amount = $value;
						$database_data['id_polozky'] = $item_id;
						$database_data['mnozstvi'] = $item_amount;
						$database_data['id_objednavky'] = $order_id;
						$this->orders_model->item_to_order($database_data);

						$price+= $this->restaurants_model->get_price($item_id) * $item_amount;
					}
				}
				if($is_order)
					$this->orders_model->order_price($order_id,$price);

				$this->orders_model->edit_order_time($order_id,$time);
				$tmp_item = $this->orders_model->get_item($item_id);
				$this->orders_model->edit_amount($item_id,$tmp_item[0]['zbyva']-$item_amount);
				redirect(base_url() . 'index.php/Orders');	
			}
			else
			{
					//make osoba and order
				$osoba_data['telefon'] = $input['phone'];
				$osoba_data['adresa'] = $input['adresa'];
				
				$is_order=false;
				foreach ($input as $key => $value) 
				{
					if (strpos($key, 'item') !== false && $value != 0) 
					{
						$is_order = true;
					}
				}
				if($is_order)
				{
					$osoba_id = $this->users_model->create_osoba($osoba_data);
					$data = array('id_osoby'=>$osoba_id,'id_provozovny' => $input['restaurant']);
					$order_id = $this->orders_model->make_order($data);
					$price=0;
				}	
				
				

				foreach ($input as $key => $value) 
				{
					if (strpos($key, 'item') !== false && $value != 0) 
					{
						$item_exploded = explode(":",$key);
						$item_id = $item_exploded[1];
						$item_amount = $value;
						$database_data['id_polozky'] = $item_id;
						$database_data['mnozstvi'] = $item_amount;
						$database_data['id_objednavky'] = $order_id;
						$this->orders_model->item_to_order($database_data);

						$price+= $this->restaurants_model->get_price($item_id) * $item_amount;
					}
				}
				if($is_order)
					$this->orders_model->order_price($order_id,$price);
				$this->orders_model->edit_order_time($order_id,$time);
				$tmp_item = $this->orders_model->get_item($item_id);
				$this->orders_model->edit_amount($item_id,$tmp_item[0]['zbyva']-$item_amount);

				redirect(base_url() . 'index.php/Orders');
				
			}
		}
		elseif (strpos($input['insert'], 'register') !== false)
		{
				//make osoba and order
			$osoba_data['telefon'] = $input['phone'];
			$osoba_data['adresa'] = $input['adresa'];
			


			$is_order=false;
			foreach ($input as $key => $value) 
			{
				if (strpos($key, 'item') !== false && $value != 0) 
				{
					$is_order = true;
				}
			}
			if($is_order)
			{
				$price=0;
				$order_id = $this->orders_model->make_order($data);
			}	
			$osoba_id = $this->users_model->create_osoba($osoba_data);
			$data = array('id_osoby'=>$osoba_id,'id_provozovny' => $input['restaurant']);
			foreach ($input as $key => $value) 
			{
				if (strpos($key, 'item') !== false && $value != 0) 
				{
					$item_exploded = explode(":",$key);
					$item_id = $item_exploded[1];
					$item_amount = $value;
					$database_data['id_polozky'] = $item_id;
					$database_data['mnozstvi'] = $item_amount;
					$database_data['id_objednavky'] = $order_id;
					$this->orders_model->item_to_order($database_data);

					$price+= $this->restaurants_model->get_price($item_id) * $item_amount;
				}
			}
			if($is_order)
				$this->orders_model->order_price($order_id,$price);
			
			$this->orders_model->edit_order_time($order_id,$time);
			$tmp_item = $this->orders_model->get_item($item_id);
			$this->orders_model->edit_amount($item_id,$tmp_item[0]['zbyva']-$item_amount);

			$this->session->set_flashdata('osoba_id', $osoba_id);
			redirect(base_url() . 'index.php/Orders');
		}

			/*
			$order = array();
			foreach ($input as $key => $value) 
			{
				if(strpos($key, 'item') !== false) 
				{
					$item_exploded = explode(":",$key);
					$item_id = $item_exploded[1];
					$order[$item_id] = $value;
				}
			}
			*/
		}
		
		public function add_item()
		{
			if(!$this->session->userdata('id'))
				redirect(base_url() . 'index.php');
		
			if(!in_array($this->session->userdata('role'), array('admin','operator')))
				redirect(base_url() . 'index.php');	

			$this->load->model("restaurants_model");
			$input = $this->input->post();
			if(!$input)
				redirect(base_url() . 'index.php');
			
			if(!$input['restaurant'])
				redirect(base_url() . 'index.php');
			
			$nazev = isset($input['name']) ? $input['name'] : "";
			$cena = isset($input['price']) ? $input['price'] : "";
			$popis = isset($input['description']) ? $input['description'] : "";
			$img = ($input['img'] != "") ? $input['img'] : "http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/food.png";
			$zbyva = isset($input['remains']) ? $input['remains'] : "";
			$trvala_polozka = isset($input['permanent']) ? '1' : '0';
			

			$data = array(
				'nazev' => $nazev,
				'cena' => $cena,
				'popis' => $popis,
				'img' => $img,
				'trvala_polozka' => $trvala_polozka,
				'id_provozovny' => $input['restaurant'],
				'zbyva' => $zbyva
			);

			$inserted_id = $this->restaurants_model->insert_item($data);
			if(isset($input['dia']))
				$this->restaurants_model->insert_type($inserted_id,"dia");
			if(isset($input['vegan']))
				$this->restaurants_model->insert_type($inserted_id,"vegan");
			if(isset($input['vegetarian']))
				$this->restaurants_model->insert_type($inserted_id,"vegetarian");
			if(isset($input['gluten_free']))
				$this->restaurants_model->insert_type($inserted_id,"gluten_free");

			$this->session->set_flashdata('restaurant_id', $input['restaurant']);
			redirect(base_url() . 'index.php/Restaurant');
		}

		public function delete()
		{
			
			if(!$this->session->userdata('id') || $this->session->userdata('role')!== 'operator' && $this->session->userdata('role') !== 'admin')
			{
				$this->session->set_flashdata('restaurant_id', $restaurant_id);
				redirect(base_url() . 'index.php/Restaurant');
			}
			$this->load->model("restaurants_model");
			$this->load->model("orders_model");

			$input = $this->input->get();
			$restaurant_id = isset($input['restaurant_id']) ? $input['restaurant_id'] : "";
			$item_id = isset($input['item_id']) ? $input['item_id'] : "";
			if(!$restaurant_id || !$item_id)
			{
				$this->session->set_flashdata('restaurant_id', $restaurant_id);
				redirect(base_url() . 'index.php/Restaurant');
			}
			$this->restaurants_model->delete_item($item_id);
			

			$orders = $this->orders_model->return_all_orders();
			foreach ($orders as $key => $value) {
				if(!$this->orders_model->get_order_items($value['id_objednavky']))
				{
					$this->orders_model->remove_order($value['id_objednavky']);
				}

			}

			$this->session->set_flashdata('restaurant_id', $restaurant_id);
			redirect(base_url() . 'index.php/Restaurant');
			
		}

		public function edit_restaurant()
		{
			if(!$this->session->userdata('id'))
				redirect(base_url() . 'index.php');
		
			if(!in_array($this->session->userdata('role'), array('admin','operator')))
				redirect(base_url() . 'index.php');

			$this->load->model("restaurants_model");
			$input = $this->input->post();
			if(!$input)
				redirect(base_url() . 'index.php');
			
			if(!$input['restaurant'])
				redirect(base_url() . 'index.php');
			if(!$this->session->userdata('id'))
				redirect(base_url() . 'index.php');

			$img = isset($input['img']) ? $input['img'] : "";
			$nazev = isset($input['name']) ? $input['name'] : "";
			$popis = isset($input['description']) ? $input['description'] : "";
			$adresa = isset($input['address']) ? $input['address'] : "";
			$telefon = isset($input['number']) ? $input['number'] : "";
			$oteviraci_doba = isset($input['open_time']) ? $input['open_time'] : "";
			$zaviraci_doba = isset($input['close_time']) ? $input['close_time'] : "";
			$id_provozovny = isset($input['restaurant']) ? $input['restaurant'] : "";

			$data = array(
				'img' => $img,
				'nazev' => $nazev,
				'popis' => $popis,
				'adresa' => $adresa,
				'telefon' => $telefon,	
				'oteviraci_doba' => $oteviraci_doba,
				'zaviraci_doba' => $zaviraci_doba
			);

			$this->restaurants_model->edit_restaurant($id_provozovny, $data);
			$this->session->set_flashdata('restaurant_id', $id_provozovny);
			redirect(base_url() . 'index.php/Restaurant');
		}
		
	}

	?>