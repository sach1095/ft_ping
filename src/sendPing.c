#include "lib.h"

t_stats stats;

static void CatchSignal(int signal)
{
	(void)signal;
	PrintStats(stats);
	exit(0);
}

static void SaveStats(t_stats *stats){

	float old_time = 0;
	stats->time_recv = get_time();
	old_time = stats->time;
	stats->time = stats->time_recv - stats->time_start;
	if (stats->min > stats->time)
		stats->min = stats->time;
	if (stats->max < stats->time)
		stats->max = stats->time;
	stats->avg += stats->time;
	if (stats->seq > 1){
		if (stats->time > old_time)
			stats->mdev += stats->time - old_time;
		else
			stats->mdev += old_time - stats->time;
	}
}

static t_packet create_packet(t_stats *stats)
{
	unsigned long i = 0;
	t_packet packet;

	ft_bzero(&packet, sizeof(packet));
	packet.hdr.type = ICMP_ECHO;
	packet.hdr.un.echo.id = getpid();

	while (i < sizeof(packet.msg)-1){
		packet.msg[i] = i + '0';
		i++;
	}
	packet.msg[i] = 0;
	packet.hdr.un.echo.sequence = stats->seq++;
	packet.hdr.checksum = CalcChecksum(&packet, sizeof(packet));
	usleep(PING_SLEEP_RATE);
	return packet;
}

int	SendPing(t_args *args, struct sockaddr_in *addr_config)
{
	struct sockaddr_in *addr_config_recev;
	unsigned int size_config_recev = sizeof(addr_config_recev);
	InitStatsAndTime(&stats, args);
	signal(SIGINT, CatchSignal);
	bool sucess;

	printf("PING %s (%s) 56(84) bytes of data.\n", args->ip_brut, args->ip);
	if (setsockopt(args->sock, SOL_IP, IP_TTL, &args->ttl, sizeof(args->ttl)) != 0)
		return PrintError("Set option socket fail");

	setsockopt(args->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&args->tv_out, sizeof args->tv_out);
	stats.time_gstart = get_time();
	while (1)
	{
		sucess = true;
		t_packet packet = create_packet(&stats);
		stats.time_start = get_time();
		if (sendto(args->sock, &packet, sizeof(packet), 0,
			(struct sockaddr*) addr_config, sizeof(*addr_config)) <= 0)
			sucess = false;
		else
			stats.pkt_transmited++;
		int code;
		if (!(code = recvfrom(args->sock, &packet, sizeof(packet), 0,
			(struct sockaddr*)&addr_config_recev, &size_config_recev) <= 0 && stats.seq > 1))
		{
			SaveStats(&stats);
			if (sucess)
			{
				if(packet.hdr.type == RESERVED && packet.hdr.code == ECHO_REPLY)
					PrintReceiveSuccess(args, &stats);
				else if (packet.hdr.type == RESERVED && packet.hdr.code == TTL_EXCCEDED && args->flags[V] == true)
					PrintReceiveFail(args, &stats);
			}
		}
	}
	return 0;
}
