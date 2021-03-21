//
// Created by bingy on 2021/3/17.
//

#ifndef INC_011_FFMPEG_MASTER_DZAVPACKETQUEUE_H
#define INC_011_FFMPEG_MASTER_DZAVPACKETQUEUE_H

#include <queue>
#include <pthread.h>

extern "C" {
#include "librtmp/rtmp.h"
};


using namespace std;

class DZRTMPPacketQueue{
private:
    std::queue<RTMPPacket *> *pPacketQueue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;


public:
    DZRTMPPacketQueue();
    ~DZRTMPPacketQueue();

    void push(RTMPPacket *packet);
    RTMPPacket *pop();
    void clear();
};

#endif //INC_011_FFMPEG_MASTER_DZAVPACKETQUEUE_H
