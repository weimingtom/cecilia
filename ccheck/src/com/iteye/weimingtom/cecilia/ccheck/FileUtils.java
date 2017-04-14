package com.iteye.weimingtom.cecilia.ccheck;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.List;

public class FileUtils {
	public static List<String> fileToList(String filename) {
		List<String> resultList = new ArrayList<String>();
		FileInputStream fin = null;
		InputStreamReader reader = null;
		BufferedReader buffer = null;
		try {
			fin = new FileInputStream(filename);
			reader = new InputStreamReader(fin); 
			buffer = new BufferedReader(reader);
			String line;
			while (null != (line = buffer.readLine())) {
				resultList.add(line);
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (buffer != null) {
				try {
					buffer.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (fin != null) {
				try {
					fin.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return resultList;
	}
	
	public static void listTofile(String filename, List<String> list) {
		FileOutputStream fout = null;
		OutputStreamWriter writer = null;
		BufferedWriter buffer = null;
		try {
			fout = new FileOutputStream(filename);
			writer = new OutputStreamWriter(fout); 
			buffer = new BufferedWriter(writer);
			for (String str : list) {
				buffer.write(str);
				buffer.newLine();
				buffer.flush();
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (buffer != null) {
				try {
					buffer.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (writer != null) {
				try {
					writer.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (fout != null) {
				try {
					fout.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
