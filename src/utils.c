#include "lib.h"

int length(char *str){
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

double	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char*)s;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t x;

	x = 0;
	if (n == 0)
		return (0);
	while ((s1[x] && s2[x]) && x < n - 1)
	{
		if (s1[x] != s2[x])
			return ((unsigned char)s1[x] - (unsigned char)s2[x]);
		x++;
	}
	return ((unsigned char)s1[x] - (unsigned char)s2[x]);
}

char *ft_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	dest = (char *)malloc(sizeof(char) * (length(src) + 1));
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/*
** Calculating the Check Sum
** see paragraphe 4.1  "C" of http://www.faqs.org/rfcs/rfc1071.html for mnore detail
*/
unsigned short CalcChecksum(void *packet, int len)
{
	unsigned short *buf = packet;
	unsigned int sum = 0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;
	if ( len == 1 )
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}
