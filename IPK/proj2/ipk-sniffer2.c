#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        int dev, snaplen = 65535;
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *open_dev = NULL;
        pcap_if_t **alldevs = NULL;
        int count = atoi(argv[2]);
        printf("%s\n", argv[2]);
        //najdi vsechna network rozhrani
        dev = pcap_findalldevs(alldevs, errbuf);
        if (dev) {
            fprintf(stderr, "Nastal error: %s\n", errbuf);
            return(1);
        }
        //zadne rozhrani nebylo nalezeno
        if(alldevs == NULL)
        {
            printf("Nenasel jsem zadne rozhrani");
            return(1);
        }
        pcap_if_t *ptr = *alldevs;
        while(ptr != NULL)
        {
            printf("rozhrani: %s\n",ptr->name);
            ptr = ptr->next;
        }
        return(1);
        //ziska handle
        /*open_dev = pcap_open_live(argv[1], snaplen, 1, 1000, errbuf);
        if (open_dev == NULL)
        {
            fprintf(stderr, "Nastal error: %s", errbuf);
            return(1);
        }
        //aktivuje zachytavani packetu
        int pckts = pcap_activate(open_dev);
        //< 0 error
        if(pckts < 0)
        {
            fprintf(stderr, "Nastal error: %d", pckts);
            return(1);
        }
        //uspech s varovanim
        else if(pckts)
        {
            fprintf(stderr, "Warning pri zachytavani packetu: %d", pckts);
        }
        for(int i = 0; i < count;++i)
        {
        
        }
        pcap_close(open_dev);*/
        return(0);
}
