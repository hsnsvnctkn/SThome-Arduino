package com.example.sthome.Service;

import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import com.example.sthome.MainActivity;
import com.example.sthome.R;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

public class BekServis extends Service
{
    public BekServis()
    {
        Log.e("OKKK", "OKKKKK");

    }

    @Override
    public IBinder onBind(Intent intent)
    {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {

        Timer timer = new Timer();
        timer.schedule(new TimerTask()
        {
            @Override
            public void run()
            {
                Integer gaz = gazVer();
                Integer hirsiz = hirsizDurumVer();
                if (gaz > 320)
                {
                    Intent intent = new Intent(BekServis.this, MainActivity.class);
                    showNotification(BekServis.this, "SThome", "Evde Gaz Kaçağı Olabilir !!",intent);
                }
                if (hirsiz == 1)
                {
                    Intent intent = new Intent(BekServis.this, MainActivity.class);
                    showNotification(BekServis.this, "SThome", "Evde Hırsız Olabilir !!",intent);
                }
            }

        }, 0, 1000 * 10); // Update every 10 seconds
        return super.onStartCommand(intent, flags, startId);

    }

    private void sendPush()
    {
        Intent intent = new Intent(this, MainActivity.class);
        PendingIntent contentIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);

        NotificationCompat.Builder b = new NotificationCompat.Builder(this);

        b.setAutoCancel(true)
                .setDefaults(Notification.DEFAULT_ALL)
                .setWhen(System.currentTimeMillis())
                .setSmallIcon(R.mipmap.ic_launcher)
                .setTicker("Hearty365")
                .setContentTitle("Default notification")
                .setContentText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.")
                .setDefaults(Notification.DEFAULT_LIGHTS| Notification.DEFAULT_SOUND)
                .setContentIntent(contentIntent)
                .setContentInfo("Info");


        NotificationManager notificationManager = (NotificationManager) this.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.notify(1, b.build());


    }

    public void showNotification(Context context, String title, String body, Intent intent) {
        NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

        int notificationId = 1;
        String channelId = "channel-01";
        String channelName = "Channel Name";
        int importance = NotificationManager.IMPORTANCE_HIGH;

        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
            NotificationChannel mChannel = new NotificationChannel(
                    channelId, channelName, importance);
            notificationManager.createNotificationChannel(mChannel);
        }

        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(context, channelId)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setContentTitle(title)
                .setContentText(body);

        TaskStackBuilder stackBuilder = TaskStackBuilder.create(context);
        stackBuilder.addNextIntent(intent);
        PendingIntent resultPendingIntent = stackBuilder.getPendingIntent(
                0,
                PendingIntent.FLAG_UPDATE_CURRENT
        );
        mBuilder.setContentIntent(resultPendingIntent);

        notificationManager.notify(notificationId, mBuilder.build());
    }


    private Integer gazVer()
    {

        Log.w("Activity - GAZLA", "gazlaniyor");

        try
        {
            String gazStr = Jsoup.connect("https://thingspeak.com/channels/763927/field/3/last.html").get().text();
            return Integer.valueOf(gazStr);
        }
        catch (IOException e)
        {
            e.printStackTrace();
            return 0;
        }
    }
    private Integer hirsizDurumVer()
    {

        Log.w("Activity - GAZLA", "gazlaniyor");

        try
        {
            String hirsizDurumu = Jsoup.connect("https://thingspeak.com/channels/763927/field/4/last.html").get().text();
            return Integer.valueOf(hirsizDurumu);
        }
        catch (IOException e)
        {
            e.printStackTrace();
            return 0;
        }
    }

}
