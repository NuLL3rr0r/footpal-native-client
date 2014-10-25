package com.ertebat.client;

import java.io.IOException;
import java.security.InvalidParameterException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import javax.mail.Address;
import javax.mail.Folder;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Multipart;
import javax.mail.NoSuchProviderException;
import javax.mail.Part;
import javax.mail.Session;
import javax.mail.Store;
import javax.mail.Transport;
import javax.mail.Message.RecipientType;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class MailProfile extends org.qtproject.qt5.android.bindings.QtActivity {
        private static final String TAG = "[Android Mail Interface / top.social.mailApplication.MailProfile]";
        public static String Title = "GMail";
        public static String SMTPHost = "smtp.gmail.com";
        public static String Host = "imap.gmail.com";
        public static MailProtocol Protocol = MailProtocol.MP_POP3S;
        public static String Username = "";
        public static String Password = "";
        public static boolean sIsReading = false;

        private static Map<String, String> sMonthIndices;
        private static Properties sMailProperties;
        private static Session sMailSession;
        private static Transport sMailTransport;
        private static Store sMailStore;
        private static Message sMailMessage;	// TODO: Decide what to do with this

        public MailProfile() {
                Log.v(TAG, "Android interface successfully initialized!");
        }

        /**
         * Prepares MailProfile for setup. Must be called before other methods
         */
        public static void init() {
                Log.v(TAG, "INIT IS HERE!");
                sMonthIndices = new HashMap<String, String>();
                sMonthIndices.put("Jan", "01");
                sMonthIndices.put("Feb", "02");
                sMonthIndices.put("Mar", "03");
                sMonthIndices.put("Apr", "04");
                sMonthIndices.put("May", "05");
                sMonthIndices.put("Jun", "06");
                sMonthIndices.put("Jul", "07");
                sMonthIndices.put("Aug", "08");
                sMonthIndices.put("Sep", "09");
                sMonthIndices.put("Oct", "10");
                sMonthIndices.put("Nov", "11");
                sMonthIndices.put("Dec", "12");

                sMailProperties = System.getProperties();

                Log.v(TAG, "INIT IS FINISHED!");
                if(System.getProperties() == null) {
                    Log.v(TAG, "NOOOOOOOOOOOOOOOOOOO!");
                }

        }

        /**
         * Sets the host address for the given protocol
         * @param host address of the host e.g. 'smtp.gmail.com' for GMail's SMTP host
         * @param protocol the protocol for which the host address is set. Accepted values are 'smtp', 'imap', 'pop3'
         */
        public static void setHost(String host, String protocol) {
                Log.v(TAG, "setHost(host = " + host + ", protocol = " + protocol);

                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }
                if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
                        System.out.println("Unknown protocol: " + protocol);
                        throw new InvalidParameterException("Unknown protocol: " + protocol);
                }

                sMailProperties.setProperty("mail." + protocol + ".host", host);
        }

        /**
         * Sets the SMTP host address
         * @param host Address of the SMTP host e.g. 'smtp.gmail.com' for GMail
         */
        public static void setSmtpHost(String host) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.smtp.host", host);
        }

        /**
         * Sets the IMAP host address
         * @param host Address of the IMAP host e.g. 'imap.gmail.com' for GMail
         */
        public static void setImapHost(String host) {
                Log.v(TAG,"SetImapHost " + host);

                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.imap.host", host);
        }

        /**
         * Sets the port number for the given protocol
         * @param port port number on the host to connect to
         * @param protocol the protocol for which the port number is set. Accepted values are 'smtp', 'imap', 'pop3'
         */
        public static void setPort(short port, String protocol) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }
                if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
                        System.out.println("Unknown protocol: " + protocol);
                        throw new InvalidParameterException("Unknown protocol: " + protocol);
                }

                sMailProperties.setProperty("mail." + protocol + ".port", String.valueOf(port));
        }

        /**
         * Sets the SMTP host port
         * @param port port number on SMTP host to connect to
         */
        public static void setSmtpPort(short port) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.smtp.port", String.valueOf(port));
        }

        /**
         * Sets the IMAP host port
         * @param port port number on IMAP host to connect to
         */
        public static void setImapPort(short port) {
                Log.v(TAG,"SetImapPort " + port);
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.imap.port", String.valueOf(port));
        }

        /**
         * Sets the user name for the current account and all protocols
         * @param username e.g. someone@somewhere.com
         */
        public static void setUsername(String username) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.smtp.user", username);
                sMailProperties.setProperty("mail.imap.user", username);
                sMailProperties.setProperty("mail.pop3.user", username);
        }

        /**
         * Sets the user name for the current account and the given protocol
         * @param username e.g. someone@somewhere.com
         * @param protocol the protocol to apply the user name to. Accepted values are 'smtp', 'imap', 'pop3'.
         */
        public static void setUsername(String username, String protocol) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }
                if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
                        System.out.println("Unknown protocol: " + protocol);
                        throw new InvalidParameterException("Unknown protocol: " + protocol);
                }

                sMailProperties.setProperty("mail." + protocol + ".user", username);
                Username = username;
        }

        /**
         * Sets the user name for the current account
         * @param username e.g. someone@somewhere.com
         */
        public static void setSmtpUsername(String username) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.smtp.user", username);
                Username = username;
        }

        /**
         * Sets the user name for the current account
         * @param username e.g. someone@somewhere.com
         */
        public static void setImapUsername(String username) {
                Log.v(TAG,"setImapUsername " + username);
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.imap.user", username);
                Username = username;
        }

        /**
         * Sets the password to use with the account and all protocols
         * @param password
         */
        public static void setPassword(String password) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.smtp.password", password);
                sMailProperties.setProperty("mail.imap.password", password);
                sMailProperties.setProperty("mail.pop3.password", password);
                Password = password;
        }

        /**
         * Sets the password to use with the account and the given protocol
         * @param password
         * @param protocol the protocol to apply the password to. Accepted values are 'smtp', 'imap', 'pop3'.
         */
        public static void setPassword(String password, String protocol) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }
                if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
                        System.out.println("Unknown protocol: " + protocol);
                        throw new InvalidParameterException("Unknown protocol: " + protocol);
                }

                sMailProperties.setProperty("mail." + protocol + ".password", password);
                Password = password;
        }

        /**
         * Sets the password to use with the account
         * @param password
         */
        public static void setSmtpPassword(String password) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.smtp.password", password);
                Password = password;
        }

        /**
         * Sets the password to use with the account
         * @param password
         */
        public static void setImapPassword(String password) {
                Log.v(TAG,"setImapPassword " + password);
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                sMailProperties.setProperty("mail.imap.password", password);
                Password = password;
        }

        /**
         * Configures MailProfile for the given security type on all protocols
         * @param securityTypeIndex index of the preferred security method. 0=PLAIN, 1=SSL/TLS, 2=STARTTLS
         */
        public static void setSecurity(int securityTypeIndex) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                switch (securityTypeIndex) {
                case 1:			//	SSL/TLS
                        sMailProperties.setProperty("mail.smtp.ssl.enable", "true");
                        sMailProperties.setProperty("mail.imap.ssl.enable", "true");
                        sMailProperties.setProperty("mail.pop3.ssl.enable", "true");
                        break;
                case 2:			//	STARTTLS
                        sMailProperties.setProperty("mail.smtp.starttls.enable", "true");
                        sMailProperties.setProperty("mail.imap.starttls.enable", "true");
                        sMailProperties.setProperty("mail.pop3.starttls.enable", "true");
                        break;
                case 0:			//	PLAIN TEXT
                        //	fall through
                default:
                        break;
                }
        }

        /**
         * Configures MailProfile for the given security type on the given protocol
         * @param securityTypeIndex index of the preferred security method. 0=PLAIN, 1=SSL/TLS, 2=STARTTLS
         * @param protocol the protocol for which to set the security configuration. Accepted values are 'smtp', 'imap', 'pop3'.
         */
        public static void setSecurity(int securityTypeIndex, String protocol) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }
                if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
                        System.out.println("Unknown protocol: " + protocol);
                        throw new InvalidParameterException("Unknown protocol: " + protocol);
                }

                switch (securityTypeIndex) {
                case 1:			//	SSL/TLS
                        sMailProperties.setProperty("mail." + protocol + ".ssl.enable", "true");
                        break;
                case 2:			//	STARTTLS
                        sMailProperties.setProperty("mail." + protocol + ".starttls.enable", "true");
                        break;
                case 0:			//	PLAIN TEXT
                        //	fall through
                default:
                        break;
                }
        }

        /**
         * Configures MailProfile for the given security type when using SMTP
         * @param securityTypeIndex index of the preferred security method. 0=PLAIN, 1=SSL/TLS, 2=STARTTLS
         */
        public static void setSmtpSecurity(int securityTypeIndex) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                switch (securityTypeIndex) {
                case 1:			//	SSL/TLS
                        sMailProperties.setProperty("mail.smtp.ssl.enable", "true");
                        break;
                case 2:			//	STARTTLS
                        sMailProperties.setProperty("mail.smtp.starttls.enable", "true");
                        break;
                case 0:			//	PLAIN TEXT
                        //	fall through
                default:
                        break;
                }
        }

        /**
         * Configures MailProfile for the given security type when using IMAP
         * @param securityTypeIndex index of the preferred security method. 0=PLAIN, 1=SSL/TLS, 2=STARTTLS
         */
        public static void setImapSecurity(int securityTypeIndex) {
                Log.v(TAG,"setImapSecurity " + securityTypeIndex);
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                switch (securityTypeIndex) {
                case 1:			//	SSL/TLS
                        sMailProperties.setProperty("mail.imap.ssl.enable", "true");
                        break;
                case 2:			//	STARTTLS
                        sMailProperties.setProperty("mail.imap.starttls.enable", "true");
                        break;
                case 0:			//	PLAIN TEXT
                        //	fall through
                default:
                        break;
                }
        }

        /**
         * Opens a connection to the host using the given protocol. Must be called before send and fetch operations. For fetch
         * operations a Store is created and connected to the host while for send operations a Transport object is created and connected.
         * @param protocol the protocol by which to connect to the host. Accepted values are 'smtp', 'imap' and 'pop3'.
         * Use SMTP for send and IMAP/POP3 for fetch operations
         */
         public static void connect(String protocol) {
                Log.v(TAG,"connect " + protocol);
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");

                        return;
                }
                if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
                        System.out.println("Unknown protocol: " + protocol);
                        throw new InvalidParameterException("Unknown protocol: " + protocol);
                }

                try {
                        if (protocol.equals("smtp")) {
                                sMailSession = Session.getInstance(sMailProperties);
                                sMailTransport = sMailSession.getTransport("smtp");
                                sMailTransport.connect();
                        } else {
                                sMailSession = Session.getInstance(sMailProperties);
                                sMailStore = sMailSession.getStore(protocol);
                                sMailStore.connect();
                        }
                } catch (NoSuchProviderException nspEx) {
                        if (nspEx.getMessage() != null)
                                Log.d(TAG, nspEx.getMessage());
                        else
                                Log.d(TAG, "NoSuchProviderException was thrown with no further message");
                } catch (MessagingException mEx) {
                        if (mEx.getMessage() != null)
                                Log.d(TAG, mEx.getMessage());
                        else
                                Log.d(TAG, "MessagingException was thrown with no further message");
                }
        }

        /**
         * Opens a connection to the SMTP host. Must be called before send()
         */
        public static void connectSmtp() {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                try {
                        sMailSession = Session.getInstance(sMailProperties);
                        sMailTransport = sMailSession.getTransport("smtp");
                        sMailTransport.connect();
                } catch (NoSuchProviderException nspEx) {
                        if (nspEx.getMessage() != null)
                                Log.d(TAG, nspEx.getMessage());
                        else
                                Log.d(TAG, "NoSuchProviderException was thrown with no further message");
                } catch (MessagingException mEx) {
                        if (mEx.getMessage() != null)
                                Log.d(TAG, mEx.getMessage());
                        else
                                Log.d(TAG, "MessagingException was thrown with no further message");
                }
        }

        /**
         * Closes the previously established connection for a certain protocol. Must be called after connect()
         * @param protocol the protocol by which the connection had been made. Accepted values are 'smtp', 'imap' and 'pop3'
         */
        public static void disconnect(String protocol) {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }
                if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
                        System.out.println("Unknown protocol: " + protocol);
                        return;
                }

                try {
                        if (protocol.equals("smtp")) {
                                if (sMailTransport != null && sMailTransport.isConnected()) {
                                        sMailTransport.close();
                                }
                        } else {
                                if (sMailStore != null && sMailStore.isConnected()) {
                                        sMailStore.close();
                                }
                        }
                } catch (MessagingException mEx) {
                        if (mEx.getMessage() != null)
                                Log.d(TAG, mEx.getMessage());
                        else
                                Log.d(TAG, "MessagingException was thrown with no further message");
                }
        }

        /**
         * Closes the previously established connection with the SMTP host. Must be called after connect()
         */
        public static void disconnectSmtp() {
                if (sMailProperties == null) {
                        System.out.println("Invalid method call. Must first call init()");
                        return;
                }

                if (sMailTransport != null && sMailTransport.isConnected()) {
                        try {
                                sMailTransport.close();
                        } catch (MessagingException mEx) {
                                if (mEx.getMessage() != null)
                                        Log.d(TAG, mEx.getMessage());
                                else
                                        Log.d(TAG, "MessagingException was thrown with no further message");
                        }
                }
        }

        /**
         * TODO: Decide on how to configure the message
         * @param jsonMessage the message in j
         */
        public static void send(String jsonMessage) {

        }

        /**
         * Reads a range of messages in reverse chronological order
         * @param startIndex zero-based index of the message to read first. 0 is the index of the newest message of the inbox
         * @param count number of messages to read
         */

         public static int getMessageCount() {
             try {
                     // get inbox folder
                     Folder folder = sMailStore.getFolder("inbox");
                     folder.open(Folder.READ_ONLY);

                     if (folder.isOpen()) {

                             int messageCount = folder.getMessageCount();

                             //folder.close();
                             sMailStore.close();
                             return messageCount;
                     } else {
                             System.out.println("Folder isn't open");
                     }

                     //folder.close();
                     sMailStore.close();
                     return 0;
             } catch (Exception ex) {
                     System.out.println(ex.getMessage());
             }

             return 0;
         }


        public static String fetchMessages(int startIndex, int count) {

            System.out.println("fetchMessage called! :P");
            Log.v(TAG, "++ fetchMessage(" + startIndex + ", count = " + count + ")");
            String ret = "{\"DATA\": [";

            try {
                    // get inbox folder
                    Folder folder = sMailStore.getFolder("inbox");
                    folder.open(Folder.READ_ONLY);

                    if (folder.isOpen()) {
                            Log.d(TAG, "Total: " + folder.getMessageCount());
                            Log.d(TAG, "New: " + folder.getNewMessageCount());
                            Log.d(TAG, "Unread: " + folder.getUnreadMessageCount());

                            // get messages
                            int messageCount = folder.getMessageCount();
                            Message[] messages = folder.getMessages(messageCount - 2, messageCount);
                            //String[] temp = new String[messageCount];

                            for (int i = messages.length - 1; i >= 0; i--) {
                                   System.out.println("i");
                                   String temp = getJsonMessage(messages[i]);
                                   if(i == messages.length - 1) {
                                        ret = ret + ",";
                                   }
                                   ret = ret + "{\"ORIG\": " + temp;
                                   System.out.println("i - " + temp);
                            }

                            ret = ret + "] }";
                            folder.close(true);
                            sMailStore.close();

                            // TODO: call something on CPP side

                            return ret;
                    } else {
                            System.out.println("Folder isn't open");
                    }

                    //folder.close();
                    sMailStore.close();

            } catch (Exception ex) {
                    System.out.println(ex.getMessage());
            }

            ret = "[DEVIL]";
            System.out.println("--- END");
            return ret;
        }

        private static String getJsonMessage(Message message) {
                String jsonString = "";

                try {
                        JSONObject jsonObject = new JSONObject();
                        String[] headers = message.getHeader("Message-ID");
                        if (headers == null)
                                jsonObject.put("message_id", "");
                        else
                                jsonObject.put("message_id", headers[0]);

                        JSONArray jsonArray = new JSONArray();
                        JSONObject tempJsonObject = new JSONObject();

                        for (Address address : message.getFrom()) {
                                tempJsonObject = new JSONObject();
                                String[] tokens = address.toString().split("[<>]");
                                if (tokens.length > 1) {
                                        tempJsonObject.put("name", tokens[0]);
                                        tempJsonObject.put("email", tokens[1]);
                                } else {
                                        tempJsonObject.put("email", tokens[0]);
                                }
                                jsonArray.put(tempJsonObject);
                        }
                        jsonObject.put("from", jsonArray);
                        jsonObject.put("subject", message.getSubject());

                        jsonArray = new JSONArray();
                        Address[] addresses = message.getRecipients(RecipientType.TO);
                        if (addresses != null) {
                                for (int i = 0; i < addresses.length; i++) {
                                        tempJsonObject = new JSONObject();
                                        String[] tokens = addresses[i].toString().split("[<>]");
                                        if (tokens.length > 1) {
                                                tempJsonObject.put("name", tokens[0]);
                                                tempJsonObject.put("email", tokens[1]);
                                                tempJsonObject.put("type", "TO");
                                        } else {
                                                tempJsonObject.put("name", "");
                                                tempJsonObject.put("email", tokens[0]);
                                                tempJsonObject.put("type", "TO");
                                        }
                                        jsonArray.put(tempJsonObject);
                                }
                        }

                        addresses = message.getRecipients(RecipientType.CC);
                        if (addresses != null) {
                                for (int i = 0; i < addresses.length; i++) {
                                        tempJsonObject = new JSONObject();
                                        String[] tokens = addresses[i].toString().split("[<>]");
                                        if (tokens.length > 1) {
                                                tempJsonObject.put("name", tokens[0]);
                                                tempJsonObject.put("email", tokens[1]);
                                                tempJsonObject.put("type", "CC");
                                        } else {
                                                tempJsonObject.put("name", "");
                                                tempJsonObject.put("email", tokens[0]);
                                                tempJsonObject.put("type", "CC");
                                        }
                                        jsonArray.put(tempJsonObject);
                                }
                        }

                        addresses = message.getRecipients(RecipientType.BCC);
                        if (addresses != null) {
                                for (int i = 0; i < addresses.length; i++) {
                                        tempJsonObject = new JSONObject();
                                        String[] tokens = addresses[i].toString().split("[<>]");
                                        if (tokens.length > 1) {
                                                tempJsonObject.put("name", tokens[0]);
                                                tempJsonObject.put("email", tokens[1]);
                                                tempJsonObject.put("type", "BCC");
                                        } else {
                                                tempJsonObject.put("name", "");
                                                tempJsonObject.put("email", tokens[0]);
                                                tempJsonObject.put("type", "BCC");
                                        }
                                        jsonArray.put(tempJsonObject);
                                }
                        }
                        jsonObject.put("to", jsonArray);

                        jsonObject.put("date", parseHeaderDate(message.getHeader("Date")[0]));
                        tempJsonObject = new JSONObject();
                        headers = message.getHeader("Reply-To");
                        if (headers != null)
                                tempJsonObject.put("Reply-To", headers[0]);
                        jsonObject.put("headers", tempJsonObject);
                        jsonObject.put("text", getMessageText(message));
                        jsonObject.put("attachments", "[{}]");

                        jsonString = jsonObject.toString();
                } catch (JSONException jEx) {
                        if (jEx.getMessage() != null)
                                Log.d(TAG, jEx.getMessage());
                        else
                                Log.d(TAG, "A JSONException was thrown with no further message!");
                } catch (MessagingException mEx) {
                        if (mEx.getMessage() != null)
                                Log.d(TAG, mEx.getMessage());
                        else
                                Log.d(TAG, "A MessagingException was thrown with no further message!");
                }

                return jsonString;
        }

        private static String parseHeaderDate(String date) {
                // Sat, 18 Oct 2014 13:18:23 +0330
                String[] tokens = date.split("[ :\\,]");

                if (tokens.length < 9) {
                        return "INVALID";
                }

                String result = tokens[0];
                if (tokens[2].length() < 2)
                        tokens[2] = "0" + tokens[2];

                tokens[3] = sMonthIndices.get(tokens[3]);

                for (int i = 1; i < 9; i++) {
                        if (tokens[i].length() > 0)
                                result += ":" + tokens[i];
                }

                return result;
        }

        /**
         * Return the primary text content of the message.
         */
        private static String getMessageText(Part p) {
                try {
                        if (p.isMimeType("text/*")) {
                                String s = (String)p.getContent();
                                //            textIsHtml = p.isMimeType("text/html");
                                return s;
                        }

                        if (p.isMimeType("multipart/alternative")) {
                                // prefer html text over plain text
                                Multipart mp = (Multipart)p.getContent();
                                String text = null;
                                for (int i = 0; i < mp.getCount(); i++) {
                                        Part bp = mp.getBodyPart(i);
                                        if (bp.isMimeType("text/plain")) {
                                                text = (String) bp.getContent();
                                        } else if (bp.isMimeType("text/html")) {
                                                String s = getMessageText(bp);
                                                if (s != null)
                                                        return s;
                                        } else {
                                                return getMessageText(bp);
                                        }
                                }
                                return text;
                        } else if (p.isMimeType("multipart/*")) {
                                Multipart mp = (Multipart)p.getContent();
                                for (int i = 0; i < mp.getCount(); i++) {
                                        String s = getMessageText(mp.getBodyPart(i));
                                        if (s != null)
                                                return s;
                                }
                        }
                } catch (MessagingException mEx) {
                        if (mEx.getMessage() != null)
                                Log.d(TAG, mEx.getMessage());
                        else
                                Log.d(TAG, "A MessagingException was thrown with no further message!");
                } catch (IOException ioEx) {
                        if (ioEx.getMessage() != null)
                                Log.d(TAG, ioEx.getMessage());
                        else
                                Log.d(TAG, "A IOException was thrown with no further message!");
                }

                return "COULD NOT FIND MESSAGE BODY";
        }
}
