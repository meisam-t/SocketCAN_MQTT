
#ifndef CANSENDRECV_H 
#define CANSENDRECV_H

int can_init(void);
void can_recv(int s);
void can_send(int id, int serialnum, char *value);
int sock_canid(int sockid);

extern int sockID;
int s_id;

//void cansendStruct(struct candata data);

#endif


