#pragma once

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <netdb.h>
# include <stdbool.h>
# include <netdb.h>
# include <signal.h>
# include <float.h>

# define H 0
# define V 1
# define PKT_SIZE 64
# define RESERVED 69
# define TTL_EXCCEDED 192
# define ECHO_REPLY 0
# define PING_SLEEP_RATE 1000000

/*
** Struct for main variables
*/
typedef struct s_args
{
	char				*ip;
	char				*ip_brut;
	char				*hostname;
	int					sock;
	bool				diff;
	bool				flags[2];
	int					ttl;
	struct timeval		tv_out;
}t_args;

/*
** Strcut for save the times of icmp packet like :
** min 41.445ms : best low time (minimum)
** avg 50.697 : average times (moyenne)
** max 110.247ms : best high time (maximum)
** mdev : 15.931 ms : moyenne des écarts de temps de chaque ping
*/
typedef struct s_stats
{
	float	min;
	float	avg;
	float	max;
	float	mdev;
	float	time;
	int		seq;
	int		pkt_lost;
	int		pkt_error;
	int		pkt_received;
	int		pkt_transmited;
	double	time_gstart;
	double	time_start;
	double	time_recv;
	double	time_btw_packet;
	char	*ip;
	bool	verbos;
}t_stats;

/*
** Struct for the icmp packet
*/
typedef struct s_packet
{
	struct icmphdr hdr;
	char msg[PKT_SIZE-sizeof(struct icmphdr)];
}t_packet;


/*******************************
** Methods in different files **
*******************************/

/*
** utils.c
*/
int		length(char *str);
double	get_time(void);
void	ft_bzero(void *s, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strcpy(char *dest, char *src);
unsigned short CalcChecksum(void *packet, int len);

/*
** sendPing.c
*/
int		SendPing(t_args *args, struct sockaddr_in *addr_config);

/*
** getHost.c
*/
int ProcessHost(t_args *args, struct sockaddr_in *addr_config);

/*
** printMethod.c
*/
int		PrintError(char *strError);
void	PrintStats(t_stats stats);
void	PrintReceiveSuccess(t_args *args, t_stats *stats);
void	PrintReceiveFail(t_args *args, t_stats *stats);

/*
** init.c
*/
void	InitStatsAndTime(t_stats *stats, t_args *args);
void	init_args(t_args *args);
