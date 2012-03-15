package net.schattenkind.nativelove;

import java.io.File;
import java.io.FileFilter;
import java.util.Arrays;
import java.util.Comparator;

import android.app.Dialog;
import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class FileBrowser extends ListActivity implements OnItemLongClickListener {
	public static final String EXTRA_PATH = "browser_path";
	
	private static final int DIALOG_USE_FOLDER = 0;
	
	private String path = Environment.getExternalStorageDirectory().getAbsolutePath();
	private String[] content = null;
	
	private String folderPath = "";
	
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Intent intent = getIntent();
		if (intent.hasExtra(EXTRA_PATH)) {
			path = intent.getStringExtra(EXTRA_PATH);
		}
		
		fillList();
		getListView().setOnItemLongClickListener(this);
	}
	
	public void onListItemClick(ListView l, View v, int position, long id) {
		File file = new File(path + "/" + content[position]);
		if (file.isDirectory()) {
			Intent i = new Intent(this, FileBrowser.class);
			i.putExtra(EXTRA_PATH, file.getAbsolutePath());
			startActivity(i);
			return;
		}
		
		Intent i = new Intent(this, LoveNative.class);
		i.setData(Uri.parse("file://" + file.getAbsolutePath()));
		startActivity(i);
	}
	

	
	public boolean onItemLongClick(AdapterView<?> parent, View v, int position, long id) {
		File file = new File(path + "/" + content[position]);
		if (file.isDirectory()) {
			folderPath = "file://" + file.getAbsolutePath();
			showDialog(DIALOG_USE_FOLDER);
		}
		
		return false;
	}
	
    @Override
    protected Dialog onCreateDialog(int id) {
        switch (id) {
	        case DIALOG_USE_FOLDER:
	            return new AlertDialog.Builder(this)
	            	.setTitle("Open with folder")
	                .setMessage("Do you want to open this folder as an unpacked .love application?")
	                .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
	                    public void onClick(DialogInterface dialog, int whichButton) {
	                    	Intent i = new Intent(FileBrowser.this, LoveNative.class);
	                		i.setData(Uri.parse(folderPath));
	                		startActivity(i);
	                    }
	                })
	                .setNegativeButton("No", new DialogInterface.OnClickListener() {
	                    public void onClick(DialogInterface dialog, int whichButton) {
	
	                    }
	                })
	                .create();
        }
        
        return null;
    }
	
	private void fillList() {
		File file = new File(path);
		if (!file.isDirectory()) {
			return;
		}
		
		File[] files = file.listFiles(new LoveFilter());
		content = new String[files.length];
		
		for (int i = 0; i < files.length; i++) {
			File curFile = files[i];
			String name = curFile.getName();
			if (curFile.isDirectory()) {
				name += "/";
			}
			content[i] = name;
		}
		
		Arrays.sort(content, new FileSorter());
		
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, content);
		setListAdapter(adapter);
	}
	
	public class FileSorter implements Comparator<String> {
		
		public int compare(String s0, String s1) {
			boolean directory0 = s0.endsWith("/");
			boolean directory1 = s1.endsWith("/");
			
			if (directory0 == directory1) {
				return s0.compareToIgnoreCase(s1);
			}
			
			if (directory0) {
				return -1;
			}
			
			return 1;
		}
	}
	
	public class LoveFilter implements FileFilter {

		public boolean accept(File file) {
			return (file.isDirectory() || file.getName().endsWith(".love"));
		}
	}
}
