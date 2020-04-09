#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
        int dev, snaplen = 65535;
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *open_dev;
        const u_char *packet;
        pcap_if_t *alldevs;
        int count = atoi(argv[2]);
        printf("%d\n", count);
        char *devv = argv[1];
        struct pcap_pkthdr header;
        char CAS[100];
        //najdi vsechna network rozhrani
        dev = pcap_findalldevs(&alldevs, errbuf);
        if (dev) {
            fprintf(stderr, "Nastal error: %s\n", errbuf);
            return(1);
        }
        //zadne rozhrani nebylo nalezeno
        if(alldevs == NULL)
        {
            printf("Nenasel jsem zadne rozhrani\n");
            return(1);
        }
        pcap_if_t *ptr = alldevs;
        while(ptr != NULL)
        {
            printf("rozhrani: %s\n",ptr->name);
            ptr = ptr->next;
        }
        printf("odchytavam na: %s\n",devv);
        //ziska handle
        open_dev = pcap_open_live(devv, snaplen, 0, 0, errbuf);
        if (open_dev == NULL)
        {
            fprintf(stderr, "Nastal error: %s\n", errbuf);
            return(1);
        }
        for(int i = 0; i < count;++i)
        {
            packet = pcap_next(open_dev, &header);
            struct tm *tmp = localtime(&header.ts.tv_sec);
            if (tmp == NULL) return 1;
            int cas = sizeof(CAS);
            printf("cas: %s\n",strftime(CAS,cas, "%x",tmp));
            printf("size: %u\n",header.len);
            printf("%u\n",packet);
            if(packet == NULL)
            {
                fprintf(stderr, "Nastal error\n");
                return 1;
            }
        }
        pcap_close(open_dev);
        return(0);
}
