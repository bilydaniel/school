#include "d6r.h"
#include "dhcpv6.h"
using namespace std;

//GLOBAL VARIABLES
ifaddrs* gl_if;
struct arguments *args_struct;

int main(int argc, char**argv)
{
    //join multicast
    //open socket
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    //bind the socket
    struct sockaddr_in6 address = {AF_INET6, htons(4321)};
    bind(fd, (struct sockaddr*)&address, sizeof address);
    //join group
    struct ipv6_mreq group;
    group.ipv6mr_interface = 0;
    inet_pton(AF_INET6, "ff12::1234", &group.ipv6mr_multiaddr);
    if(setsockopt(fd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &group, sizeof group)<0)
        exit_program(INTERNAL_ERROR);

    //arguments
    args_struct = (arguments*)malloc(sizeof(arguments));//saving all the arguments
    if(!args_struct)
        exit_program(MALLOC_ERROR);
    bool s_missing = true;//argument -s missing
    vector<string> args(argv, argv+argc);
    char* interface_c = NULL;//argument -i
    int arg;//for switch

    //test if -s is present
    for (size_t i = 1; i < args.size(); ++i)
    {
        if (args[i] == "-s")
        {
            s_missing = false;
        }
    }
    if(s_missing)
    {
        exit_program(MISSING_ARGUMENT);
    }
   //get argumetns, save the values
    while((arg = getopt(argc, argv, "s:ldi:")) != -1)
    {
        switch(arg)
        {
            case 's':
                memcpy(args_struct->server_ip,optarg,sizeof(args_struct->server_ip));
                break;

            case 'l':
                args_struct->logging = true;
                break;

            case 'd':
                args_struct->debug = true;
                break;

            case 'i':
                interface_c = optarg;
                break;

            default:
                print_help();
        }
    }
    //check server ip
    struct addrinfo hints, *res;
    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    if(getaddrinfo(args_struct->server_ip, NULL, &hints, &res) != 0)
        exit_program(WRONG_SERVER_IP);

    //interfaces
    ifaddrs *ifap;
    vector<ifaddrs*>interfaces;//vector of all used interfaces
    int in_vector = 0;
    //get interfaces
    if(getifaddrs(&ifap) != 0)
        exit_program(INTERNAL_ERROR);

    //if the interface is not given save all of them, otherwise take only the one given
    if(interface_c == NULL)
    {
        for(; ifap!=NULL; ifap = ifap->ifa_next)
        {
            in_vector = 0;
            if (ifap->ifa_addr == NULL)
                continue;
            for(int i = 0;i < interfaces.size(); i++)
            {
                //if its already in the vector
                if(strcmp(ifap->ifa_name,interfaces[i]->ifa_name) == 0)
                {
                    in_vector = 1;
                    break;
                }
            }
            if(!in_vector && (ifap->ifa_addr->sa_family==AF_INET6))
            {
                if(IN6_IS_ADDR_GLOBAL(((sockaddr_in6 *)ifap->ifa_addr)->sin6_addr.s6_addr) || IN6_IS_ADDR_ULA(((sockaddr_in6 *)ifap->ifa_addr)->sin6_addr.s6_addr))
                    interfaces.push_back(ifap);
            }
        }
    }
    else
    {
        for(; ifap!=NULL; ifap = ifap->ifa_next)
        {
            if (ifap->ifa_addr == NULL)
                continue;
            if(ifap->ifa_addr->sa_family==AF_INET6)
            {
                if(strcmp(ifap->ifa_name,interface_c) == 0)
                {
                    if(IN6_IS_ADDR_GLOBAL(((sockaddr_in6 *)ifap->ifa_addr)->sin6_addr.s6_addr) || IN6_IS_ADDR_ULA(((sockaddr_in6 *)ifap->ifa_addr)->sin6_addr.s6_addr))
                    {
                        interfaces.push_back(ifap);
                        break;
                    }
                }
            }
        }
    }

    //fork for every interface saved
    pid_t pid;
    for(int i = 0;i < interfaces.size(); i++)
    {
        pid = fork();
        //error
        if(pid == -1)
            exit_program(FORK_ERROR);
        //child process
        else if(pid == 0)
        {
            //call function for listening on the interface
            listen_on_if(interfaces[i]);
        }
        //parent process
        else
        {
            continue;
        }

    }
    //Sleep the main process
    wait(NULL);
    //if any child process ends => error

    exit_program(INTERNAL_ERROR);
    return 0;
}

