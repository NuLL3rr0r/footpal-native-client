package com.ertebat.client;

import java.io.IOException;
import java.security.InvalidParameterException;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Properties;

import javax.mail.Address;
import javax.mail.Folder;
import javax.mail.Message;
import javax.mail.Message.RecipientType;
import javax.mail.Authenticator;
import javax.mail.MessagingException;
import javax.mail.Multipart;
import javax.mail.NoSuchProviderException;
import javax.mail.Part;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Store;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class MailProfile {
        private final String TAG = "MailProfile";
        public String Title = "GMail";
        public String SMTPHost = "smtp.gmail.com";
        public String Host = "imap.gmail.com";
        public MailProtocol Protocol = MailProtocol.MP_POP3S;
        public String Username = "morteza.sabetraftar@gmail.com";
        public String Password = "zjwdmoszjtinyogs";

        private boolean sIsReading = false;
        private Map<String, String> sMonthIndices;
        private Properties sMailProperties;
        private Session sMailSession;
        private Transport sMailTransport;
        private Store sMailStore;

        public MailProfile() {
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
	}

	/**
	 * Sets the host address for the given protocol
	 * @param host address of the host e.g. 'smtp.gmail.com' for GMail's SMTP host
	 * @param protocol the protocol for which the host address is set. Accepted values are 'smtp', 'imap', 'pop3'
	 */
        public void setHost(String host, String protocol) {
		if (sMailProperties == null) {
			System.out.println("Invalid method call. Must first call init()");
			return;
		}
		if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
			System.out.println("Unknown protocol: " + protocol);
			return;
		}

		sMailProperties.setProperty("mail." + protocol + ".host", host);
		Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " host set to " + host);
	}

	/**
	 * Sets the port number for the given protocol
	 * @param port port number on the host to connect to
	 * @param protocol the protocol for which the port number is set. Accepted values are 'smtp', 'imap', 'pop3'
	 */
        public void setPort(short port, String protocol) {
		if (sMailProperties == null) {
			System.out.println("Invalid method call. Must first call init()");
			return;
		}
		if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
			System.out.println("Unknown protocol: " + protocol);
			return;
		}

		sMailProperties.setProperty("mail." + protocol + ".port", String.valueOf(port));
		Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " port set to " + String.valueOf(port));
	}

	/**
	 * Sets the user name for the current account and the given protocol
	 * @param username e.g. someone@somewhere.com
	 * @param protocol the protocol to apply the user name to. Accepted values are 'smtp', 'imap', 'pop3'.
	 */
        public void setUsername(String username, String protocol) {
		if (sMailProperties == null) {
			System.out.println("Invalid method call. Must first call init()");
			return;
		}
		if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
			System.out.println("Unknown protocol: " + protocol);
			return;
		}

		sMailProperties.setProperty("mail." + protocol + ".user", username);
		Username = username;
		Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " username set to " + username);
	}

	/**
	 * Sets the password to use with the account and the given protocol
	 * @param password
	 * @param protocol the protocol to apply the password to. Accepted values are 'smtp', 'imap', 'pop3'.
	 */
        public void setPassword(String password, String protocol) {
		if (sMailProperties == null) {
			System.out.println("Invalid method call. Must first call init()");
			return;
		}
		if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
			System.out.println("Unknown protocol: " + protocol);
			return;
		}

		sMailProperties.setProperty("mail." + protocol + ".password", password);
		Password = password;
		Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " password set to " + password);
	}

	/**
	 * Configures MailProfile for the given security type on the given protocol
	 * @param securityTypeIndex index of the preferred security method. 0=PLAIN, 1=SSL/TLS, 2=STARTTLS
	 * @param protocol the protocol for which to set the security configuration. Accepted values are 'smtp', 'imap', 'pop3'.
	 */
        public void setSecurity(int securityTypeIndex, String protocol) {
		if (sMailProperties == null) {
			System.out.println("Invalid method call. Must first call init()");
			return;
		}
		if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
			System.out.println("Unknown protocol: " + protocol);
			return;
		}

		switch (securityTypeIndex) {
		case 1:			//	SSL/TLS
			if (protocol.equals("smtp"))
				sMailProperties.setProperty("mail.transport.protocol", "smtps");
			else if (protocol.equals("imap"))
				sMailProperties.setProperty("mail.store.protocol", "imaps");
			else if (protocol.equals("pop3"))
				sMailProperties.setProperty("mail.store.protocol", "pop3s");

			sMailProperties.setProperty("mail." + protocol + ".ssl.enable", "true");
			Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " security set to SSL/TLS");
			break;
		case 2:			//	STARTTLS
			if (protocol.equals("smtp"))
				sMailProperties.setProperty("mail.transport.protocol", "smtps");
			else if (protocol.equals("imap"))
				sMailProperties.setProperty("mail.store.protocol", "imaps");
			else if (protocol.equals("pop3"))
				sMailProperties.setProperty("mail.store.protocol", "pop3s");

			sMailProperties.setProperty("mail." + protocol + ".starttls.enable", "true");
			Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " security set to STARTTLS");
			break;
		case 0:			//	PLAIN TEXT
			if (protocol.equals("smtp"))
				sMailProperties.setProperty("mail.transport.protocol", "smtp");
			else if (protocol.equals("imap"))
				sMailProperties.setProperty("mail.store.protocol", "imap");
			else if (protocol.equals("pop3"))
				sMailProperties.setProperty("mail.store.protocol", "pop3");

			Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " security set to PLAIN");
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
        public boolean connect(String protocol) {
		if (sMailProperties == null) {
			System.out.println("Invalid method call. Must first call init()");
                        return false;
		}
		if (!protocol.equals("smtp") && !protocol.equals("imap") && !protocol.equals("pop3")) {
			System.out.println("Unknown protocol: " + protocol);
                        return false;
		}

		try {
			if (protocol.equals("smtp")) {
				sMailProperties.setProperty("mail.smtp.auth", "true");
				Authenticator auth = new Authenticator(){
					protected PasswordAuthentication getPasswordAuthentication() {
                                                return new PasswordAuthentication(Username, Password);
					}
				};
				sMailSession = Session.getInstance(sMailProperties, auth);
				sMailTransport = sMailSession.getTransport("smtp");
				sMailTransport.connect();
                                Log.d(TAG, "SMTP transport connected!");
			} else {
				sMailSession = Session.getInstance(sMailProperties, null);
				sMailStore = sMailSession.getStore();
				Log.d(TAG, "Connecting to " + protocol.toUpperCase(Locale.ENGLISH) + " store...");
				sMailStore.connect(sMailProperties.getProperty("mail." + protocol + ".host"), Username, Password);
				Log.d(TAG, protocol.toUpperCase(Locale.ENGLISH) + " store connected!");                                
			}
                        return true;
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

                return false;
	}

	/**
	 * Closes the previously established connection for a certain protocol. Must be called after connect()
	 * @param protocol the protocol by which the connection had been made. Accepted values are 'smtp', 'imap' and 'pop3'
	 */
        public void disconnect(String protocol) {
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
					Log.d(TAG, "SMTP transport disconnected!");
				}
			} else {
				if (sMailStore != null && sMailStore.isConnected()) {
					sMailStore.close();
					Log.d(TAG, "Mail store closed!");
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
	 * Gets the number of messages in the inbox folder
	 * @return number of messages
	 */
        public int getMessageCount() {
		if (sMailStore == null || !sMailStore.isConnected()) {
			Log.d(TAG, "Store not connected. Cannot enumerate messages!");
			return -1;
		}

		try {
			Folder folder = sMailStore.getFolder("inbox");
			folder.open(Folder.READ_ONLY);
			int result = folder.getMessageCount();
			folder.close(true);
			return result;
		} catch (MessagingException mEx) {
			if (mEx.getMessage() != null)
				Log.d(TAG, mEx.getMessage());
			else
				Log.d(TAG, "MessagingException was thrown with no further message!");
		}
		
		return -1;
	}

	/**
	 * Reads a range of messages in reverse chronological order
	 * @param startIndex zero-based index of the message to read first. 0 is the index of the newest message of the inbox
	 * @param count number of messages to read
	 */
        public String fetchMessages(final int startIndex, final int count) {
                String sJsonMessages = "";

		if (sIsReading) {
			Log.d(TAG, "Already in fetch!");
                        return "";
		}

		sIsReading = true;

		try {
			// get inbox folder
			if (sMailStore == null || !sMailStore.isConnected()) {
				Log.d(TAG, "Store not connected. Cannot fetch!");
				return null;
			}

			Folder folder = sMailStore.getFolder("inbox");
			folder.open(Folder.READ_ONLY);

			if (folder.isOpen()) {
				Log.d(TAG, "Total: " + folder.getMessageCount());
				Log.d(TAG, "New: " + folder.getNewMessageCount());
				Log.d(TAG, "Unread: " + folder.getUnreadMessageCount());

				// get messages
				int readCount = count;
				int messageCount = folder.getMessageCount();
				if (startIndex + readCount > messageCount)
					readCount = messageCount - startIndex;
				Message[] messages = folder.getMessages(messageCount - (startIndex + readCount) + 1, messageCount - startIndex);

				JSONArray messageArray = new JSONArray();
				for (int i = messages.length - 1; i >= 0; i--) {
					messageArray.put(new JSONObject(getJsonMessage(messages[i])));
                                        //System.out.println(messages[i].getSubject());
				}
				JSONObject resultJson = new JSONObject();
				resultJson.put("data", messageArray);

				folder.close(true);
				sIsReading = false;
				sJsonMessages = resultJson.toString();
			} else {
				System.out.println("Folder isn't open");
			}
		} catch (JSONException jEx) {
			if (jEx.getMessage() != null)
				Log.d(TAG, jEx.getMessage());
			else
				Log.d(TAG, "JSONException was thrown with no further message");
		} catch (MessagingException mEx) {
			if (mEx.getMessage() != null)
				Log.d(TAG, mEx.getMessage());
			else
				Log.d(TAG, "MessagingException was thrown with no further message");
		}

		return sJsonMessages;
	}

	/**
	 * Sends an MIME message defined by a JSON string
	 * @param jsonMessage the JSON-formatted message to send
	 */
        public boolean send(String jsonMessage) {
		try {
			MimeMessage message = new MimeMessage(sMailSession);
                        message.setFrom(new InternetAddress(Username));

			JSONObject json = new JSONObject(jsonMessage);
			JSONArray jArray = json.getJSONArray("to");

			for (int i = 0; i < jArray.length(); i++) {
				JSONObject recipient = jArray.getJSONObject(i);
				Message.RecipientType type = stringToRecipientType(recipient.getString("type"));
				message.addRecipient(type, new InternetAddress(recipient.getString("email")));
			}

			message.setSubject(json.getString("subject"), "UTF-8");
                        message.setContent(json.getString("text"), "text/html");

			if (sMailTransport != null) {
				sMailTransport.sendMessage(message, message.getAllRecipients());
				Log.d(TAG, "Message successfully sent!");
                                return true;
			} else {
				System.out.println("Invalid method call. Must first call connect()");
                                return false;
			}
		} catch (JSONException jEx) {
			if (jEx.getMessage() != null)
				Log.d(TAG, jEx.getMessage());
			else
				Log.d(TAG, "JSONException was thrown with no further message");
		} catch (MessagingException mEx) {
			if (mEx.getMessage() != null)
				Log.d(TAG, mEx.getMessage());
			else
				Log.d(TAG, "MessagingException was thrown with no further message");
		}

                return false;
	}



        private String getJsonMessage(Message message) {
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
				Log.d(TAG, "JSONException was thrown with no further message");
		} catch (MessagingException mEx) {
			if (mEx.getMessage() != null)
				Log.d(TAG, mEx.getMessage());
			else
				Log.d(TAG, "MessagingException was thrown with no further message");
		}

		return jsonString;
	}

        private String parseHeaderDate(String date) {
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

        private String getMessageText(Part p) {
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

        private Message.RecipientType stringToRecipientType(String type) throws InvalidParameterException {
		if (type.equals("TO"))
			return RecipientType.TO;
		else if (type.equals("CC"))
			return RecipientType.CC;
		else if (type.equals("BCC"))
			return RecipientType.BCC;
		else
			throw new InvalidParameterException("Unknown recipient type");
	}
}
