package com.example.livepush;

public interface EventListener {
    void onError(int code, String msg);
    void onSuccess();
}
