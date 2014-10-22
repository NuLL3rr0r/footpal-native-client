package top.social.mailapplication;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import javax.mail.Address;
import javax.mail.Authenticator;
import javax.mail.FetchProfile;
import javax.mail.Folder;
import javax.mail.Message;
import javax.mail.Message.RecipientType;
import javax.mail.MessagingException;
import javax.mail.Multipart;
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

import android.support.v7.app.ActionBarActivity;
import android.content.Intent;
import android.graphics.Typeface;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ListView;

public class MainActivity extends ActionBarActivity {
	private static final String TAG = "Mail";
	private static Map<String, String> sMonthIndices;
	public static Typeface TextFont;

	private Handler mHandler;
	private ListView mListViewMessages;
	private List<Message> mDataSet;
	private MessageAdapter mAdapter;
	private Button mBtnNewMail;
	private boolean mIsReading = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

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

		mHandler = new Handler();
		mBtnNewMail = (Button)findViewById(R.id.btnMainNewMail);
		mBtnNewMail.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				sendMail(new String[] { "morteza.sabetraftar@gmail.com", "info@babaei.net", "majid.sadeghi.alavijeh@gmail.com"});
			}
		});

		mDataSet = new ArrayList<Message>();
		mAdapter = new MessageAdapter(this, mDataSet);
		mListViewMessages = (ListView)findViewById(R.id.listViewMainInbox);
		mListViewMessages.setAdapter(mAdapter);
	}

	@Override
	protected void onResume() {
		setTitle(MailProfile.Title);
		super.onResume();

		mDataSet.clear();
		mAdapter.notifyDataSetChanged();

		getInbox();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			Intent intent = new Intent(this, SettingsActivity.class);
			startActivity(intent);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	private void getInbox() {
		//		String _from = ""; // email sent from
		//		String _port = "465"; // default smtp port
		//		String _sport = "465"; // default socketfactory port
		//		String _host = "smtp.gmail.com"; // default smtp server
		//		boolean _auth;
		//		boolean _debuggable;
		//		Multipart _multipart;
		
		if (mIsReading)
			return;
		
		mIsReading = true;

		new Thread(new Runnable() {

			@Override
			public void run() {
				// open email and get inbox
				Properties props = System.getProperties();
				props.put("mail.imap.host", "imap.gmail.com");
				props.setProperty("mail.store.protocol", "imaps");

				try {
					// open session
					Session session = Session.getDefaultInstance(props, null);
					Store store = session.getStore();
					store.connect(MailProfile.Host, MailProfile.Username, MailProfile.Password);

					// get inbox folder
					Folder folder = store.getFolder("inbox");
					folder.open(Folder.READ_ONLY);

					if (folder.isOpen()) {
						Log.d(TAG, "Total: " + folder.getMessageCount());
						Log.d(TAG, "New: " + folder.getNewMessageCount());
						Log.d(TAG, "Unread: " + folder.getUnreadMessageCount());

						// get messages
						int messageCount = folder.getMessageCount();
						Message[] messages = folder.getMessages(messageCount - 2, messageCount);
						mDataSet.clear();

						for (int i = messages.length - 1; i >= 0; i--) {
							mDataSet.add(messages[i]);

//							System.out.println(messages[i].getSubject());
//							System.out.println(getMessageText(messages[i]));
							System.out.println(getJsonMessage(messages[i]));

							//							writePart(messages[i]);
							//							getJsonMessage(messages[i]);

							// senders
							//							Address[] senders = messages[i].getFrom();
							//							for(Address addr:senders)
							//								System.out.println("Sender: "+addr); // TODO

							// message subject
							//							System.out.println("Subject: " + messages[i].getSubject()); // TODO

							// get message content (plain text) (part 0) , for HTML use part 1
							//							try {
							//								Multipart multipart = (Multipart)messages[i].getContent();
							//								BodyPart plainTextPart = multipart.getBodyPart(0);
							//								System.out.println("Body:");
							//								System.out.println(plainTextPart.getContent()); // TODO
							//							} catch (Exception e) {
							//								System.out.println("ERROR");
							//								e.printStackTrace();
							//							}
						}
						
						folder.close(true);
						
						mHandler.post(new Runnable() {
							@Override
							public void run() {
								mAdapter.notifyDataSetChanged();
							}
						});

						mIsReading = false;
					} else {
						System.out.println("Folder isn't open");
					}

					store.close();
				} catch (Exception ex) {
					System.out.println(ex.getMessage());
				}
			}
		}).start();
	}

	private void sendMail(final String[] tos) {
		new Thread(new Runnable() {

			@Override
			public void run() {
				try {
					Properties props = System.getProperties();
					props.put("mail.transport.protocol", "smtp");
					props.put("mail.smtp.host", MailProfile.SMTPHost);
					props.put("mail.smtp.port", "587");
					props.put("mail.smtp.auth", "true");
					props.put("mail.smtp.starttls.enable", "true");
					props.put("mail.smtp.user", MailProfile.Username);
					props.put("mail.smtp.password", MailProfile.Password);

					// Create session
					Session mailSession = null;
					Authenticator auth = new Authenticator(){
						protected PasswordAuthentication getPasswordAuthentication() {
							return new PasswordAuthentication(MailProfile.Username, MailProfile.Password);
						}
					};
					mailSession = Session.getInstance(props, auth);

					// Create a default MimeMessage object.
					MimeMessage message = new MimeMessage(mailSession);

					// Set "from" address
					message.setFrom(new InternetAddress(MailProfile.Username));

					// Set "to" address
					for (int i = 0; i < tos.length; i++) {
						message.addRecipient(Message.RecipientType.TO, new InternetAddress(tos[i]));
					}

					// Subject
					message.setSubject("Got it working!");

					// Body
					message.setText("Hey guys! This mail was sent by our app using javamail on Android.\n\nHave Fun! ;)");

					// Send message
					Transport transport = mailSession.getTransport("smtp");
					transport.connect(MailProfile.SMTPHost, MailProfile.Username, MailProfile.Password);
					transport.sendMessage(message, message.getAllRecipients());
					transport.close();

					Log.d(TAG, "Finished sending!");
				} catch (MessagingException mEx) {
					Log.d(TAG, mEx.getMessage());
				} catch (Exception ex) {
					Log.d(TAG, ex.getMessage());
				}
			}
		}).start();
	}

	private void findMessageAttachments(Part part, JSONArray result) throws Exception {
		//check if the content has attachment
		if (part.isMimeType("multipart/*")) {
			Multipart mp = (Multipart) part.getContent();
			int count = mp.getCount();
			for (int i = 0; i < count; i++)
				findMessageAttachments(mp.getBodyPart(i), result);
		} else if (part.isMimeType("image/*")) {

		} else if (!part.isMimeType("text/*") && !part.isMimeType("message/*")) {

		}

	}

	/*
	 * This method checks for content-type 
	 * based on which, it processes and
	 * fetches the content of the message
	 */
	private void writePart(Part p) throws Exception {
		//		if (p instanceof Message)
		//			//Call methos writeEnvelope
		//			writeEnvelope((Message) p);

		System.out.println("----------------------------");
		System.out.println("CONTENT-TYPE: " + p.getContentType());

		//check if the content is plain text
		if (p.isMimeType("text/plain")) {
			System.out.println("This is plain text");
			System.out.println("---------------------------");
			System.out.println((String) p.getContent());
		} 
		//check if the content has attachment
		else if (p.isMimeType("multipart/*")) {
			System.out.println("This is a Multipart");
			System.out.println("---------------------------");
			Multipart mp = (Multipart) p.getContent();
			int count = mp.getCount();
			for (int i = 0; i < count; i++)
				writePart(mp.getBodyPart(i));
		} 
		//check if the content is a nested message
		else if (p.isMimeType("message/rfc822")) {
			System.out.println("This is a Nested Message");
			System.out.println("---------------------------");
			writePart((Part) p.getContent());
		} 
		//check if the content is an inline image
		else if (p.isMimeType("image/jpeg")) {
			System.out.println("--------> image/jpeg");
			Object o = p.getContent();

			InputStream x = (InputStream) o;
			// Construct the required byte array
			System.out.println("x.length = " + x.available());
			byte[] bArray = new byte[x.available()];

			while ((int) ((InputStream) x).available() > 0) {
				int result = (int) (((InputStream) x).read(bArray));
				if (result == -1)
					break;
			}
			x.close();
			FileOutputStream f2 = new FileOutputStream("/tmp/image.jpg");
			f2.write(bArray);
			f2.close();
		} 
		else if (p.getContentType().contains("image/")) {
			System.out.println("content type" + p.getContentType());
			File f = new File("image" + new Date().getTime() + ".jpg");
			DataOutputStream output = new DataOutputStream(
					new BufferedOutputStream(new FileOutputStream(f)));
			com.sun.mail.util.BASE64DecoderStream test = 
					(com.sun.mail.util.BASE64DecoderStream) p
					.getContent();
			byte[] buffer = new byte[1024];
			int bytesRead;
			while ((bytesRead = test.read(buffer)) != -1) {
				output.write(buffer, 0, bytesRead);
			}
			output.close();
		} 
		else {
			Object o = p.getContent();
			if (o instanceof String) {
				System.out.println("This is a string");
				System.out.println("---------------------------");
				System.out.println((String) o);
			} 
			else if (o instanceof InputStream) {
				System.out.println("This is just an input stream");
				System.out.println("---------------------------");
				InputStream is = (InputStream) o;
				int c;
				while ((c = is.read()) != -1)
					System.out.write(c);
				is.close();
			} 
			else {
				System.out.println("This is an unknown type");
				System.out.println("---------------------------");
				System.out.println(o.toString());
			}
		}
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
				Log.d(TAG, "A JSONException was thrown with no further message!");
		} catch (MessagingException mEx) {
			if (mEx.getMessage() != null)
				Log.d(TAG, mEx.getMessage());
			else
				Log.d(TAG, "A MessagingException was thrown with no further message!");
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

	/**
	 * Return the primary text content of the message.
	 */
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
}
