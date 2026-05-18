package com.example.vedanshu.homeautomation;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.Toast;
import java.text.SimpleDateFormat;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Calendar;

public class Menu1 extends AppCompatActivity {
    private Button BT1, BT2, BT3, BT4,BT5;
    int a=1,b=1,c=1,d=1;
    private CheckBox checkBox1, checkBox2, checkBox3,checkBox4;
    private RadioGroup radiogroup;
    private RadioButton radioButton;
    private Spinner spinner;
    private String choice[] = new String[]{"Select","2 Minutes","2 Hours", "3 Hours", "4 Hours"};
    private ArrayAdapter<String> adapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu1);
        BT1 = findViewById(R.id.button);
        BT2 = findViewById(R.id.button2);
        BT3 = findViewById(R.id.button3);
        BT4 = findViewById(R.id.button4);
        BT5 = findViewById(R.id.button5);
        BT1.setText("OFF");
        BT2.setText("OFF");
        BT3.setText("OFF");
        BT4.setText("OFF");
        checkBox1 = (CheckBox) findViewById(R.id.chech1);
        checkBox2 = (CheckBox) findViewById(R.id.chech2);
        checkBox3 = (CheckBox) findViewById(R.id.chech3);
        checkBox4 = (CheckBox) findViewById(R.id.chech4);
        radiogroup = findViewById(R.id.radioGroup);
        spinner = (Spinner) findViewById(R.id.choice);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item,choice);
        spinner.setAdapter(adapter);
        if(getSupportActionBar()!=null){
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowHomeEnabled(true);
        }
        BT1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //  Log.d("Hello","Hi");
                if(a==0) {
                    sendMessage("11", 5);
                    BT1.setText("Off");
                    a=1;
                }
                else if(a==1){
                    sendMessage("55",5);
                    BT1.setText("On");
                    a=0;
                }
            }
        });
        BT2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(b==0) {
                    sendMessage("15", 5);
                    BT2.setText("Off");
                    b=1;
                }
                else if(b==1){
                    sendMessage("60",5);
                    BT2.setText("On");
                    b=0;
                }
            }
        });
        BT3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(c==0) {
                    sendMessage("44", 5);
                    BT3.setText("Off");
                    c=1;
                }
                else if(c==1){
                    sendMessage("77",5);
                    BT3.setText("On");
                    c=0;
                }
            }
        });
        BT4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(d==0) {
                    sendMessage("95", 5);
                    BT4.setText("Off");
                    d=1;
                }
                else if(d==1){
                    sendMessage("88",5);
                    BT4.setText("On");
                    d=0;
                }
            }
        });
        BT5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int selectedId = radiogroup.getCheckedRadioButtonId();
                radioButton = (RadioButton) findViewById(selectedId);
                if (radioButton.getText().equals("ON")){
                    if(spinner.getSelectedItem().equals("Select")){
                        Toast.makeText(Menu1.this,
                                "Please Select Hours", Toast.LENGTH_SHORT).show();
                    }
                    else if(spinner.getSelectedItem().equals("2 Hours")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("HH");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+2;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("mmss");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            Str=Str+b+strDate2;
                            int c=count*10;
                            String sp="20"+Integer.toString(c);
                            Str=Str+sp+"66";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);
                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                    else if(spinner.getSelectedItem().equals("2 Minutes")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("mm");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+2;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("HH");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            SimpleDateFormat mdformat2 = new SimpleDateFormat("ss");
                            String strDate3 = mdformat2.format(calendar.getTime());
                            Str=Str+strDate2+b+strDate3;
                            int c=count*10;
                            String sp="30"+Integer.toString(c);
                            Str=Str+sp+"66";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);
                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                    else if(spinner.getSelectedItem().equals("3 Hours")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("HH");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+3;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("mmss");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            Str=Str+b+strDate2;
                            int c=count*10;
                            String sp="30"+Integer.toString(c);
                            Str=Str+sp+"66";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);
                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                    else if(spinner.getSelectedItem().equals("4 Hours")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("HH");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+4;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("mmss");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            Str=Str+b+strDate2;
                            int c=count*10;
                            String sp="40"+Integer.toString(c);
                            Str=Str+sp+"66";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);

                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                }
                else if (radioButton.getText().equals("OFF")){
                    if(spinner.getSelectedItem().equals("Select")){
                        Toast.makeText(Menu1.this,
                                "Please Select Hours", Toast.LENGTH_SHORT).show();
                    }
                    else if(spinner.getSelectedItem().equals("2 Hours")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("HH");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+2;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("mmss");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            Str=Str+b+strDate2;
                            int c=count*10;
                            String sp="20"+Integer.toString(c);
                            Str=Str+sp+"99";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);
                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                    else if(spinner.getSelectedItem().equals("2 Minutes")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("mm");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+2;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("HH");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            SimpleDateFormat mdformat2 = new SimpleDateFormat("ss");
                            String strDate3 = mdformat2.format(calendar.getTime());
                            Str=Str+strDate2+b+strDate3;
                            int c=count*10;
                            String sp="30"+Integer.toString(c);
                            Str=Str+sp+"99";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);
                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                    else if(spinner.getSelectedItem().equals("3 Hours")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("HH");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+3;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("mmss");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            Str=Str+b+strDate2;
                            int c=count*10;
                            String sp="30"+Integer.toString(c);
                            Str=Str+sp+"99";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);
                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                    else if(spinner.getSelectedItem().equals("4 Hours")){
                        if(checkBox1.isChecked() || checkBox2.isChecked() ||
                                checkBox3.isChecked() || checkBox4.isChecked()){
                            String Str=new String();
                            int count=0;
                            if(checkBox1.isChecked()){
                                Str="1";
                                count++;
                            }
                            else
                            {
                                Str="0";
                            }
                            if(checkBox2.isChecked()){
                                Str=Str+"2";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox3.isChecked()){
                                Str=Str+"3";
                                count++;
                            }
                            else
                            {
                                Str=Str+"0";
                            }
                            if(checkBox4.isChecked()){
                                Str=Str+"4";
                                count++;
                            }
                            else {
                                Str = Str + "0";
                            }

                            Calendar calendar = Calendar.getInstance();
                            SimpleDateFormat mdformat = new SimpleDateFormat("HH");
                            String strDate1 = mdformat.format(calendar.getTime());
                            int a=Integer.parseInt(strDate1);
                            a=a+4;
                            String b=Integer.toString(a);
                            SimpleDateFormat mdformat1 = new SimpleDateFormat("mmss");
                            String strDate2 = mdformat1.format(calendar.getTime());
                            Str=Str+b+strDate2;
                            int c=count*10;
                            String sp="40"+Integer.toString(c);
                            Str=Str+sp+"99";
                            Log.d("Numbers",Str);
                            sendMessage(Str,5);

                        }
                        else
                        {
                            Toast.makeText(Menu1.this,
                                    "Please Select that you want to off",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                }
            }
        });
        checkBox1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(Menu1.this,
                        "CheckBox 1 selected", Toast.LENGTH_SHORT).show();
            }
        });
        checkBox2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(Menu1.this,
                        "CheckBox 2 selected", Toast.LENGTH_SHORT).show();
            }
        });
        checkBox3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(Menu1.this,
                        "CheckBox 3 selected", Toast.LENGTH_SHORT).show();
            }
        });
        checkBox4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(Menu1.this,
                        "CheckBox 4 selected", Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if(item.getItemId()==android.R.id.home){
            finish();
        }
        return super.onOptionsItemSelected(item);
    }

    private void sendMessage(final String msg, final int count) {
        //   final Handler handler = new Handler();
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {

                try {
                    Socket s = new Socket("192.168.4.1", 333);
                    Thread.sleep(100);
                    OutputStream out = s.getOutputStream();
                    PrintWriter output = new PrintWriter(out);
                    Thread.sleep(100);
                    output.println(msg);
                    Thread.sleep(100);
                    output.flush();
                    output.close();
                    out.close();
                    s.close();
                    for(int i=0;i<count;i++)
                    {
                        Thread.sleep(100);
                        s = new Socket("192.168.4.1", 333);
                        Thread.sleep(100);
                        out = s.getOutputStream();
                        output = new PrintWriter(out);
                        Thread.sleep(100);
                        output.println(msg);
                        Thread.sleep(100);
                        output.flush();
                        output.close();
                        out.close();
                        s.close();
                    }
                } catch (UnknownHostException e) {
                    //e.printStackTrace();
                    Log.d("E", "Exception1");
                } catch (IOException e) {
                    //e.printStackTrace();
                    Log.d("E", "Exception2");
                } catch (InterruptedException e) {
                    //e.printStackTrace();
                    Log.d("E", "Exception3");
                } catch (Exception e) {
                    //e.printStackTrace();
                    Log.d("E", "Exception4");
                }
            }
        });
        thread.start();
    }
}
