import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;


public class Main {

	public static void main(String[] args) {
		List<String> lines = readlines("down.txt");
		int index = 0;
		String preStr = "";
		for (String line : lines) {
			if (index == 0) {
				preStr = line;
			} else {
				if (preStr.length() > 0) {
					if (line.startsWith(preStr)) {
						String path = line.substring(preStr.length());
						System.out.println("curl " + line + " -o " + path + " --insecure --create-dirs ");
						//$ curl https://demo.avgjs.org/assets/saveload/load_bg.webp -o assets/saveload/l
						//oad_bg.webp --insecure --create-dirs
					}
				}
			}
			index++;
		}
	}

	private static List<String> readlines(String filename) {
		List<String> result = new ArrayList<String>();
		FileInputStream fin = null;
		InputStreamReader reader = null;
		BufferedReader buff = null;
		try {
			fin = new FileInputStream(filename);
			reader = new InputStreamReader(fin, "UTF-8");
			buff = new BufferedReader(reader);
			String line;
			while ((line = buff.readLine()) != null) {
				line = line.trim();
				if (line != null && line.length() > 0) {
					result.add(line);
				}
			}
		} catch (IOException e) {
			
		} finally {
			if (buff != null) {
				try {
					buff.close();
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
		return result;
	}
}
