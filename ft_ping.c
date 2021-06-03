#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

typedef struct s_global
{
    char			*destination;
    int				transmitted;
	int				received;
	struct timeval	start;
	int				sockfd;
}               t_global;

t_global g_global;

void signal_handler(int sig)
{
	struct timeval	actual;

	close(g_global.sockfd);
	gettimeofday(&actual, NULL);
	double time = (double) (actual.tv_usec - g_global.start.tv_usec) / 1000 + (double) (actual.tv_sec - g_global.start.tv_sec) * 1000;
    printf("\n--- %s ping statistic ---\n", g_global.destination);
	printf("%d packets transmitted, %d received, 0%% packet loss, time %.0fms\n", g_global.transmitted, g_global.received, time);
	printf("rtt min/avg/max/mdev = ?/?/?/? ms\n");
    exit(0);
}

void	error(char *s)
{
	printf("\e[91mError: %s\e[0m\n", s);
	if (g_global.sockfd >= 0)
		close(g_global.sockfd);
	exit(1);
}

int main(int ac, char **av)
{


	struct sockaddr_in addr;

	int byte_count;
  	struct msghdr recvBuff;


    signal(SIGINT, signal_handler);
	memset(&recvBuff, 0, sizeof(recvBuff));

    if (ac == 1)
    {
        printf("Usage: %s [-h/-v] destination\n", av[0]);
        return (2);
    }

    g_global.destination = av[1];

	addr.sin_addr.s_addr = inet_addr((const char *)av[1]);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);


	if((g_global.sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
      error("Could not create socket");
  	else
	  printf("\e[92mCreated socket successfully.\e[0m\n");

	if(connect(g_global.sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
      error("Connect Failed");
 	else
	  printf("\e[92mConnected successfully to %s\e[0m\n", av[1]);

/* Try a first send but still wait response (maybe not a good sendto)
	struct sockaddr *dest;
	char buffer[4096];
	memset(&dest,0, sizeof(dest));

	sendto(g_global.sockfd, buffer, 4096, 0, dest, sizeof(dest));

	printf("Message sended ! Waiting for response...\n");

	byte_count = recvmsg(g_global.sockfd, &recvBuff, 0);

	printf("Received: %d packets // errno: %s\n", byte_count, strerror(errno));
*/

	gettimeofday(&g_global.start, NULL);

    while(1);
    return (0);
}