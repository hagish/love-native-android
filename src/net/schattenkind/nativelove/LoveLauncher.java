package net.schattenkind.nativelove;

import java.io.File;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class LoveLauncher extends Activity {
	
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.launcher);
	}
	
	public void onResume() {
		super.onResume();
		
		populateRecents();
	}
	
	private void populateRecents() {
		String[] recent = RecentHelper.getRecent(this);
		String[] recentShort = new String[recent.length];
		
		for (int i = 0; i < recent.length; i++) {
			if (recent[i] != null) {
				File file = new File(recent[i]);
				recentShort[i] = file.getName();
			}
		}
		
		ListView listView = (ListView) findViewById(R.id.recentListView);
		listView.setAdapter(new TwoLineArrayAdapter(this, recentShort, recent));
		
		listView.setOnItemClickListener(new OnItemClickListener(){

			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				Intent i = new Intent(LoveLauncher.this, LoveNative.class);
				i.setData(Uri.parse("file://" + (String) (parent.getAdapter().getItem(position))));
				startActivity(i);
			}
			
		});
	}
	
	public void launchDefault(View v) {
		Intent i = new Intent(this, LoveNative.class);
		startActivity(i);
	}
	
	public void browseFiles(View v) {
		Intent i = new Intent(this, FileBrowser.class);
		startActivity(i);
	}
	
	
	
	
	public class TwoLineArrayAdapter extends BaseAdapter {
		private LayoutInflater mInflater;
		
		private String[] array0;
		private String[] array1;
		
		public TwoLineArrayAdapter(Context context, String[] array0, String[] array1) {
			mInflater = LayoutInflater.from(context);
			
			this.array0 = array0;
			this.array1 = array1;
		}
		
		public View getView(int position, View convertView, ViewGroup parent) {
			if (convertView == null) {
				convertView = mInflater.inflate(android.R.layout.two_line_list_item, parent, false);
			}
			TextView textView0 = (TextView) convertView.findViewById(android.R.id.text1);
			TextView textView1 = (TextView) convertView.findViewById(android.R.id.text2);
			
			textView0.setText(array0[position]);
			textView1.setText(array1[position]);
			
			return convertView;
		}
		
		public int getCount() {
			return array0.length;
		}
		
		public Object getItem(int arg0) {
			return array1[arg0];
		}
		
		public long getItemId(int arg0) {
			return arg0;
		}
	}
}
