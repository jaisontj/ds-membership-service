#ifndef NETWORK_DATA_TYPES_H
#define NETWORK_DATA_TYPES_H

#include <iostream>

#define JOIN 1
#define REQUEST 2
#define OK 3
#define NEW_VIEW 4
#define NEW_LEADER 5
#define NEW_LEADER_RESPONSE 6

#define OP_ADD 1
#define OP_DEL 2
#define OP_PENDING 3
#define OP_NOTHING 4


struct JoinMessage {
	uint32_t type; //1
	uint32_t process_id;
};

struct ReqMessage {
	uint32_t type; //2 or 6
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

struct NewLeaderMessage {
	uint32_t type; //5
	uint32_t req_id;
	uint32_t view_id;
	uint32_t op_type;
};

struct NetworkMessage {
	uint32_t msg_type;
	uint32_t id1;
	uint32_t id2;
	uint32_t id3;
	uint32_t id4;
};

#endif