void listen_on_if(ifaddrs* interface)
{
    gl_if = interface;//every process sets its interface global
    char *dev,err_buffer[PCAP_ERRBUF_SIZE];
    struct bpf_program fp; //compiled filter expression
    char filter_exp[] = "src port 546";//filter expression
    bpf_u_int32 mask;
    bpf_u_int32 net;
    pcap_t *handle;
    dev = interface->ifa_name;

    //get net info
    if (pcap_lookupnet(dev, &net, &mask, err_buffer) == -1)
    {
        net = 0;
        mask = 0;
        exit_program(PCAP_ERROR);
    }
    //open listening
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, err_buffer);
    if(handle == NULL)
        exit_program(PCAP_ERROR);
    //compile filter
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
    {
        exit_program(PCAP_ERROR);
    }
    //apply filter
    if (pcap_setfilter(handle, &fp) == -1)
    {
        exit_program(PCAP_ERROR);
    }
    //listen for incoming packets, callback to handle_packet
    pcap_loop(handle, -1, handle_packet, NULL);//-1 for infinite
    pcap_close(handle);
}

void handle_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    struct ether_header *eptr;
    struct ip6_hdr *ipv6;
    struct udphdr *udpptr;
    struct dhcpv6_packet *dhcpv6ptr;
    size_t dhcp_packet_size = header->len - (SIZE_ETHERNET + SIZE_IPV6 + SIZE_UDP);//dhcp = length of the packet - ethernet - ipv6 - udp

    //relay forward structure
    struct dhcpv6_relay_packet *dhcprelay = (struct dhcpv6_relay_packet*)malloc(sizeof(struct dhcpv6_relay_packet));
    if(dhcprelay == NULL)
        exit_program(INTERNAL_ERROR);
    //relay option structure
    struct dhcp_option *option_relay = (struct dhcp_option *)malloc(sizeof(struct dhcp_option) );
    if(option_relay == NULL)
        exit_program(INTERNAL_ERROR);
    //link layer address option
    struct link_option *link_opt = (struct link_option *)malloc(sizeof(struct link_option));
    if(link_opt == NULL)
        exit_program(INTERNAL_ERROR);

    //get the ethernet header
    if(header->len < SIZE_ETHERNET + SIZE_IPV6 + SIZE_UDP)
        exit_program(BAD_PACKET);
    eptr = (struct ether_header *)packet;
    string client_mac = ether_ntoa((const struct ether_addr *)&eptr->ether_shost);
    //get ipv6 header
    ipv6 = (struct ip6_hdr*)(packet + SIZE_ETHERNET);
    //get udp header
    udpptr = (struct udphdr*)(packet + SIZE_ETHERNET + SIZE_IPV6);
    //get dhcpv6 header
    dhcpv6ptr = (struct dhcpv6_packet*)(packet + SIZE_ETHERNET + SIZE_IPV6 + SIZE_UDP);

    //fill the relay forward structure
    dhcprelay->msg_type = DHCPV6_RELAY_FORW;
    dhcprelay->hop_count = 0;
    memcpy(dhcprelay->link_address ,((struct sockaddr_in6 *)gl_if->ifa_addr)->sin6_addr.s6_addr , sizeof(((struct sockaddr_in6 *)gl_if->ifa_addr)->sin6_addr.s6_addr));
    memcpy(dhcprelay->peer_address,ipv6->ip6_src.s6_addr,sizeof(dhcprelay->peer_address));

    //fill the relay option
    option_relay->option_code = htons(D6O_RELAY_MSG);
    option_relay->option_len = htons(dhcp_packet_size);

    //fill the link layer address option
    link_opt->option_code = htons(D6O_CLIENT_LINKLAYER_ADDR);
    link_opt->option_len = htons(8);//6 for mac address 2 for type
    link_opt->link_type = htons(1);
    memcpy(link_opt->address, eptr->ether_shost, sizeof(eptr->ether_shost));
    //buffers
    char send_buffer[sizeof(struct dhcpv6_relay_packet) + sizeof(struct dhcp_option) + dhcp_packet_size + sizeof(struct link_option)];
    char recv_buffer[UDPBUFFER];
    memset(recv_buffer,0,UDPBUFFER);
    //create the packet for server
    memcpy(send_buffer,dhcprelay,sizeof(*dhcprelay));
    memcpy(send_buffer + sizeof(struct dhcpv6_relay_packet),option_relay,sizeof(struct dhcp_option));
    memcpy(send_buffer + sizeof(struct dhcpv6_relay_packet) + sizeof(struct dhcp_option),  dhcpv6ptr,dhcp_packet_size);
    memcpy(send_buffer + sizeof(struct dhcpv6_relay_packet) + sizeof(struct dhcp_option) + dhcp_packet_size, link_opt, sizeof(struct link_option));
    //send the packet, recieve response
    communicate_to_server(send_buffer,sizeof(send_buffer), recv_buffer);
    struct ia_address *ia_result = (struct ia_address*)malloc(sizeof(struct ia_address));
    if(ia_result == NULL)
        exit_program(MALLOC_ERROR);
    //takes the packet apart and sends it to the client
    int proc_res = process_packet(recv_buffer,ia_result);
    //logging
    if(proc_res != 0)
    {
        char recv_iaip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(ia_result->ipv6_addr), recv_iaip, INET6_ADDRSTRLEN);
        string output = recv_iaip;
        output.append(",");
        output.append(client_mac);
        if(args_struct->debug)
            cout << output << endl;
        const char * c = output.c_str();
        openlog("d6r", LOG_NDELAY, LOG_USER);
        if(args_struct->logging)
            syslog(LOG_DEBUG, c);
        closelog();
    }

}
int communicate_to_server(char *send_buffer,int s_buffer_size, char *recv_buffer)
{
    int i,send_sock;
    struct sockaddr_in6 server, relay;

    //create the socket
    if ((send_sock = socket(AF_INET6 , SOCK_DGRAM , 0)) == -1)   //create a client socket
    {
        exit_program(INTERNAL_ERROR);
    }

    const int trueFlag = 1;
    if (setsockopt(send_sock, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
        exit_program(INTERNAL_ERROR);

    //bind the socket
    relay.sin6_family = AF_INET6;
    relay.sin6_addr = in6addr_any;
    relay.sin6_port = htons(SERVER_PORT);
    int bind_status = bind(send_sock, (struct sockaddr *) &relay, sizeof(relay));
    if(bind_status != 0)
        exit_program(INTERNAL_ERROR);
    inet_pton(AF_INET6, args_struct->server_ip, &server.sin6_addr);
    server.sin6_family = AF_INET6;
    server.sin6_port = htons(SERVER_PORT);
    socklen_t len = sizeof(server);
    i = sendto(send_sock,send_buffer,s_buffer_size,0,(struct sockaddr *) &server, len);
    if(i == -1)
    {
        exit_program(INTERNAL_ERROR);
    }


    recv_from_server(send_sock, recv_buffer, relay);

    close(send_sock);
    return 1;
}

int recv_from_server(int send_sock, char *recv_buffer,struct sockaddr_in6 relay)
{
    int recv_status;
    socklen_t len;
    len = sizeof(relay);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec =0;

    if(setsockopt(send_sock, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) < 0)
    {

        exit_program(INTERNAL_ERROR);
    }


    recv_status = recvfrom(send_sock, recv_buffer, UDPBUFFER,0,(struct sockaddr *) &relay, &len);
    if(recv_status == -1)
        exit_program(INTERNAL_ERROR);



}

int process_packet(char *recv_buffer, struct ia_address *ia_result)
{
    struct dhcpv6_relay_packet *recv_dhcp;
    struct dhcp_option *tmp_option, *recv_relay_option;
    struct dhcpv6_packet *dhcp_header;
    struct dhcp_option *iana;
    struct dhcp_option *tmp_ia;

    size_t options_start;
    size_t options_offset = 0;
    size_t options_relay = 0;

    recv_dhcp = (struct dhcpv6_relay_packet *)recv_buffer;
    options_start = sizeof(struct dhcpv6_relay_packet);
    //cycle through all the option until you find relay option
    tmp_option = (struct dhcp_option*)(recv_buffer + sizeof(struct dhcpv6_relay_packet));
    while(ntohs(tmp_option->option_code) != D6O_RELAY_MSG && ntohs(tmp_option->option_len) != 0)
    {
        options_offset += sizeof(struct dhcp_option) + tmp_option->option_len;
        tmp_option = (struct dhcp_option*)(recv_buffer + options_start + options_offset);
    }
    //relay_msg option must be there
    if(ntohs(tmp_option->option_code) != D6O_RELAY_MSG)
        exit_program(INTERNAL_ERROR);

    options_relay = options_start + options_offset;
    recv_relay_option = tmp_option;

    dhcp_header = (struct dhcpv6_packet *)(recv_buffer + options_relay + sizeof (struct dhcp_option));

    options_start = options_relay + sizeof (struct dhcp_option) + sizeof(struct dhcpv6_packet);
    options_offset = 0;

    tmp_ia = (dhcp_option*)(recv_buffer + options_start);
    //find an IA option
    int ia_found = 0; //1 == IANA, 2 == IATA, 3 == IAPD
    while(ntohs(tmp_ia->option_code) != 0 && ntohs(tmp_ia->option_len) != 0)//ntohs(iana->option_code) != 3 &&
    {
        tmp_ia = (dhcp_option*)(recv_buffer + options_start + options_offset);
        if(ntohs(tmp_ia->option_code) == D6O_IA_NA)
        {
            ia_found = 1;
            break;
        }
        else if(ntohs(tmp_ia->option_code) == D6O_IA_TA)
        {
            ia_found = 2;
            break;
        }
        else if(ntohs(tmp_ia->option_code) == D6O_IA_PD)
        {
            ia_found = 3;
            break;
        }
        options_offset += sizeof(struct dhcp_option) + ntohs(tmp_ia->option_len);
    }

    //get ip from the ia option
    int ia_addr_start;
    int ia_addr_offset = 0;
    if(ia_found == 1 || ia_found == 2)
    {

        struct ia_address *ia_addr;
        if(ia_found == 1)
            ia_addr_start = options_start + options_offset + 16;
        else
            ia_addr_start = options_start + options_offset + 8;

        ia_addr = (struct ia_address *) (recv_buffer + ia_addr_start);
        while(ntohs(ia_addr->ia_code) != D6O_IAADDR && ia_addr->ia_len != 0)
        {
            ia_addr = (struct ia_address *) (recv_buffer +ia_addr_start + ia_addr_offset);
            ia_addr_offset += 4 + ntohs(ia_addr->ia_len);//4 is the header of ia option
        }
        if(ntohs(ia_addr->ia_code) != D6O_IAADDR)
        {
            ia_found = 0;
        }
        else
        {
            memcpy(ia_result,ia_addr,sizeof(struct ia_address));
        }
    }
    else if (ia_found == 3)
    {
        struct ia_prefix *ia_pref;
        ia_addr_start = options_start + options_offset + 16;

        ia_pref = (struct ia_prefix *) (recv_buffer + ia_addr_start);
        while(ntohs(ia_pref->ia_code) != D6O_IAPREFIX && ntohs(ia_pref->ia_len) != 0)
        {
            ia_pref = (struct ia_prefix *) (recv_buffer +ia_addr_start + ia_addr_offset);
            ia_addr_offset += 4 + ntohs(ia_pref->ia_len);//4 is the header of ia option
        }
        if(ntohs(ia_pref->ia_code) != D6O_IAPREFIX)
        {
            ia_found = 0;
        }
        else
        {
            memcpy(ia_result,ia_pref,sizeof(struct ia_address));
        }
    }
    //send the recieved packet to client
    send_to_client(ntohs(recv_relay_option->option_len), dhcp_header,recv_dhcp);
    return ia_found;
}

int send_to_client(int client_dh_size,struct dhcpv6_packet *dhcp_header, struct dhcpv6_relay_packet *recv_dhcp)
{
    int client_sock, client_len;
    struct sockaddr_in6 client;

    if ((client_sock = socket(AF_INET6 , SOCK_DGRAM , 0)) == -1)   //create a client socket
    {
        err(1,"socket() failed\n");
    }
    const int trueFlag = 1;
    if (setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
        exit_program(120);

    char client_buffer[client_dh_size];
    memcpy(client_buffer,dhcp_header,client_dh_size);

    client.sin6_family = AF_INET6;
    client.sin6_port = htons(CLIENT_PORT);
    memcpy(&client.sin6_addr,recv_dhcp->peer_address,sizeof(recv_dhcp->peer_address));
    client_len = sizeof(client);

    uint32_t interfaceIndex = if_nametoindex(gl_if->ifa_name);
    client.sin6_scope_id = interfaceIndex;
    int i;
    i = sendto(client_sock,client_buffer,sizeof(client_buffer),0,(struct sockaddr *) &client, client_len);
    if(i == -1)
        exit_program(INTERNAL_ERROR);

}
static void print_help()
{
    cout << "d6r -s server [-l] [-d] [-i interface]" << endl;
    cout << "-server musi byt platna ipv6 adresa" << endl;
    cout << "-l je priznak pro logovani pomoci syslog" << endl;
    cout << "-d je priznak pro debugging, vypis pridelene ipv6 adresy a mac adresy na vystup" << endl;
    cout << "-interface je nazev rozhrani na kterem ma relay poslouchat, pri prazdne hodnote posloucha na vsech, rozhrani musi mit platnou ULA/GUA adresu" << endl;
    exit(-1);
}
void exit_program(int err)
{
    string msg;
    switch(err)
    {
        case INTERNAL_ERROR:
            msg = "Internal error";
            break;
        case PCAP_ERROR:
            msg = "Pcap error";
            break;
        case MALLOC_ERROR:
            msg = "Malloc error";
            break;
        case WRONG_SERVER_IP:
            msg = "Wrong server ip";
            break;
        case FORK_ERROR:
            msg = "Fork error";
            break;
        case BAD_PACKET:
            msg = "Bad packet";
            break;
        case MISSING_ARGUMENT:
            msg = "Missing argument";
            break;
    }
    cerr << "ERROR: " << msg << endl;
    exit(-1);

}


