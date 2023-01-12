#include "lib.h"

static void	PrintHelp()
{
	printf("\nUsage\n  ping [options] <destination>\n\nOptions:\n");
	printf("  <destination>      dns name or ip address\n");
	printf("  -h                 print help and exit\n");
	printf("  -v                 verbose output\n\n");
	printf("For more details see ping(8).\n");
	exit(0);
}

static bool ParsArgs(char **av, int ac, t_args *args)
{
	int i = 1;

	init_args(args);
	if (args->ttl == 0)
		return (PrintError("Ft_ping: cannot set unicast time-to-live: Invalid argument\n"));
	if (ac == 3){
		if (av[1][0] == '-' && (length(av[1]) >= 2 && length(av[1]) <= 3)){
			for (size_t i = 1; av[1][i] != '\0'; i++)
			{
				if (av[1][i] == 'h' || av[1][i] == 'v'){
					if (av[1][i] == 'h')
						args->flags[H] = true;
					else
						args->flags[V] = true;
				}
				else
					return (PrintError("Ft_Ping: invalid option -- 'x'.\n"));
			}
		}
		else
			return (PrintError("Ft_ping: Flags format invalide: you can use -[t, v] only.\n"));
		i++;
	}
	args->ip = av[i];
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	t_args				args;
	struct sockaddr_in	addr_config;

	if (!(ac >= 2 && ac <= 3))
		return (PrintError("Number of arguments incorrect. ./ft_ping [flags v,t (optionel)] [IpAdress (v4 only)].\n"));

	if (ParsArgs(av, ac, &args))
		return (EXIT_FAILURE);

	if (args.flags[H] == true)
		PrintHelp();

	if (ProcessHost(&args, &addr_config))
		return (EXIT_FAILURE);

	SendPing(&args, &addr_config);
	return (EXIT_SUCCESS);
}
