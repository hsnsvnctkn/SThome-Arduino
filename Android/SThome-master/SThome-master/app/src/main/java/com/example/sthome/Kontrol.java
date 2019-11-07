package com.example.sthome;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;

public class Kontrol extends AppCompatActivity {
    String adres;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_kontrol);

        final Button btnBack = (Button) this.findViewById(R.id.btnBack);
        final EditText inputIp = (EditText) this.findViewById(R.id.txtIp);
        final Button btnGo =(Button) this.findViewById(R.id.btnGo);
        final WebView web =(WebView) this.findViewById(R.id.web);

        btnBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        btnGo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                adres ="http://" + inputIp.getText().toString();
                WebSettings webSettings=web.getSettings();
                webSettings.setJavaScriptEnabled(true);
                web.setWebViewClient(new WebViewClient());
                web.loadUrl(adres);
            }
        });

    }
}
