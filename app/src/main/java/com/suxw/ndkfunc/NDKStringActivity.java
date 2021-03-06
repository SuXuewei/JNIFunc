package com.suxw.ndkfunc;

import android.os.Bundle;
import android.os.PersistableBundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import com.suxw.ndkfunc.util.SerialPort;
import com.suxw.ndkfunc.util.StringUtil;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

/**
 * @ClassName: NDKStringActivity
 * @Description: Test NDK String
 * @Author: able
 * @Mail: g_su_xw@163.com
 * @Date: 2021/1/18
 */
public class NDKStringActivity extends AppCompatActivity {

    EditText etJavaString;
    TextView tvTestInfo;
    SerialPort serialPort;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ndk_string);

        Button btnStartTest = (Button)findViewById(R.id.btnTestNdkString);
        Button btnClear = (Button)findViewById(R.id.btnClear);
        etJavaString = (EditText)findViewById(R.id.etJavaString);
        tvTestInfo = (TextView)findViewById(R.id.tvNdkStringInfo);

        serialPort = new SerialPort();

        btnStartTest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                StringUtil stringUtil = new StringUtil();
                String strInput = etJavaString.getText().toString();
                String strOutput = stringUtil.getOutString(strInput);
                //testSerialPort();
                String strTestInfo = buildTestInfo(strInput, strOutput);
                String info = tvTestInfo.getText().toString();
                info += strTestInfo;
                tvTestInfo.setText(info);
            }
        });
        btnClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                etJavaString.setText("");
                tvTestInfo.setText("");
            }
        });
    }

    String buildTestInfo(String strInput, String strOutput)
    {
        String strTestInfo;
        strTestInfo = "Input String：" + strInput + "\n" +
                      "Output String: \n" + strOutput + "\n";
        return strTestInfo;
    }

    void testSerialPort()
    {
        new Thread(new Runnable() {
            @Override
            public void run() {
                if(serialPort != null) {
                    serialPort.testSerialPort();
                }
            }
        }).start();
    }
}
