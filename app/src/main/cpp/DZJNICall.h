//
// Created by bingy on 2021/3/14.
//

#ifndef INC_011_FFMPEG_MASTER_DZJNICALL_H
#define INC_011_FFMPEG_MASTER_DZJNICALL_H

#include <jni.h>

enum ThreadMode{
    THREAD_CHILD,THREAD_MAIN
};

class DZJNICall{
public:
    JavaVM *javaVM;
    JNIEnv *jniEnv;
    jobject jLivePlayObj;
    jmethodID jLivePlayOnErrorMid;
    jmethodID jLivePlayOnSuccessMid;
    void onConnectError(ThreadMode threadMode, int code, char *msg);
    void onSuccess(ThreadMode threadMode);

public:
    DZJNICall(JavaVM *javaVM, JNIEnv *jniEnv, jobject obj);
    ~DZJNICall();
//    void onConnectError(ThreadMode threadMode);

private:

};

#endif //INC_011_FFMPEG_MASTER_DZJNICALL_H
