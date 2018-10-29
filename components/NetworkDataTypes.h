#ifndef NETWORK_DATA_TYPES_H
#define NETWORK_DATA_TYPES_H

#include <iostream>

struct JoinMessage {
	uint32_t type; //1
	uint32_t process_id;
};

struct ReqMessage {
	uint32_t type; //2
	uint32_t req_id;
	uint32_t view_id;
	uint32_t op_type;
	uint32_t peer_id;
};

struct OkMessage {
	uint32_t type; //3
	uint32_t req_id;
	uint32_t view_id;
};

struct NewViewMessage {
	uint32_t type; //4
	uint32_t view_id;
	uint32_t peer_list;
};

struct NetworkMessage {
	uint32_t msg_type;
	uint32_t id1;
	uint32_t id2;
	uint32_t id3;
	uint32_t id4;
};

//OP type
//1 - ADD
//2 - DEL
//3 - PENDING
//4 - NOTHING

#endif
