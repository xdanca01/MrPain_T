#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>


//callback funkce pro zpracovani dat packetu
void callback_for_packet(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    char CAS[100];
    int x = pkthdr->len - 2;
    time_t ttime = pkthdr->ts.tv_sec;
    struct tm *tmp = localtime(&ttime);
    char format[] = "%H:%M:%S";
    strftime(CAS,sizeof(CAS), format,tmp);
    int sourcePort = 0;
    memcpy(&(((char *)&sourcePort)[2]),(void *)&packet[35],2);
    printf("%d %s.%06ld %d\n", x,CAS,pkthdr->ts.tv_usec,sourcePort);
    unsigned char *output;
    output = malloc(x*sizeof(unsigned char));
    if (output == NULL)
        exit(1);
    printf(" "); 
    for (int q = 0; q < x; ++q)
    {
        if(q == 12)
        {
            q = q + 1;
            continue;
        }
        if(q % 16 == 0 && q != 0)
        {
            printf(" ");
            for(int d = q-16; d < q; ++d)
            {
                if(d % 8 == 0 && d != 0) printf(" ");
                printf("%c", output[d]);
            }
            printf("\n");
        }
        if (q > 0) printf(" ");
        printf("%02X",packet[q]);
        //0xff
        if((int)packet[q] >= 32 && (int)packet[q] <= 127)
        {
            output[q] =(unsigned char) packet[q];
        }
        else
        {
            output[q] = '.';
        }
        if(q + 1 == x && q % 16 != 0)
        {
            printf(" ");
            for(int d = x - (q % 16); d < x; ++d)
            {
                if(d % 8 == 0 && d != 0) printf(" ");
                printf("%c", output[d]);
            }
            printf("\n");
        }
    }
    free(output);
    return;
}

