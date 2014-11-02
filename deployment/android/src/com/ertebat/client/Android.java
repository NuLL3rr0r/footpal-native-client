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
    private static MailProfile s_mailProfileInstance;

    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_notificationBuilder;

    public static native void mailProfile_onConnectCompleted(boolean succeeded);
    public static native void mailProfile_onDisconnectCompleted();
    public static native void mailProfile_onSendCompleted(boolean succeeded);
    public static native void mailProfile_onGetMessageCountCompleted(int count);
    public static native void mailProfile_onFetchMessagesCompleted(CharSequence json);

    public static boolean release()
    {
        try {
            if (s_instance != null) {
                s_instance = null;

                Log.v(TAG, "Android interface successfully released!");
            }

            if (s_mailProfileInstance != null) {
                s_mailProfileInstance = null;

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
        s_mailProfileInstance.setHost(host.toString(), protocol.toString());
    }

    public static void mailProfile_init() {
        s_mailProfileInstance = new MailProfile();
    }

    public static void mailProfile_setPort(short port, CharSequence protocol) {
        s_mailProfileInstance.setPort(port, protocol.toString());
    }

    public static void mailProfile_setUsername(CharSequence username, CharSequence protocol) {
        s_mailProfileInstance.setUsername(username.toString(), protocol.toString());
    }

    public static void mailProfile_setPassword(CharSequence password, CharSequence protocol) {
        s_mailProfileInstance.setPassword(password.toString(), protocol.toString());
    }

    public static void mailProfile_setSecurity(int securityTypeIndex, CharSequence protocol) {
        s_mailProfileInstance.setSecurity(securityTypeIndex, protocol.toString());
    }

    public static void mailProfile_connect(CharSequence protocol) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                mailProfile_onConnectCompleted(s_mailProfileInstance.connect(String.valueOf(protocol)));
            }
        }).start();
    }

    public static void mailProfile_disconnect(CharSequence protocol) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                s_mailProfileInstance.disconnect(protocol.toString());
                mailProfile_onDisconnectCompleted();
            }
        }).start();
    }

    public static void mailProfile_send(CharSequence jsonMessage) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                mailProfile_onSendCompleted(s_mailProfileInstance.send(jsonMessage.toString()));
           }
       }).start();
    }

    public static void mailProfile_getMessageCount() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                mailProfile_onGetMessageCountCompleted(s_mailProfileInstance.getMessageCount());
            }
        }).start();
    }

    public static void mailProfile_fetchMessages(int startIndex, int count) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                mailProfile_onFetchMessagesCompleted(s_mailProfileInstance.fetchMessages(startIndex, count));
            }
        }).start();
    }

    public Android()
    {
        s_instance = this;
        s_mailProfileInstance = new MailProfile();

        Log.v(TAG, "Android interface successfully initialized!");
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }
}

