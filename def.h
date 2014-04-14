#ifndef DEF_H
#define DEF_H

//#define OPTION_ALLOW_MIGRATION

#define MAX_REQUESTS 15000

#define MAX_REQ_NODES 1000
#define MAX_REQ_LINKS 100
//100

#define MAX_REQ_PER_NODE 1000
#define MAX_REQ_PER_LINK 1000 //15000

#define MAX_SUB_NODES 450
#define MAX_SUB_LINKS 450

#define LEN_FILENAME 300

#define MAX_CPU 100
#define MAX_BW 100

#define MAX_NECK_COUNT 1000

#define TOPO_GENERAL 0
#define TOPO_STAR 1
#define TOPO_TREE 2
#define TOPO_HUBS 3
#define TOPO_FULL 4

#define STATE_MAP_NODE_FAIL -1  //tried but failed
#define STATE_NEW 0   //first state
#define STATE_MAP_NODE  1  //success in mapping node (only nodes have been mapped)
#define STATE_MAP_LINK  2  // links were successfully mapped too
#define STATE_DONE  3  //done
#define STATE_EXPIRE 4
#define STATE_MAP_LINK_FAIL  5 //nodes fine but links failed

#define STATE_RECEIVED 0   //has just arrived
#define STATE_RUNNING  1  //success in mapping node and links
#define STATE_WAITING  2  // waiting for free resources
#define STATE_EXPIRED 3  //after duration
#define STATE_ABORTED 4  //after duration
#define STATE_REJECTED 5  //after reaching max queueing delay

#define NO_MIGRATION 0
#define ROUTE_MIGRATION 1
//#define NODE_MIGRATION 2

#define LINK_SPLITTABLE 0
#define LINK_UNSPLITTABLE 1

#define TIMES_TRY 1

#define LEAVE_EVENT 0
#define EXPIRE_EVENT 1

#define POISSON 0
#define GOOGLE 1
#define WEB 2
#define XML 3

#define OFF 0
#define ON 1

#define QUEING_DELAY 60

#endif//DEF_H
