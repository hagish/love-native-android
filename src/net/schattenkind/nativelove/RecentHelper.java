package net.schattenkind.nativelove;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Set;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class RecentHelper {
	private static final String PREFS_NAME = "net.schattenkind.nativelove.RecentPreferences";
	
	private static final String FIELD_RECENT = "recent";
	
	private static final LastUsedComparator lastUsedComparator = new LastUsedComparator();
	
	public static void addRecent(Context context, String path) {
		SharedPreferences preferences = context.getSharedPreferences(PREFS_NAME, 0);
		Set<String> recentSet = preferences.getStringSet(FIELD_RECENT, null);
		ArrayList<String> recentArrayList;
		if (recentSet != null) {
			recentArrayList = new ArrayList<String>(recentSet);
		}
		else {
			recentArrayList = new ArrayList<String>();
		}
		
		for (String string : recentArrayList) {
			RecentObject recent = RecentObject.parseFromString(string);
			if (recent.path.equals(path)) {
				recentArrayList.remove(string);
				break;
			}
		}
		
		recentArrayList.add(new RecentObject(path).toString());

		recentSet = new HashSet<String>(recentArrayList);
		Editor editor = preferences.edit();
		editor.putStringSet(FIELD_RECENT, recentSet);
		editor.commit();
	}
	
	public static String[] getRecent(Context context) {
		SharedPreferences preferences = context.getSharedPreferences(PREFS_NAME, 0);
		Set<String> recentSet = preferences.getStringSet(FIELD_RECENT, null);
		if (recentSet == null) {
			return new String[0];
		}
		
		
		RecentObject[] recent = new RecentObject[recentSet.size()];
		
		int i = 0;
		for (String path : recentSet) {
			recent[i] = RecentObject.parseFromString(path);
			i++;
		}
		
		Arrays.sort(recent, lastUsedComparator);
		
		String[] recentPaths = new String[recent.length];
		for (i = 0; i < recent.length; i++) {
			if (recent[i] != null) {
				recentPaths[i] = recent[i].path;
			}
		}
		
		return recentPaths;
	}
	
	public static class RecentObject {
		public long lastUsed;
		public String path;
		
		public RecentObject(String path) {
			this(System.currentTimeMillis(), path);
		}
		
		public RecentObject(long lastUsed, String path) {
			this.lastUsed = lastUsed;
			this.path = path;
		}
		
		public String toString() {
			return lastUsed + "." + path;
		}
		
		public static RecentObject parseFromString(String string) throws IllegalArgumentException {
			try {
				int dot = string.indexOf('.');
				if (dot == -1) {
					throw new Exception();
				}
				
				long lastUsed;
				String path;
				
				String lastUsed_string = string.substring(0, dot);
				lastUsed = Long.parseLong(lastUsed_string);
				
				path = string.substring(dot + 1);
				
				return new RecentObject(lastUsed, path);
			}
			catch (Exception e) {
				//Invalid format
				return null;
			}
		}
	}
	
	public static class LastUsedComparator implements Comparator<RecentObject> {
		
		public int compare(RecentObject o1, RecentObject o2) {
			
			//We are doing a bigger(...)smaller sorting, thus, o2 is written before o1.
			long comp = o2.lastUsed - o1.lastUsed;
			if (comp != 0) {
				return (int) comp;
			}
			
			return o1.path.compareToIgnoreCase(o2.path);
		}
	}
}
