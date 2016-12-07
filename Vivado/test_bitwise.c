#include <stdio.h>

#define NODE_ID_WIFI	0x1	// Bottom, 		0b0001
#define NODE_ID_IMU	0x3	// Middle bottom	0b0011
#define NODE_ID_SEV	0x7	// Middle top		0b0111
#define NODE_ID_GPS	0xE	// Top			0b1110

#define PRIOR_BIT		0x0
#define NODE_ID			NODE_ID_GPS
#define MSG_TYPE		0x2B

#define SHIFT_PRIOBIT		0x4
#define SHIFT_NODEID		0x6

#define SUBSCRIPTIONS		5
#define SUBSCRIBED		1
#define NOTSUBSCRIBED		0

struct ProtocolData {
  int prioritybit;
  int nodeID;
  int msgtype;
  int last2bits;
};


int subscriptions[SUBSCRIPTIONS] = {0xeb,0xec,0xad,0x1d,0x3f};

int createProtocolID(struct ProtocolData pData){
  int msgID;
  
  msgID = (pData.prioritybit)<<SHIFT_PRIOBIT | (pData.nodeID);
  msgID = msgID<<SHIFT_NODEID | (pData.msgtype);
  
  return msgID;
}


int getMessageID(struct ProtocolData pData){
    return (pData.nodeID<<0x4) | pData.msgtype;
}

int amISubscribed(int msgid){
  int i = 0;
  
  for( i=0 ; i<SUBSCRIPTIONS ; i++){
    if( subscriptions[i] == msgid )
      return SUBSCRIBED;
  }
  return NOTSUBSCRIBED;
}

int main(){
  
  struct ProtocolData pData;
  int msgID;
  
  
  pData.prioritybit = PRIOR_BIT;
  pData.nodeID = NODE_ID;
  pData.msgtype = MSG_TYPE;
  
  printf("Example:\n\tprioritybit: %x\n\tnodeID: %x\n\tmsgtype: %x\n\n", PRIOR_BIT, NODE_ID, MSG_TYPE);
  
  msgID = createProtocolID(pData);
  
  printf("Created protocol ID: %x\n\n", msgID);
   
  printf("Decoding from msgID %x:\n\tprioritybit: %x\n\tsenderID: %x\n\tmsgtype: %x\n\n", msgID, getPriorityBit(msgID), getSenderID(msgID), getMsgType(msgID));
  
  printf("The message ID (nodeID and msgtype) is: %x, subscribed: %d\n\n", getMessageID(pData), amISubscribed(getMessageID(pData)));
  return 0;
}


int getPriorityBit(int msgid){
	return ( (msgid >> 0xa) & 0x1 );
}
int getSenderID(int msgid){
	return ( (msgid >> 0x6) & 0xf );
}
int getMsgType(int msgid){
	return ( msgid & 0x3f );
}