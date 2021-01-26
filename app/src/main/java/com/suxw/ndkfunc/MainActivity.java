package com.suxw.ndkfunc;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class MainActivity extends AppCompatActivity {

    private String[] funcNames = {"NDK String", "Serial Port", "TODO"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ArrayAdapter<String> funcListViewAdapter
                = new ArrayAdapter<String>(MainActivity.this,
                R.layout.func_list_item, R.id.tvFuncName, funcNames);
        ListView listView = (ListView) findViewById(R.id.lvFunc);
        listView.setAdapter(funcListViewAdapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                processOnItemClick(position);
            }
        });
    }

    //元素点击那妞
    boolean processOnItemClick(int position)
    {
        Intent intent;
        switch (position)
        {
            case 0:
                intent = new Intent(MainActivity.this,
                        NDKStringActivity.class);
                startActivityForResult(intent, 1);
                break;
            case 1:
                intent = new Intent(MainActivity.this,
                        SerialPortActivity.class);
                startActivityForResult(intent, 1);
                break;
            default:
                break;
        }
        return true;
    }
}
