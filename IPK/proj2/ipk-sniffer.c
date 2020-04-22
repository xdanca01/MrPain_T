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
    struct ether_header *ether = (struct ether_header*)packet;
    char CAS[100];
    int x = pkthdr->len;
    time_t ttime = pkthdr->ts.tv_sec;
    struct tm *tmp = localtime(&ttime);
    char format[] = "%H:%M:%S";
    strftime(CAS,sizeof(CAS), format,tmp);
    char sourceIP[300], destIP[300];
    if(ether->ether_type == 8)
    {
        unsigned int sourcePort = packet[34] << 8 | packet[35];
        unsigned int destPort = packet[36] << 8 | packet[37];
        sprintf(sourceIP, "%d.%d.%d.%d",ether->ether_shost[0],ether->ether_shost[1],ether->ether_shost[2],ether->ether_shost[3]);
        sprintf(destIP, "%d.%d.%d.%d",ether->ether_dhost[0],ether->ether_dhost[1],ether->ether_dhost[2],ether->ether_dhost[3]);
        struct sockaddr_in srcHOST;
        srcHOST.sin_family = AF_INET;
        srcHOST.sin_port = sourcePort;
        inet_aton(sourceIP, &srcHOST.sin_addr);
        char SRChostt[200];
        int SRC = getnameinfo((struct sockaddr *)&srcHOST,sizeof(srcHOST),&SRChostt[0],200,NULL,0,0);
        struct sockaddr_in destHOST;
        destHOST.sin_family = AF_INET;
        destHOST.sin_port = destPort;
        inet_aton(destIP, &destHOST.sin_addr);
        char DSThostt[200];
        int DST = getnameinfo((struct sockaddr *)&destHOST,sizeof(destHOST),&DSThostt[0],200,NULL,0,0);
        if(SRC == 0 && DST == 0)
        {
            printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,DSThostt,destPort,x);
        }
        else if(SRC == 0 && DST) printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,destIP,destPort,x);
        else if(SRC && DST == 0) printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,sourceIP,sourcePort,DSThostt,destPort,x);
        else printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,sourceIP,sourcePort,destIP,destPort,x);
    }
    else if(ether->ether_type == 56710)
    {
        unsigned int dstIP[16] = {packet[38],packet[39],packet[40],packet[41],packet[42],packet[43],packet[44],packet[45],packet[46],packet[47],packet[48],packet[49],packet[50],packet[51],packet[52],packet[53]};
        unsigned int srcIP[16] = {packet[22],packet[23],packet[24],packet[25],packet[26],packet[27],packet[28],packet[29],packet[30],packet[31],packet[32],packet[33],packet[34],packet[35],packet[36],packet[37]};
        unsigned int sourcePort = packet[34] << 8 | packet[35];
        unsigned int destPort = packet[36] << 8 | packet[37];
        sprintf(sourceIP, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",srcIP[0],srcIP[1],srcIP[2],srcIP[3],srcIP[4],srcIP[5],srcIP[6],srcIP[7],srcIP[8],srcIP[9],srcIP[10],srcIP[11],srcIP[12],srcIP[13],srcIP[14],srcIP[15]);
        sprintf(destIP, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",dstIP[0],dstIP[1],dstIP[2],dstIP[3],dstIP[4],dstIP[5],dstIP[6],dstIP[7],dstIP[8],dstIP[9],dstIP[10],dstIP[11],dstIP[12],dstIP[13],dstIP[14],dstIP[15]);
        struct sockaddr_in srcHOST;
        srcHOST.sin_family = AF_INET6;
        srcHOST.sin_port = sourcePort;
        inet_aton(sourceIP, &srcHOST.sin_addr);
        char SRChostt[200];
        int SRC = getnameinfo((struct sockaddr *)&srcHOST,sizeof(srcHOST),&SRChostt[0],200,NULL,0,0);
        struct sockaddr_in destHOST;
        destHOST.sin_family = AF_INET6;
        destHOST.sin_port = destPort;
        inet_aton(destIP, &destHOST.sin_addr);
        char DSThostt[200];
        int DST = getnameinfo((struct sockaddr *)&destHOST,sizeof(destHOST),&DSThostt[0],200,NULL,0,0);
        printf("%d %d\n\n",DST,SRC);
        if(SRC == 0 && DST == 0)
        {   
            printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,DSThostt,destPort,x);
        }
        else if(SRC == 0 && DST) printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,SRChostt,sourcePort,destIP,destPort,x);
        else if(SRC && DST == 0) printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,sourceIP,sourcePort,DSThostt,destPort,x);
        else printf("%s.%06ld %s : %d > %s : %d\n length: %d\n",CAS,pkthdr->ts.tv_usec,sourceIP,sourcePort,destIP,destPort,x);

    }

    
    struct ether_header necoe;
    unsigned char *output;
    output = malloc(x*sizeof(unsigned char));
    if (output == NULL)
        exit(1);
    printf(" "); 
    for (int q = 0; q < x; ++q)
    {
        if(q % 8 == 0 && q != 0) printf(" ");
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
        if(q % 16 == 0) printf("0x%04X: ",q);
        printf("%02x",packet[q]);
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
            printf("  ");
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
        int snaplen = 65535;
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *open_dev = NULL;
        pcap_if_t *alldevs = NULL;
        int count = 1;
        char *port = NULL;
        char *interface = NULL;
        struct pcap_pkthdr header;
        static struct option long_options[] = {{"tcp", 0, NULL, 'x'}, {"udp", 0, NULL, 'y'}};
        int argument = getopt_long(argc, argv, "i:p:n:tu", long_options ,NULL);
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
                case 'x':
                    tcp = true;
                    break;
                case 'y':
                case 'u':
                    udp = true;
                    break;
                case 'i':
                    printf("%s\n",optarg);
                    interface = optarg;
                    break;
                case 'p':
                    port = optarg;
                    break;
                case 'n':
                    count = atoi(optarg);
                    break;
                case '?':
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
        printf("DEBUG: odchytavam na: %s\n",interface);
        //ziska handle
        open_dev = pcap_open_live(interface, snaplen, 1, 0, errbuf);
        if (open_dev == NULL)
        {
            fprintf(stderr, "Nastal error: %s\n", errbuf);
            return(1);
        }

        if( pcap_lookupnet(interface,&netip,&maska,errbuf) < 0 )
        {
            printf("pcap_lookupnet: %s\n", errbuf);
            return 1;
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
        if(pcap_compile(open_dev, &fp, protokol, 0, maska) == -1)
        {
            pcap_perror(open_dev,"prefix:");
            printf("%s\n", pcap_geterr(open_dev));
            fprintf(stderr, "Nastal error behem compilovani filteru\n");
            return 1;
        }
        if(pcap_setfilter(open_dev, &fp) < 0)
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
