/* 
 * File:   receiver_main.c
 * Author: 
 *
 * Created on
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define DATA 2000

using namespace std;

struct sockaddr_in si_me, si_other;
int s;
socklen_t slen;

void diep(char *s) {
    perror(s);
    exit(1);
}

typedef struct Header {
	int num_sequence;
	int data_size;
}header;

typedef struct Packet {
	int num_sequence;
	int size;
	char data[DATA];
}packet;

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
    char buf[2000];
    slen = sizeof (si_other);


    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");

    memset((char *) &si_me, 0, sizeof (si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(myUDPport);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Now binding\n");
    if (bind(s, (struct sockaddr*) &si_me, sizeof (si_me)) == -1)
        diep("bind");

    int correct_seq = 0;


	/* Now receive data and send acknowledgements */    
	    //header *h1 = new header;
	    //int num_bytes = recvfrom(s, h1, sizeof(*h1), 0, (struct sockaddr*)&si_other, &slen);
	    //if(num_bytes == -1) {
	//	    cout << "eoor"; 
	  //  }
     FILE *fp;
     fp = fopen(destinationFile, "ab");
     if(fp == NULL) {
	     cout << "File can not be opened";
	     exit(1);
     }
     while(true) {
	     packet *p1 = new packet;
	     int num_bytes = recvfrom(s, p1, sizeof(*p1), 0, (struct sockaddr*)&si_other, &slen);
	     if(num_bytes == -1) {
		     cout << "Error kkk";
	     }
	     if(p1->num_sequence == -1) {
		     cout << "Finish";
		     break;
	     }
	     if(p1->num_sequence == correct_seq) {
		     fwrite(p1->data, sizeof(char), p1->size, fp);

	     }
	     correct_seq ++;
     }

     fclose(fp);


    close(s);
	printf("%s received.", destinationFile);
    return;
}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;

    if (argc != 3) {
        fprintf(stderr, "usage: %s UDP_port filename_to_write\n\n", argv[0]);
        exit(1);
    }

    udpPort = (unsigned short int) atoi(argv[1]);

    reliablyReceive(udpPort, argv[2]);
}

