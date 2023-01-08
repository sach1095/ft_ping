#include "lib.h"

void InitStatsAndTime(t_stats *stats, t_args *args)
{
	stats->min = (float)INT32_MAX;
	stats->avg = 0;
	stats->max = 0;
	stats->mdev = 0;
	stats->time = 0;
	stats->seq = 0;
	stats->pkt_lost = 0;
	stats->time_gstart = 0;
	stats->pkt_error = 0;
	stats->time_start = 0;
	stats->time_recv = 0;
	stats->pkt_received = 0;
	stats->pkt_transmited = 0;
	stats->time_btw_packet = 0;
	stats->ip = NULL;
	stats->verbos = args->flags[V];
	stats->ip = args->ip_brut;
}

void	init_args(t_args *args)
{
	args->flags[0] = false;
	args->flags[1] = false;
	args->ip = NULL;
	args->ip_brut = NULL;
	args->hostname = NULL;
	args->sock = 0;
	args->ttl = 63;
	args->tv_out.tv_sec = 1;
	args->tv_out.tv_usec = 0;
	args->diff = false;
}
