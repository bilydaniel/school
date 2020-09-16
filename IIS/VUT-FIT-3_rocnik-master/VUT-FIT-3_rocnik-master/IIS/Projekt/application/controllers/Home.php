<?php  
class Home extends CI_Controller
{
	public function index()
	{
		$this->load->model("restaurants_model");
		$this->load->model("users_model");

			$data["restaurants"] = $this->restaurants_model->return_all_restaurants();//TODO finish restaurants model


			//posilat data neni potreba, staci session
			$data["logged_in_as"] = $this->session->userdata('username');
			$data["user_id"] = $this->session->userdata('id');
			
			$id_osoba = $this->users_model->get_id_osoba($this->session->userdata('id'));
			$data["user"] = $this->users_model->get_osoba_data($id_osoba);
			//echo "<pre>";
			//print_r($data["restaurants"]);
			$this->load->view("index.html",$data);//TODO change	
			
			//$this->load->view("home.html",$data);//TODO change	
		}

		

		function validate_login()
		{
			$this->load->library('form_validation');
			

			$this->form_validation->set_rules('username','Username','required');
			$this->form_validation->set_rules('password','Password','required');

			if($this->form_validation->run())
			{
				$this->load->model('users_model');

				$username = $this->input->post('username');  
				$password = $this->input->post('password');  

				if($this->users_model->can_login($username, $password))
				{
					$user_id = $this->users_model->get_user_id($username);
					if(!$user_id)
					{
                		//TODO osetrit
					}
					$osoba_id = $this->users_model->get_id_osoba($user_id);
					$address = $this->users_model->get_address($osoba_id);
					$phone = $this->users_model->get_phone($osoba_id);
					$role =  $this->users_model->get_role($user_id);
					$session_data = array('username'=>$username,'id'=>$user_id,'address'=>$address,'phone'=>$phone,'role'=>$role);  
					$this->session->set_userdata($session_data);

					redirect(base_url() . 'index.php');
				}
				else
				{
					$this->session->set_flashdata('login_err', '1');
					redirect(base_url() . 'index.php');
				}

			}
			else
			{
				redirect(base_url() . 'index.php');
			}
		}

		function logout()
		{
			$this->session->unset_userdata('username');  
			$this->session->unset_userdata('id'); 
			$this->session->unset_userdata('address'); 
			$this->session->unset_userdata('phone'); 
			$this->session->unset_userdata('role'); 
			redirect(base_url() . 'index.php');
		}

		function register()
		{	

			$this->load->library('form_validation');

			$this->form_validation->set_rules('username','Username','required');
			$this->form_validation->set_rules('password','Password','required');
			$this->form_validation->set_rules('confirm_password', 'Confirm Password', 'required|matches[password]');

			if($this->form_validation->run())
			{
				$this->load->model('users_model');

				$username = $this->input->post('username');  
				$password = $this->input->post('password');  
				$address = $this->input->post('address');
				$phone = $this->input->post('phone');

				if($this->users_model->user_exists($username))
				{
					$this->session->set_flashdata('error', 'This username is already taken.');
					redirect(base_url() . 'index.php'); 
				}
				else
				{
					if($this->input->post('osoba_id'))
					{
						$osoba_id = $this->input->post('osoba_id');
					}
					else
					{
						$osoba_data['jmeno'] = '';
						$osoba_id = $this->users_model->create_osoba($osoba_data);
					}

					if($this->users_model->register_user($username, $password,$osoba_id))
					{
						$user_id = $this->users_model->get_user_id($username);
						if(!$user_id)
						{
                			//TODO osetrit
						}



						$osoba_id = $this->users_model->get_id_osoba($user_id);
						$this->users_model->set_address($osoba_id,$address);
						$this->users_model->set_phone($osoba_id,$phone);
						

						$role =  $this->users_model->get_role($user_id);
						$session_data = array('username'=>$username,'id'=>$user_id,'address'=>$address,'phone'=>$phone,'role'=>$role); 
						$this->session->set_userdata($session_data);
						redirect(base_url() . 'index.php');
                		//TODO finish me
					}
					else
					{
						$this->session->set_flashdata('error', 'Registration was not successful, try again later');
						redirect(base_url() . 'index.php');
					}
				}
			}
			else
			{
				$this->session->set_flashdata('error', 'Registration was not successful');
				$this->index();
			}
		}


		function add_restaurant()
		{
			//TODO check session, check incoming data
			if(!$this->session->userdata('id'))
				redirect(base_url() . 'index.php');
		
			if(!in_array($this->session->userdata('role'), array('admin','operator')))
				redirect(base_url() . 'index.php');

			$this->load->model("restaurants_model");
			$input = $this->input->post();
			if(!$input)
				redirect(base_url() . 'index.php');
			
			$nazev = isset($input['name']) ? $input['name'] : "";
			$popis = isset($input['description']) ? $input['description'] : "";
			$obrazek = ($input['img'] != "") ? $input['img'] : "http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/restaurant.png";
			$adresa = isset($input['address']) ? $input['address'] : "";
			$telefon = isset($input['number']) ? $input['number'] : "";
			$oteviraci_doba = isset($input['open_time']) ? $input['open_time'] : "";
			$zaviraci_doba = isset($input['close_time']) ? $input['close_time'] : "";

			$data = array(
				'nazev' => $nazev,
				'popis' => $popis,
				'img' => $obrazek,
				'adresa' => $adresa,
				'telefon' => $telefon,
				'oteviraci_doba' => $oteviraci_doba,
				'zaviraci_doba' => $zaviraci_doba
			);

			$this->restaurants_model->add_restaurant($data);
			redirect(base_url() . 'index.php');
		}
	}

	?>