//
// Created by bingy on 2021/3/14.
//

#include "DZJNICall.h"
#include "DZConstDefine.h"

DZJNICall::DZJNICall(JavaVM *javaVM, JNIEnv *jniEnv, jobject obj) {
    this->javaVM = javaVM;
    this->jniEnv = jniEnv;
    //NewGlobalRef, so it can be used in child thread
    this->jLivePlayObj = jniEnv->NewGlobalRef(obj);

    jclass playerClz = jniEnv->GetObjectClass(jLivePlayObj);
    jLivePlayOnErrorMid = jniEnv->GetMethodID(playerClz, "onConnectError", "(ILjava/lang/String;)V");
    jLivePlayOnSuccessMid = jniEnv->GetMethodID(playerClz, "onSuccess", "()V");
}

DZJNICall::~DZJNICall() {
    jniEnv->DeleteGlobalRef(jLivePlayObj);
}

void DZJNICall::onConnectError(ThreadMode threadMode, int code, char *msg) {
    LOGE("onConnectError code %d, msg %s ", code, msg);
    if(threadMode == THREAD_MAIN){
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jLivePlayObj, jLivePlayOnErrorMid, code, jmsg);
        jniEnv->DeleteLocalRef(jmsg);
    } else if(threadMode == THREAD_CHILD){
        //获取当前线程的JNIEnv，通过JavaVM
        JNIEnv *env;
        if(javaVM->AttachCurrentThread(&env, 0) != JNI_OK){
            LOGE("get child thread jniEnv error!!");
            return;
        }
        jstring jmsg = env->NewStringUTF(msg);
        env->CallVoidMethod(jLivePlayObj, jLivePlayOnErrorMid, code, jmsg);
        env->DeleteLocalRef(jmsg);
        javaVM->DetachCurrentThread();
    }
}

void DZJNICall::onSuccess(ThreadMode threadMode) {
    LOGE("onSuccess ");
    if(threadMode == THREAD_MAIN){
        jniEnv->CallVoidMethod(jLivePlayObj, jLivePlayOnSuccessMid);
    } else if(threadMode == THREAD_CHILD){
        //获取当前线程的JNIEnv，通过JavaVM
        JNIEnv *env;
        if(javaVM->AttachCurrentThread(&env, 0) != JNI_OK){
            LOGE("get child thread jniEnv error!!");
            return;
        }
        env->CallVoidMethod(jLivePlayObj, jLivePlayOnSuccessMid);
        javaVM->DetachCurrentThread();
    }
}

//void DZJNICall::onConnectError(ThreadMode threadMode) {
//    LOGE("onPrepared");
//    if(threadMode == THREAD_MAIN){
//        jniEnv->CallVoidMethod(jLivePlayObj, jDarrenPlayOnPreparedMid);
//    } else if(threadMode == THREAD_CHILD){
//        //获取当前线程的JNIEnv，通过JavaVM
//        JNIEnv *env;
//        if(javaVM->AttachCurrentThread(&env, 0) != JNI_OK){
//            LOGE("get child thread jniEnv error!!");
//            return;
//        }
//        env->CallVoidMethod(jLivePlayObj, jDarrenPlayOnPreparedMid);
//        javaVM->DetachCurrentThread();
//    }
//}

