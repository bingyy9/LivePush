//
// Created by bingy on 2021/3/17.
//

#include "DZRTMPPacketQueue.h"

void DZRTMPPacketQueue::clear() {
    //清楚队列，和每个AVPacket*的内存数据
    if(pPacketQueue == NULL){
        return;
    }

    pthread_mutex_lock(&mutex);
    while(!pPacketQueue->empty()) {
        RTMPPacket *packet = pPacketQueue->front();
        pPacketQueue->pop();
        RTMPPacket_Free(packet);
        free(packet);
        packet = NULL;
    }
    pthread_mutex_unlock(&mutex);
}

DZRTMPPacketQueue::DZRTMPPacketQueue() {
    pPacketQueue = new std::queue<RTMPPacket *>();
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

DZRTMPPacketQueue::~DZRTMPPacketQueue() {
    if(pPacketQueue != NULL){
        clear();
        delete pPacketQueue;
        pPacketQueue = NULL;
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void DZRTMPPacketQueue::push(RTMPPacket *packet) {
    if(pPacketQueue == NULL){
        return;
    }

    pthread_mutex_lock(&mutex);
    pPacketQueue->push(packet);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

RTMPPacket *DZRTMPPacketQueue::pop() {
    if(pPacketQueue == NULL){
        return NULL;
    }

    pthread_mutex_lock(&mutex);
    while(pPacketQueue->empty()){
        pthread_cond_wait(&cond, &mutex);
    }
    RTMPPacket *packet = pPacketQueue->front();
    pPacketQueue->pop();
    pthread_mutex_unlock(&mutex);
    return packet;
}
