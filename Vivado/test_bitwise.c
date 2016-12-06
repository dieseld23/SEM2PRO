#include <stdio.h>


#define NODE_ID_WIFI	0x1	// Bottom, 		0b0001
#define NODE_ID_IMU	0x3	// Middle bottom	0b0011
#define NODE_ID_SEV	0x7	// Middle top		0b0111
#define NODE_ID_GPS	0xE	// Top			0b1110

#define PRIOR_BIT		0x0
#define NODE_ID			NODE_ID_GPS
#define MSG_TYPE		0xB
#define LAST2BITS		0x3

#define SHIFT_PRIOBIT		0x4
#define SHIFT_NODEID		0x4
#define SHIFT_MSGTYPE		0x2

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
  int protocolID;
  
  protocolID = (pData.prioritybit)<<SHIFT_PRIOBIT | (pData.nodeID);
  protocolID = protocolID<<SHIFT_NODEID | (pData.msgtype);
  protocolID = protocolID<<SHIFT_MSGTYPE | (pData.last2bits);
  
  return protocolID;
}

struct ProtocolData decodeProtocolID(int protocolID){
  struct ProtocolData decodedData;
  int pID;
  pID = protocolID;
  
  decodedData.last2bits = pID & 0x3;
  
  pID = pID>>SHIFT_MSGTYPE;
  decodedData.msgtype = pID & 0xF;
  
  pID = pID>>SHIFT_NODEID;
  decodedData.nodeID = pID & 0xF;
  
  decodedData.prioritybit = pID>>SHIFT_PRIOBIT;  
  
  return decodedData;
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
  int protocolID;
  
  
  pData.prioritybit = PRIOR_BIT;
  pData.nodeID = NODE_ID;
  pData.msgtype = MSG_TYPE;
  pData.last2bits = LAST2BITS;
  
  printf("Example:\n\tprioritybit: %x\n\tnodeID: %x\n\tmsgtype: %x\n\tlast2bits: %x\n\n", PRIOR_BIT, NODE_ID, MSG_TYPE, LAST2BITS);
  
  protocolID = createProtocolID(pData);
  
  printf("Created protocol ID: %x\n\n", protocolID);
  
  pData = decodeProtocolID(protocolID);  
  printf("Decoding from protocolID %x:\n\tprioritybit: %x\n\tnodeID: %x\n\tmsgtype: %x\n\tlast2bits: %x\n\n", protocolID, pData.prioritybit, pData.nodeID, pData.msgtype, pData.last2bits);
  
  printf("The message ID (nodeID and msgtype) is: %x, subscribed: %d\n\n", getMessageID(pData), amISubscribed(getMessageID(pData)));
  return 0;
}