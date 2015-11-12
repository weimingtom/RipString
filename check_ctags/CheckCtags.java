import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class CheckCtags {

	public static void main(String[] args) throws IOException {
		Scanner scanner;
		scanner = new Scanner(new File("check_ctags_output.txt"), "utf-8");
		Map<String, String> map = new HashMap<String, String>();
		while (scanner.hasNextLine()) {
			String line = scanner.nextLine();
			String[] lines = line.split("\\t");
			if (lines.length > 0) {
				if (map.containsKey(lines[0])) {
					System.out.println("redefined symbol : " + lines[0] + " => " + line);
				} else {
					map.put(lines[0], lines[0]);
				}
			}
		}
		scanner.close();
	}
	
}
