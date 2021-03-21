//
// Created by bingy on 2021/3/20.
//


#include <cstring>
#include "DZLivePush.h"

DZLivePush::DZLivePush(const char *liveUrl, DZJNICall *pJniCall) {
    this->pJniCall = pJniCall;
    this->liveUrl = (char *)malloc(strlen(liveUrl) + 1);
    strcpy(this->liveUrl, liveUrl);
//    memcpy(this->liveUrl, liveUrl, strlen(liveUrl) + 1);
    pRTMPPacketQueue = new DZRTMPPacketQueue();
}

DZLivePush::~DZLivePush() {
    if(this->liveUrl != NULL){
        free(liveUrl);
        liveUrl = NULL;
    }

    if(pRTMPPacketQueue != NULL){
        delete pRTMPPacketQueue;
        pRTMPPacketQueue = NULL;
    }

    if(rtmp != NULL){
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
        rtmp = NULL;
    }
}

void *initConnectFun(void *context){
    DZLivePush *pLivePush = (DZLivePush *) context;
    //1.创建RTMP
    pLivePush->rtmp = RTMP_Alloc();
    //2.初始化
    RTMP_Init(pLivePush->rtmp);
    //3. 设置参数，连接超时时间等
    pLivePush->rtmp->Link.timeout = 10; //10s
    pLivePush->rtmp->Link.lFlags |= RTMP_LF_LIVE; //直播
    RTMP_SetupURL(pLivePush->rtmp, pLivePush->liveUrl);
    RTMP_EnableWrite(pLivePush->rtmp);
    //开始连接
    if(!RTMP_Connect(pLivePush->rtmp, NULL)){
        //回调到Java层，连接异常
        LOGE("rtmp connect error.");
        pLivePush->pJniCall->onConnectError(THREAD_CHILD, INIT_RTMP_CONNECT_ERROR_CODE, "rtmp connect error.");
        return (void *)(INIT_RTMP_CONNECT_ERROR_CODE);
    }

    if(!RTMP_ConnectStream(pLivePush->rtmp, 0)){
        //回调到Java层，连接异常
        LOGE("rtmp connect stream error.");
        pLivePush->pJniCall->onConnectError(THREAD_CHILD, INIT_RTMP_CONNECT_STREAM_ERROR_CODE, "rtmp connect stream error.");
        return (void *)(INIT_RTMP_CONNECT_STREAM_ERROR_CODE);
    }

    LOGE("rtmp connect success.");
    pLivePush->pJniCall->onSuccess(THREAD_CHILD);
    return 0;
}

void DZLivePush::initConnect() {
    pthread_t initConnectTid;
    pthread_create(&initConnectTid, NULL, initConnectFun, this);
    pthread_detach(initConnectTid);
}
