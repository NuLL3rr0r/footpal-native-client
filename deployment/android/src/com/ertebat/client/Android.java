/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


package com.ertebat.client;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;
import java.lang.Exception;

public class Android extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static final String TAG = "[Android Interface / com.ertebat.client.Android]";

    private static Android s_instance;

    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_notificationBuilder;

    public static boolean release()
    {
        try {
            if (s_instance != null) {
                s_instance = null;

                Log.v(TAG, "Android interface successfully released!");
            }
        }

        catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public static boolean isInitialized()
    {
        Log.v(TAG, "isInitialized");

        return s_instance != null ? true : false;
    }

    public static CharSequence getScreenType()
    {
        return s_instance.getResources().getString(R.string.screen_type);
    }

    public static boolean notify(final CharSequence title, final CharSequence text, final int id)
    {
        Log.v(TAG, "notify");

        try {
            if (m_notificationManager == null) {
                m_notificationManager = (NotificationManager)s_instance.getSystemService(Context.NOTIFICATION_SERVICE);
                m_notificationBuilder = new Notification.Builder(s_instance);
                m_notificationBuilder.setSmallIcon(R.drawable.icon);
            }

            m_notificationBuilder.setContentTitle(title);
            m_notificationBuilder.setContentText(text);
            m_notificationManager.notify(id, m_notificationBuilder.build());
        }

        catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public static boolean showToast(final CharSequence text, final int duration)
    {
        Log.v(TAG, "showToast");

        try {
            s_instance.runOnUiThread(new Runnable() {
                public void run() {
                    Toast toast = Toast.makeText(s_instance, text, duration);
                    toast.show();
                }
            });
        }

        catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public static void mailProfile_setHost(CharSequence host, CharSequence protocol)
    {
        MailProfile.setHost(host.toString(), protocol.toString());
    }

    public static void mailProfile_init() {
        MailProfile.init();
    }

    public static void mailProfile_setPort(short port, CharSequence protocol) {
        MailProfile.setPort(port, protocol.toString());
    }

    public static void mailProfile_setUsername(CharSequence username, CharSequence protocol) {
        MailProfile.setUsername(username.toString(), protocol.toString());
    }

    public static void mailProfile_setPassword(CharSequence password, CharSequence protocol) {
        MailProfile.setPassword(password.toString(), protocol.toString());
    }

    public static void mailProfile_setSecurity(int securityTypeIndex, CharSequence protocol) {
        MailProfile.setSecurity(securityTypeIndex, protocol.toString());
    }

    public static void mailProfile_connect(CharSequence protocol) {
        Log.d(TAG, protocol.toString());
        MailProfile.connect(String.valueOf(protocol));
    }

    public static void mailProfile_disconnect(CharSequence protocol) {
        MailProfile.disconnect(protocol.toString());
    }


    public static void mailProfile_send(CharSequence jsonMessage) {
        MailProfile.send(jsonMessage.toString());
    }


    public static int mailProfile_getMessageCount() {
        return MailProfile.getMessageCount();
    }


    public static CharSequence mailProfile_fetchMessages(int startIndex, int count) {
        String ret = MailProfile.fetchMessages(startIndex, count);
        return ret;
    }



    public Android()
    {
        s_instance = this;

        Log.v(TAG, "Android interface successfully initialized!");
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }
}

