#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "pub.h"
#include "sub.h"
#include <cjson/cJSON.h>
//const char *topic_toR = "Robofork/" "3" "/toR"; 
#define True 1
//#define MAX_EVENTS 5

static int epfd;

static struct epoll_event events;

int sockID;
//return socket id 
int sock_canid(int sockid){
	extern int sockID;
	sockID =sockid;
	printf("Socket id is %d",sockID);
	return sockID;
}


////

int can_init()
{
    printf("CAN has been Initialized\n");
    
        int usleep(useconds_t usec);
        int s;
	    int s2;
        int nbytes;
        struct sockaddr_can addr;
        struct can_frame frame;
        struct ifreq ifr;
        struct can_filter filterID[20];
        struct can_filter
        {
        canid_t can_id;
        canid_t can_mask;
        };
    

    const char *ifname = "vcan0"; // "slcan0";
    
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Error while opening socket");
        return -1;
    }
    
    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);
    
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    //  setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    printf("%s at index %d\n", ifname, ifr.ifr_ifindex);
    
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error in socket bind");
        return -2;
    }
    
    frame.can_dlc = 8;
    
    // The fileters have been addeed, reciving data will be only from designated can ID numbers
	//["401", "402", "403", "1F0", "27D", "270", "27F", "1A0", "4A0", "169", "110", "119", "180", "190"]
// always share ["270", "27F", "4A0"]
    filterID[0].can_id = 0x401;
    filterID[0].can_mask = CAN_EFF_MASK;
    filterID[1].can_id = 0x402;
    filterID[1].can_mask = CAN_EFF_MASK;
    filterID[2].can_id = 0x403;
    filterID[2].can_mask = CAN_EFF_MASK;
    filterID[3].can_id = 0x1F0;
    filterID[3].can_mask = CAN_EFF_MASK;
	filterID[4].can_id = 0x27D;
    filterID[4].can_mask = CAN_EFF_MASK;
	filterID[5].can_id = 0x270;
    filterID[5].can_mask = CAN_EFF_MASK;
	filterID[6].can_id = 0x27F;
    filterID[6].can_mask = CAN_EFF_MASK;
	filterID[7].can_id = 0x1A0;
    filterID[7].can_mask = CAN_EFF_MASK;
	filterID[8].can_id = 0x4A0;
    filterID[8].can_mask = CAN_EFF_MASK;
	filterID[9].can_id = 0x169;
    filterID[9].can_mask = CAN_EFF_MASK;
	filterID[10].can_id = 0x110;
    filterID[10].can_mask = CAN_EFF_MASK;
	filterID[11].can_id = 0x119;
    filterID[11].can_mask = CAN_EFF_MASK;
	filterID[12].can_id = 0x180;
    filterID[12].can_mask = CAN_EFF_MASK;
	
	filterID[13].can_id = 0x190;
    filterID[13].can_mask = CAN_EFF_MASK;
	filterID[14].can_id = 0x270;
    filterID[14].can_mask = CAN_EFF_MASK;
	
	filterID[15].can_id = 0x27F;
    filterID[15].can_mask = CAN_EFF_MASK;
	filterID[16].can_id = 0x4A0;
    filterID[16].can_mask = CAN_EFF_MASK;
	perror("after filters ");

    //Filters have been applied on the socket
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &filterID, sizeof(filterID));
   //  printf("%d\n",s);
	
	//printf("Socket id is  . . . . . .%d \n",sock_canid(s));
	perror("return ......");
return s;
}



void can_recv(int s, int id){
//     printf("%s","Inside can recieve ");
   	int nbytes;
	char *topic_toS = "Robofork/3/toS";
	char CandumpDataPublish[100];
	struct can_frame frame;
	int nfd;
    nbytes = read(s, &frame, sizeof(struct can_frame));

/// copy to another string 

	
	sprintf(CandumpDataPublish,"%x%x%x%x%x%x%x%x%x",frame.can_id, frame.data[0], frame.data[1], frame.data[2],frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);





//// copy to another string 


//sprintf(CandumpDataPublish,"%x%x%x%x%x%x%x%x%x",
   	//frame.can_id, frame.data[0], frame.data[1], frame.data[2],
   	//frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);
 	printf("Reciving Data In can recieve function: ID:%x -  %x %x %x %x %x %x %x %x \n",
   frame.can_id, frame.data[0], frame.data[1], frame.data[2],
   frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);
	
#if defined(TEST)
   printf("Reciving Data In can recieve function: ID:%x -  %x %x %x %x %x %x %x %x \n",
   frame.can_id, frame.data[0], frame.data[1], frame.data[2],
   frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]);	
	
#endif
	int ret = publishData(CandumpDataPublish,topic_toS);
}


void can_send(int s, int id,unsigned char *value){
  	int nbytes;
	struct can_frame frame;

	frame.can_id=id;
	perror("CAN SEND");
	//frame.can_id=id;
	frame.can_dlc=8;
	frame.data[0]=value[0];
	frame.data[1]=value[1];
	frame.data[2]=value[2];
	frame.data[3]=value[3];
	frame.data[4]=value[4];
	frame.data[5]=value[5];
	frame.data[6]=value[6];
	frame.data[7]=value[7];
	perror(value);
	printf("Value  0 is %d\n",value[0]);
	printf("Value  1 is %d\n",value[1]);
	printf("Value  2 is %d\n",value[2]);
	printf("Value  3 is %d\n",value[3]);
	printf("Value  4 is %d\n",value[4]);
	printf("Value  5 is %d\n",value[5]);
	printf("Value  6 is %d\n",value[6]);
	printf("Value  7 is %d\n",value[7]);




	write(s,&frame,sizeof(struct can_frame));

}




