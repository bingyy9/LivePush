package com.example.livepush;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements EventListener{

    private LivePush livePush;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());

//        LivePush livePush = new LivePush("175.27.222.97");
        livePush = new LivePush("rtmp://148.70.96.230/myapp/mystream");
        livePush.initConnect();
        livePush.setEventListener(this);
    }

    @Override
    public void onError(int code, String msg) {
        Log.e("MainActivity", "onError");
    }

    @Override
    public void onSuccess() {
        Log.e("MainActivity", "onSuccess");
        //采集数据

        //发送数据

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(livePush != null){
            livePush.clearListener();
            livePush.stop();
        }
    }
}