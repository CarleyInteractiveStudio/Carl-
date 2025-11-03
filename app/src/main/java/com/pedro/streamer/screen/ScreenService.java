package com.pedro.streamer.screen;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;
import androidx.annotation.Nullable;
import androidx.core.app.NotificationCompat;
import com.pedro.rtplibrary.rtmp.RtmpDisplay;
import com.pedro.streamer.R;
import net.ossrs.rtmp.ConnectCheckerRtmp;

public class ScreenService extends Service implements ConnectCheckerRtmp {

    private static final String TAG = "ScreenService";
    private RtmpDisplay rtmpDisplay;
    private String url;
    private final IBinder binder = new ScreenServiceBinder();

    public class ScreenServiceBinder extends Binder {
        ScreenService getService() {
            return ScreenService.this;
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel channel = new NotificationChannel("my_channel", "My Channel", NotificationManager.IMPORTANCE_DEFAULT);
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
        rtmpDisplay = new RtmpDisplay(this, true, this);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Notification notification = new NotificationCompat.Builder(this, "my_channel")
            .setContentTitle("Streaming")
            .setContentText("Streaming in progress")
            .setSmallIcon(R.drawable.ic_launcher)
            .build();
        startForeground(1, notification);
        return START_STICKY;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public void startStream(int resultCode, Intent data) {
        if (!rtmpDisplay.isStreaming()) {
            if (rtmpDisplay.prepareAudio() && rtmpDisplay.prepareVideo()) {
                rtmpDisplay.startStream(url, resultCode, data);
            }
        }
    }

    public void stopStream() {
        if (rtmpDisplay.isStreaming()) {
            rtmpDisplay.stopStream();
        }
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    @Override
    public void onConnectionSuccessRtmp() {
        Log.i(TAG, "Connection success");
    }

    @Override
    public void onConnectionFailedRtmp(String reason) {
        Log.e(TAG, "Connection failed: " + reason);
        stopStream();
    }

    @Override
    public void onNewBitrateRtmp(long bitrate) {
    }

    @Override
    public void onDisconnectRtmp() {
        Log.i(TAG, "Disconnected");
    }

    @Override
    public void onAuthErrorRtmp() {
        Log.e(TAG, "Auth error");
    }

    @Override
    public void onAuthSuccessRtmp() {
        Log.i(TAG, "Auth success");
    }
}