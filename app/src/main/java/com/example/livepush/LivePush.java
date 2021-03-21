package com.example.livepush;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

public class LivePush {

    static {
        System.loadLibrary("live-push");
    }

    private Handler mainHandler = new Handler(Looper.getMainLooper());
    EventListener eventListener;

    public void setEventListener(EventListener eventListener) {
        this.eventListener = eventListener;
    }

    public void clearListener(){
        this.eventListener = null;
    }

    public void onConnectError(int code, String msg){
        Log.e("LivePush ", "onConnectError code = " + code + " msg " + msg);
        stop();

        if(eventListener != null){
            eventListener.onError(code, msg);
        }
    }

    public void stop(){
        mainHandler.post(()->{
            stop0(); //底层工作线程回调，切换到主线程，销毁JNI资源
        });
    }

    private native void stop0();

    public void onSuccess(){
        Log.e("LivePush onSuccess", "onSuccess.");
        if(eventListener != null){
            eventListener.onSuccess();
        }
    }

    private String liveUrl;
    public LivePush(String liveUrl){
        this.liveUrl = liveUrl;
    }

    //初始化连接
    public void initConnect(){
        initConnect0(liveUrl);
    }

    //回调

    private native void initConnect0(String liveUrl);

}
