//
// Created by bingy on 2021/3/20.
//

#ifndef LIVEPUSH_DZLIVEPUSH_H
#define LIVEPUSH_DZLIVEPUSH_H

#include "DZJNICall.h"
#include <malloc.h>
#include "DZRTMPPacketQueue.h"
#include "DZConstDefine.h"

class DZLivePush{
public:
    DZJNICall *pJniCall = NULL;
    char *liveUrl = NULL;
    DZRTMPPacketQueue *pRTMPPacketQueue = NULL;
    RTMP *rtmp = NULL;

public:
    DZLivePush(const char* liveUrl, DZJNICall *pJniCall);
    ~DZLivePush();

    void initConnect();
};

#endif //LIVEPUSH_DZLIVEPUSH_H
