package com.iteye.weimingtom.cecilia.ccheck;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CCheck {
	public static void main(String[] args) {
		String filename = "E:/github_wmt/cecilia_work/python_mini/parser/tokenizer.c";
		List<String> resultList = FileUtils.fileToList(filename);
		for (String line : resultList) {
			String lineScan = line;
			System.out.println("=========================================");
			System.out.println("lineScan = " + lineScan);
			Pattern pattern;
			Matcher matcher;
			pattern = Pattern.compile("([a-zA-Z_][a-zA-Z0-9_]*)", Pattern.MULTILINE);
			matcher = pattern.matcher(lineScan);
			while (matcher.find()) {
				String varName = matcher.group(1);
				int start = matcher.start();
				int end = matcher.end();
				String prefix1 = "";
				if (start - 1 >= 0) {
					prefix1 = lineScan.substring(start - 1, start);
				}
				String suffix1 = "";
				if (end <= lineScan.length() - 1) {
					suffix1 = lineScan.substring(end, end + 1);
				}
				String prefix2 = "";
				if (start - 2 >= 0) {
					prefix2 = lineScan.substring(start - 2, start);
				}
				String suffix2 = "";
				if (end <= lineScan.length() - 2) {
					suffix2 = lineScan.substring(end, end + 2);
				}
				if (prefix1.equals("\"") || suffix1.equals("\"")) {
					//skip
				} else if (prefix2.equals("\"<") || suffix2.equals("\">")) {
					//skip
				} else if (prefix1.equals("<") || suffix1.equals(">")) {
					//skip
					if (!lineScan.contains("#include")) {
						System.err.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>???");
						throw new RuntimeException("no #include beside <>");
					}
				} else if (prefix1.equals("#")) {
					//skip
				} else {
					System.out.println("varName = " + varName + ", pos = (" + start + ", " + end + ") : [" + prefix1 + "|" + suffix1 + "]");
				}
			}
		}
	}

}