int main(int argc, char *argv[])
{
        int snaplen = 65535;
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *open_dev = NULL;
        pcap_if_t *alldevs = NULL;
        int count = 1;
        char *port = NULL;
        char *interface = NULL;
        struct pcap_pkthdr header;
        static struct option long_options[] = {{"tcp", 0, NULL, 't'}, {"udp", 0, NULL, 'u'}};
        int argument = argument = getopt_long(argc, argv, "i:p:n:", long_options ,NULL);
        bool tcp = false, udp = false;
        char *protokol = NULL;
        struct ether_header *ether_ptr;
        bpf_u_int32 maska;
        bpf_u_int32 netip;


        while(argument != -1)
        {
            switch (argument)
            {
                case 't':
                    tcp = true;
                    break;
                case 'u':
                    udp = true;
                    break;
                case 'i':
                    interface = optarg;
                    break;
                case 'p':
                    port = optarg;
                    break;
                case 'n':
                    count = atoi(optarg);
                    break;
                default:
                    fprintf(stderr, "Error with argument occupied\n");
                    return 1 ;
            }
            argument = getopt_long(argc, argv, "i:p:n:", long_options ,NULL);
        }
        //najdi vsechna dostupna rozhrani
        if(interface == NULL)
        {
            if(pcap_findalldevs(&alldevs, errbuf))
            {
                fprintf(stderr, "Nastal error: %s\n", errbuf);
                return(1);
            }
            printf("Seznam vsech rozhrani dostupnych:\n");
            //vypis vsech rozhrani
            while(alldevs)
            {
                printf("%s\n", alldevs->name);
                alldevs = alldevs->next;
            }
            return 0;
        }
        pcap_lookupnet(interface,&netip,&maska,errbuf);
        printf("DEBUG: odchytavam na: %s\n",interface);
        //ziska handle
        open_dev = pcap_open_live(interface, snaplen, 1000, 0, errbuf);
        if (open_dev == NULL)
        {
            fprintf(stderr, "Nastal error: %s\n", errbuf);
            return(1);
        }


        //proces nastaveni filteru
        struct bpf_program fp;
        if(tcp == true)
        {
            char ppp[4] = "tcp";
            protokol = realloc(protokol, sizeof(protokol) + sizeof(ppp));
            if (protokol == NULL) return 1;
            protokol = strcat(protokol, ppp);
        }
        if(udp == true)
        {
            if(protokol == NULL)
            {
                char ppp[4] = "udp";
                protokol = realloc(protokol, sizeof(protokol) + sizeof(ppp));
                if (protokol == NULL) return 1;
                protokol = strcat(protokol, ppp);
            }
            else
            {
                free(protokol);
                char ppp[14] = "(tcp or udp)";
                protokol = realloc(protokol, sizeof(protokol) + sizeof(ppp));
                if (protokol == NULL) return 1;
                protokol = strcat(protokol, ppp);
            }
        }

        if(port != NULL)
        {
            if(protokol == NULL)
            {
                char ppp[9] = "port ";
                protokol = realloc(protokol,sizeof(protokol)+sizeof(ppp)+sizeof(port));
                if (protokol == NULL) return 1;
                protokol = strcat(protokol, ppp);
                protokol = strcat(protokol, port);
            }
            else
            {
                char ppp[12] = " and port ";
                protokol = realloc(protokol,sizeof(protokol)+sizeof(ppp)+sizeof(port));
                if (protokol == NULL) return 1;
                protokol = strcat(protokol, ppp);
                protokol = strcat(protokol, port);
            }

        }
        printf("DEBUG: protokol: %s\n", protokol);
        if(pcap_compile(open_dev, &fp, protokol, 0, netip) == -1)
        {
            pcap_perror(open_dev,"prefix:");
            printf("%s\n", pcap_geterr(open_dev));
            fprintf(stderr, "Nastal error behem compilovani filteru\n");
            return 1;
        }
        if(pcap_setfilter(open_dev, &fp) == -1)
        {
            fprintf(stderr, "Nastal error behem nastavovani filteru\n");
            return 1;
        }
        /*for(int i = 0; i < count;++i)
        {
            const u_char *packet = NULL, *packet2 = NULL;
            packet = pcap_next(open_dev, &header);
            if(packet == NULL)
            {
                fprintf(stderr, "Error pcap_next()\n");
                return 1;
            }
            int x = header.len - 2;
            packet2 = malloc(x*sizeof(u_char));
            if (packet2 == NULL)
                return 1;
            packet2 = (u_char*)memcpy((void *)packet2, (const void*) packet, x);
            time_t ttime = header.ts.tv_sec;
            struct tm *tmp = localtime(&ttime);
            char format[] = "%H:%M:%S";
            strftime(CAS,sizeof(CAS), format,tmp);
            unsigned char sourcePort[4];
            memcpy((void *)&sourcePort[2],(void *)packet2[33],2);
            printf("%s.%06ld %d\n", CAS,header.ts.tv_usec,(int)*sourcePort);
            unsigned char *output;
            output = malloc(x*sizeof(unsigned char));
            if (output == NULL)
                return 1;
            printf(" ");
            for (int q = 0; q < x; ++q)
            {
                if(q % 16 == 0 && q != 0)
                {
                    printf(" ");
                    for(int d = q-16; d < q; ++d)
                    {
                        if(d % 8 == 0 && d != 0) printf(" ");
                        printf("%c", output[d]);
                    }
                    printf("\n");
                }
                if (q > 0) printf(" ");
                printf("%02X",packet2[q]);
                //0xff
                if((int)packet2[q] >= 32 && (int)packet2[q] <= 127)
                {
                    output[q] =(unsigned char) packet2[q];
                }
                else
                {
                    output[q] = '.';
                }
                if(q + 1 == x && q % 16 != 0)
                {
                    printf(" ");
                    for(int d = x - (q % 16); d < x; ++d)
                    {
                        if(d % 8 == 0 && d != 0) printf(" ");
                        printf("%c", output[d]);
                    }
                    printf("\n");
                }
            }
            free((void *)packet2);
            free(output);
        }*/
        pcap_loop(open_dev,count,callback_for_packet,NULL);
        pcap_close(open_dev);
        free(protokol);
        return(0);
}
