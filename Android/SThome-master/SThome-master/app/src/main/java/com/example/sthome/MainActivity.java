package com.example.sthome;

import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import com.example.sthome.Service.BekServis;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    public static final String TAG_INTENT_SERVICE = "TAG_INTENT_SERVICE";


    TextView lblGaz;
    TextView lblSicaklik;
    TextView lblTnem;
    TextView lblhirsizDurum;

    String gaz,sicaklik,toprakNem,hirsizDurum;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Intent intent = new Intent(this, BekServis.class);
        startService(intent);

        Button btnKontrol = (Button) this.findViewById(R.id.btnKontrol);
        final Intent newWindow = new Intent(getBaseContext(),Kontrol.class);
        lblGaz= (TextView) this.findViewById(R.id.lblGaz);
        lblSicaklik=(TextView) this.findViewById(R.id.lblSicaklik);
        lblTnem=(TextView) this.findViewById(R.id.lblTnem);
        lblhirsizDurum=(TextView) this.findViewById(R.id.lblhirsizDurum);
        Button btnUpdate = (Button) this.findViewById(R.id.btnUpdate);

        Timer timer = new Timer();
        timer.schedule(new TimerTask()
        {
            @Override
            public void run()
            {
                updateData();
            }

        }, 0, 1000 * 10); // Update every 10 seconds

        btnUpdate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new pullData().execute();
            }
        });

        btnKontrol.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(newWindow);
            }
        });
    }
    public class pullData extends AsyncTask<Void,Void,Void>{  //Linkteki verileri çekip değişkene atma

        @Override
        protected Void doInBackground(Void... voids) {
            updateData();
            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);

            lblSicaklik.setText(sicaklik);
            lblGaz.setText(gaz);
            lblTnem.setText(toprakNem);
            if(Integer.valueOf(hirsizDurum) == 1) {
                lblhirsizDurum.setText("Tehlike Olabilir !!");
            }
            else {
                lblhirsizDurum.setText("Tehlike Yok");
            }

        }
    }

    private void updateData()
    {
        Log.w("Activity - GAZLA", "gazlaniyor");

        try {
            Document doc = Jsoup.connect("https://thingspeak.com/channels/763927/field/1/last.html").get();
            Document doc2 = Jsoup.connect("https://thingspeak.com/channels/763927/field/2/last.html").get();
            Document doc3 = Jsoup.connect("https://thingspeak.com/channels/763927/field/3/last.html").get();
            Document doc4 = Jsoup.connect("https://thingspeak.com/channels/763927/field/4/last.html").get();

            gaz = doc3.text();
            sicaklik = doc.text();
            toprakNem = doc2.text();
            hirsizDurum = doc4.text();

            Handler mainHandler = new Handler(getMainLooper());

            Runnable myRunnable = new Runnable() {
                @Override
                public void run() {
                    lblSicaklik.setText(sicaklik);
                    if(Integer.valueOf(hirsizDurum) == 1) {
                        lblhirsizDurum.setText("Tehlike Olabilir !!");
                    }
                    else {
                        lblhirsizDurum.setText("Tehlike Yok");
                    }
                    lblTnem.setText(toprakNem);
                    lblGaz.setText(gaz);

                } // This is your code
            };
            mainHandler.post(myRunnable);


        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}

