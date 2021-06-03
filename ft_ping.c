#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

typedef struct s_global
{
    char *destination;
}               t_global;

t_global g_global;

void signal_handler(int sig)
{
    printf("\n--- %s ping statistic ---\n", g_global.destination);
    exit(sig);
}

int main(int ac, char **av)
{
    signal(SIGINT, signal_handler);
    if (ac == 1)
    {
        printf("Usage: %s [-h/-v] destination\n", av[0]);
        return (2);
    }

    g_global.destination = av[1];
    while(1);
    return (0);
}