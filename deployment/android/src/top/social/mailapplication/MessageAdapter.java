package top.social.mailapplication;

import java.util.List;

import javax.mail.Message;
import javax.mail.internet.MimeUtility;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class MessageAdapter extends ArrayAdapter<Message> {
	private final String TAG = "MessageAdapter";

	private Context mContext;
	private List<Message> mDataSet;

	public MessageAdapter(Context context, List<Message> objects) {
		super(context, R.id.txtInboxListItemSubject, objects);
		mContext = context;
		mDataSet = objects;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		Message message = mDataSet.get(position);
		LayoutInflater inflater = (LayoutInflater)mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View itemView = inflater.inflate(R.layout.data_list_item_inbox, null, true);

		try {
			TextView text = (TextView)itemView.findViewById(R.id.txtInboxListItemSubject);
			text.setText(message.getSubject());
			text = (TextView)itemView.findViewById(R.id.txtInboxListItemSender);
			text.setText(MimeUtility.decodeText(message.getFrom()[0].toString()));
			text = (TextView)itemView.findViewById(R.id.txtInboxListItemDateTime);
			text.setText(message.getSentDate().toString());
		} catch (Exception ex) {
			Log.d(TAG, ex.getMessage());
		}

		return itemView;
	}
}
