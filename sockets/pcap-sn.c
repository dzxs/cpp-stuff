#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

void packet_dump(const u_char *buf, const u_int len)
{
    u_char c;
    int i, j;
    for (i = 0; i < len; i++)
    {
        printf("%02x ", buf[i]);
        if ((i % 16) == 15 || (i == len - 1))
        {
            for (j = 0; j < 15 - (i % 16); j++)
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
    struct pcap_pkthdr header;
    const u_char *packet;
    char err_buf[PCAP_ERRBUF_SIZE];
    char *device;
    pcap_t *pcap_handle;
    int i;

    device = pcap_lookupdev(err_buf);
    if (!device)
    {
        printf("Error in %s: %s\n", "pcap_lookupdev", err_buf);
        exit(EXIT_FAILURE);
    }
    printf("Sniffing on device %s\n", device);

    pcap_handle = pcap_open_live(device, 4096, 1, 0, err_buf);
    if (!pcap_handle)
    {
        printf("Error in %s: %s\n", "pcap_open_live", err_buf);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 5; i++)
    {
        packet = pcap_next(pcap_handle, &header);
        printf("%d byte packet\n", header.len);
        packet_dump(packet, header.len);
    }
    pcap_close(pcap_handle);
    return 0;
}
