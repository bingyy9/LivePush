#include <jni.h>
#include <string>
#include "DZConstDefine.h"
#include "DZLivePush.h"
#include "DZJNICall.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_livepush_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JavaVM *pJavaVm = NULL;
DZLivePush *pLivePush = NULL;
DZJNICall *pJniCall = NULL;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *javaVm, void *reserved){
    LOGE("JNI_OnLoad=====");
    pJavaVm = javaVm;
    JNIEnv *env;
    if(pJavaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK){
        return -1;
    }
    return JNI_VERSION_1_4;
}


extern "C" JNIEXPORT void JNICALL Java_com_example_livepush_LivePush_initConnect0(JNIEnv *env, jobject thiz, jstring url) {
    pJniCall = new DZJNICall(pJavaVm, env, thiz);
    const char *c_url = env->GetStringUTFChars(url, NULL);
    pLivePush = new DZLivePush(c_url, pJniCall);
    pLivePush->initConnect();
    env->ReleaseStringUTFChars(url, c_url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_livepush_LivePush_stop0(JNIEnv *env, jobject thiz) {
    if(pLivePush != NULL){
        delete pLivePush;
        pLivePush = NULL;
    }

    if(pJniCall != NULL){
        delete pJniCall;
        pJniCall = NULL;
    }

}