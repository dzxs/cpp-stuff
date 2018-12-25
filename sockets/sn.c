#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void packet_dump(const u_char *buf, const u_int len)
{
    u_char c;
    int i, j;
    for (i = 0; i < len; i++)
    {
        printf("%02x ", buf[i]);
        if ((i % 16) == 15 || (i == len - 1))
        {
            for (j = 0; i < 15 - (i % 16); j++)
            {
                printf("   ");
            }
            printf("| ");
            for (j = (i - (i % 16)); j <= i; j++)
            {
                c = buf[j];
                if ((c > 31) && (c < 127))
                {
                    printf("%c", c);
                }
                else
                {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
}

int main(int argc, char const *argv[])
{
    int received_length, sockfd;
    int i;
    u_char buf[5000];
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 5; i++)
    {
        received_length = recv(sockfd, buf, 4000, 0);
        printf("%d byte packet\n", received_length);
        packet_dump(buf, received_length);
    }
    return 0;
}
