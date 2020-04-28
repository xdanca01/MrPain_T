#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

//callback funkce pro zpracovani dat packetu
void callback_for_packet(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    //struktura, která získá údaje z packetu
    struct ether_header *ether = (struct ether_header*)packet;
    char CAS[100];
    int x = pkthdr->len;
    time_t ttime = pkthdr->ts.tv_sec;
    //převod času
    struct tm *tmp = localtime(&ttime);
    char format[] = "%H:%M:%S";
    //formulace času
    strftime(CAS,sizeof(CAS), format,tmp);
    char sourceIP[300], destIP[300];
    printf("\n");
    printf("%d\n\n\n",ntohs(ether->ether_type));
    //pokud je packet typu ipv4 - 0x0800
    if(ether->ether_type == 2048)
    {
        //ziska delku ip headeru pro offset k portum
        unsigned int IPlen = (unsigned int)(packet[14] & 15) << 2;
        //srcPort na ethernet + ipheader len + 0
        uint16_t sourcePort = ntohs(*(uint16_t *)&packet[14+IPlen]);
        //dstPort na ethernet + ipheader len + 2
        uint16_t destPort = ntohs(*(uint16_t *)&packet[16+IPlen]);
        
        struct in_addr srcADR,dstADR;
        //získá bajty z packetu
        memcpy((void *)&srcADR,(void *)&packet[26],4);
        memcpy((void *)&dstADR,(void *)&packet[30],4);
       
        //převede bajty na IPv4 se správným byte order
        char *pamet = inet_ntoa(srcADR);
        char *pamet2 = inet_ntoa(dstADR);
        
        struct sockaddr_in srcHOST;
        //IPV4
        srcHOST.sin_family = AF_INET;
        srcHOST.sin_port = sourcePort;
        srcHOST.sin_addr = srcADR;
        char SRChostt[200];

        //zkusi ziskat hostname na srcIP
        int SRC = getnameinfo((struct sockaddr *)&srcHOST,sizeof(srcHOST),&SRChostt[0],200,NULL,0,0);

        //to stejny jen pro dstIP
        struct sockaddr_in destHOST;

        destHOST.sin_family = AF_INET;
        destHOST.sin_port = destPort;

        destHOST.sin_addr = dstADR;
        char DSThostt[200];

        int DST = getnameinfo((struct sockaddr *)&destHOST,sizeof(destHOST),&DSThostt[0],200,NULL,0,0);
        
        
        //podařilo se resolvnout src i dst hostname
        if(SRC == 0 && DST == 0)
        {
            printf("%s.%06ld %s : %d > %s : %d\n length: %d\n\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,DSThostt,destPort,x);
        }

        //resolvnuti src hostname
        else if(SRC == 0 && DST) printf("%s.%06ld %s : %d > %s : %d\n\n length: %d\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,pamet2,destPort,x);
        
        //resolvnuti dst hostname
        else if(SRC && DST == 0) printf("%s.%06ld %s : %d > %s : %d\n\n length: %d\n",CAS,pkthdr->ts.tv_usec,pamet,sourcePort,DSThostt,destPort,x);
        
        //bez resolvnuti
        else printf("%s.%06ld %s : %d > %s : %d\n length: %d\n\n",CAS,pkthdr->ts.tv_usec,pamet,sourcePort,pamet2,destPort,x);
    
    }
    //end IPV4
    
    //IPV6 - 0x86DD
    else if(ether->ether_type == 34525)
    {
        //převod z dat packetu do pole int
        struct in6_addr dstIP;
        memcpy((void *)&dstIP,(void *)&packet[38],16);
        
        //převod z dat packetu do pole int
        struct in6_addr srcIP;
        memcpy((void *)&srcIP,(void *)&packet[22],16);
        
        //srcPort na ethernet (14) + ipv6 (40) + 0
        uint16_t sourcePort = ntohs(*(uint16_t *)&packet[54]);
        //dstPort na ethernet (14) + ipv6 (40) + 2
        uint16_t destPort = ntohs(*(uint16_t *)&packet[56]);

        struct sockaddr_in6 srcHOST, destHOST;
        //IPv6
        srcHOST.sin6_family = AF_INET6;
        srcHOST.sin6_port = sourcePort;

        srcHOST.sin6_addr = srcIP;
        char SRChostt[200];

        //resolvnuti src hostname
        int SRC = getnameinfo((struct sockaddr *)&srcHOST,sizeof(srcHOST),&SRChostt[0],200,NULL,0,0);

        //IPv6
        destHOST.sin6_family = AF_INET6;
        destHOST.sin6_port = destPort;

        destHOST.sin6_addr = dstIP;
        char DSThostt[200];

        //resolvnuti dst hostname
        int DST = getnameinfo((struct sockaddr *)&destHOST,sizeof(destHOST),&DSThostt[0],200,NULL,0,0);
        
        //resolvnuti src i dst
        if(SRC == 0 && DST == 0)
        {   
            printf("%s.%06ld %s : %d > %s : %d\n length: %d\n\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,DSThostt,destPort,x);
        }

        //resolvnutí src
        else if(SRC == 0 && DST) printf("%s.%06ld %s : %d > %s : %d\n length: %d\n\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,dstIP.s6_addr,destPort,x);
        
        //resolvnutí dst
        else if(SRC && DST == 0) printf("%s.%06ld %s : %d > %s : %d\n length: %d\n\n",CAS,pkthdr->ts.tv_usec,srcIP.s6_addr,sourcePort,DSThostt,destPort,x);
        
        //bez resolvnutí
        else printf("%s.%06ld %s : %d > %s : %d\n length: %d\n\n",CAS,pkthdr->ts.tv_usec,srcIP.s6_addr,sourcePort,dstIP.s6_addr,destPort,x);

    }
    //not supported
    else
    {
        fprintf(stderr, "Nepodporovaný EtherType\n");
        return;
    }
    
    unsigned char *output;
    output = malloc(x*sizeof(unsigned char));

    if (output == NULL)
    {
        fprintf(stderr, "Malloc fail\n");
        exit(1);
    }
    printf(" ");
    //výpis a převod obsahu packetu (x = len packetu a neměl by být větší než snaplen) 
    for (int q = 0; q < x && x <= 65535; ++q)
    {
        
        if(q % 8 == 0 && q != 0) printf(" ");
        
        //výpis tisknutelných znaků
        if(q % 16 == 0 && q != 0)
        {
            printf(" ");
            for(int d = q-16; d < q; ++d)
            {
                if(d % 8 == 0) printf(" ");
                printf("%c", output[d]);
            }
            printf("\n");
        }

        if (q > 0) printf(" ");

        //print počtu vypsaných řádků
        if(q % 16 == 0) printf("0x%04X: ",q);
        
        //výpis bajtu v hexa
        printf("%02x",packet[q]);
        
        //pokud je znak tisknutelný zapiš ho do pole output
        if((int)packet[q] >= 32 && (int)packet[q] <= 127)
        {
            output[q] =(unsigned char) packet[q];
        }
        //pokud není tisknutelný ulož '.'
        else
        {
            output[q] = '.';
        }
        //pokud nedošlo k vypsání obsahu packetu po 16 bajtech a má se skončit vypiš tisknutelné znaky
        if(q + 1 == x && q % 16 != 0)
        {
            //doplnění mezer pro zarovnání
            for(int d = 16 - (q % 16) ; d > 0; --d)
                printf("   ");

            //výpis tisknutelných znaků
            for(int d = x - (q % 16) - 1; d < x; ++d)
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
        //maximální uložená délka packetu v bajtech
        int snaplen = 65535;
        //místo pro uložení chybového hlášení
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *open_dev = NULL;
        pcap_if_t *alldevs = NULL;
        int count = 1;
        char *port = NULL;
        char *interface = NULL;
        static struct option long_options[] = {{"tcp", 0, NULL, 'x'}, {"udp", 0, NULL, 'y'}};
        int argument = getopt_long(argc, argv, "i:p:n:tu", long_options ,NULL);
        bool tcp = false, udp = false;
        char *protokol = NULL;
        
        while(argument != -1)
        {
            switch (argument)
            {
                case 't':
                case 'x':
                    tcp = true;
                    break;
                case 'y':
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
                case '?':
                    if(strcmp(argv[optind-1],"-i") == 0)
                        break;
                default:
                    fprintf(stderr, "Error with argument occupied\n");
                    return 1 ;
            }
            argument = getopt_long(argc, argv, "i::p:n:tu", long_options ,NULL);
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
        //ziska handle
        open_dev = pcap_open_live(interface, snaplen, 1, 0, errbuf);
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
        //konec nastaveni filteru

        //nastavi char* filter do filter programu
        if(pcap_compile(open_dev, &fp, protokol, 0, PCAP_NETMASK_UNKNOWN) == -1)
        {
            fprintf(stderr, "Nastal error behem kompilovani filteru do fp\n");
            return 1;
        }
        //nastavi filtrovani pomoci fp
        if(pcap_setfilter(open_dev, &fp) < 0)
        {
            fprintf(stderr, "Nastal error behem nastavovani filteru\n");
            return 1;
        }
        //odchytí count packetů na open_dev a pošle packet data do funkce callback_for_packet
        pcap_loop(open_dev,count,callback_for_packet,NULL);
        //zavře handle k packetum na danem interface
        pcap_close(open_dev);
        free(protokol);
        return(0);
}
