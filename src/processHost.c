#include "lib.h"

static void chek_rev_dns(char *ip_addr, t_args *args)
{
	struct sockaddr_in temp_addr;
	socklen_t len;
	char buf[NI_MAXHOST] = {0};
	args->hostname = NULL;

	temp_addr.sin_family = AF_INET;
	temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
	len = sizeof(struct sockaddr_in);

	if (getnameinfo((struct sockaddr *) &temp_addr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		return ;
	else
		args->hostname = ft_strcpy(args->hostname, buf);
}

int ProcessHost(t_args *args, struct sockaddr_in *addr_config)
{
	struct hostent *host_entity;

	/*
	** Check if we found hostname witch the ip
	** if we find the host so return struct hostent witch the information on it
	** else the host not found so exit witch msg error
	*/
	if ((host_entity = gethostbyname(args->ip)) == NULL){
		printf("ft_ping: %s: Name or service not known\n", args->ip);
		return 1;
	}

	/*
	** Converts the Internet host address cp from the IPv4 numbers-and-dots
	** to notation into binary form
	*/
	args->ip_brut = args->ip;
	args->ip = inet_ntoa(*(struct in_addr *)host_entity->h_addr);

	if (ft_strncmp(args->ip_brut, args->ip, length(args->ip_brut)))
		args->diff = true;
	/*
	** Set the type of the future socket
	** sin_familly represente the type of the family
	** sin_port represente the port to contact
	** sin_addr represente the address of the host
	*/
	(*addr_config).sin_family = host_entity->h_addrtype;
	(*addr_config).sin_port = htons(0);
	(*addr_config).sin_addr.s_addr  = *(long*)host_entity->h_addr;

	chek_rev_dns(args->ip, args);
	args->sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	return (EXIT_SUCCESS);
}
