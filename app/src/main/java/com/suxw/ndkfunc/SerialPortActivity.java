package com.suxw.ndkfunc;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.suxw.ndkfunc.util.SerialPort;

public class SerialPortActivity extends AppCompatActivity {

    EditText etInput;
    EditText etOutput;
    TextView tvTipInfo;
    SerialPort serialPort;
    Handler handler;

    static final int MSG_UPDATE_INFO = 1;
    static final int MSG_CALL_MIS = 2;
    static final String MSG_KEY_INFO ="info";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_serial_port);

        Button btnStart = (Button)findViewById(R.id.btnSerialPortStart);
        Button btnClear = (Button)findViewById(R.id.btnSerialPortClear);
        etInput = (EditText)findViewById(R.id.etInput);
        etOutput = (EditText)findViewById(R.id.etOutput);
        tvTipInfo = (TextView)findViewById(R.id.tvSerialPortInfo);
        serialPort = new SerialPort();
        handler = new Handler() {
            @Override
            public void handleMessage(@NonNull Message msg) {
                super.handleMessage(msg);
                switch (msg.what)
                {
                    case MSG_UPDATE_INFO:
                        String info = msg.getData().getString(MSG_KEY_INFO);
                        String orgInfo = tvTipInfo.getText().toString();
                        info += "\n";
                        info += orgInfo;
                        tvTipInfo.setText(info);
                        break;
                    case MSG_CALL_MIS:
                        break;
                    default:
                        break;
                }
            }
        };

        btnClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                etInput.setText("");
                etOutput.setText("");
                tvTipInfo.setText("");
            }
        });
        btnStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                testSerialPort();
            }
        });
    }

    void testSerialPort()
    {
        new Thread(new Runnable() {
            @Override
            public void run() {
                displayInfo("开始测试串口");
                serialPort.testSerialPort();
                displayInfo("测试串口结束");
            }
        }).start();
    }

    void displayInfo(String info)
    {
        Message msg = new Message();
        msg.what = MSG_UPDATE_INFO;
        Bundle bundle = new Bundle();
        bundle.putString(MSG_KEY_INFO, info);
        msg.setData(bundle);
        handler.sendMessage(msg);
    }
}
