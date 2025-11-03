package com.pedro.streamer.screen;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import androidx.appcompat.app.AppCompatActivity;
import com.pedro.rtplibrary.rtmp.RtmpDisplay;
import com.pedro.streamer.R;

public class ScreenActivity extends AppCompatActivity implements View.OnClickListener {

  private Button bStartStop;
  private EditText etRtmpUrl;
  private ScreenService screenService;

  private ServiceConnection serviceConnection = new ServiceConnection() {
    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
      screenService = ((ScreenService.ScreenServiceBinder) service).getService();
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
      screenService = null;
    }
  };

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_screen);
    bStartStop = findViewById(R.id.b_start_stop);
    etRtmpUrl = findViewById(R.id.et_rtmp_url);
    bStartStop.setOnClickListener(this);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
      startForegroundService(new Intent(this, ScreenService.class));
    } else {
      startService(new Intent(this, ScreenService.class));
    }
    bindService(new Intent(this, ScreenService.class), serviceConnection, Context.BIND_AUTO_CREATE);
  }

  @Override
  public void onClick(View view) {
    if (view.getId() == R.id.b_start_stop) {
      if (bStartStop.getText().toString().equals("Start Stream")) {
        startActivityForResult(RtmpDisplay.getMediaProjectionIntent(this), 1);
        bStartStop.setText("Stop Stream");
      } else {
        if(screenService != null) screenService.stopStream();
        bStartStop.setText("Start Stream");
      }
    }
  }

  @Override
  protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    super.onActivityResult(requestCode, resultCode, data);
    if (requestCode == 1 && resultCode == RESULT_OK) {
      if(screenService != null) {
        String url = etRtmpUrl.getText().toString();
        screenService.setUrl(url);
        screenService.startStream(resultCode, data);
      }
    }
  }
}