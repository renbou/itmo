import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.LinkedHashMap;
import java.util.Map;

public class WordStatCountShingles {
    private static final int SHINGLE_SIZE = 3;

    public static boolean isWordChar(char c) {
        return Character.isLetter(c) || c == '\'' || Character.getType(c) == Character.DASH_PUNCTUATION;
    }

    public static void MakeStatistic(String inputFileName, String outputFileName) throws IOException {
        Map<String, Integer> statisticTable = new LinkedHashMap<>();
        try (BufferedReader input = new BufferedReader(
            new InputStreamReader(new FileInputStream(inputFileName), "UTF-8"))
        ) {
            char[] shingle = new char[SHINGLE_SIZE];
            int i = 0, charValue;
            while ((charValue = input.read()) != -1) {
                char character = Character.toLowerCase((char) charValue);
                if (isWordChar(character)) {
                    shingle[i++] = character;
                } else {
                    i = 0;
                }

                if (i == SHINGLE_SIZE) {
                    statisticTable.merge(new String(shingle), 1, Integer::sum);
                    System.arraycopy(shingle, 1, shingle, 0, SHINGLE_SIZE - 1);
                    i--;
                }
            }
        }

        List<Map.Entry<String, Integer>> stats = new ArrayList<>(statisticTable.entrySet());
        stats.sort(Map.Entry.comparingByValue());

        try (BufferedWriter output = new BufferedWriter(
            new OutputStreamWriter(new FileOutputStream(outputFileName), "UTF-8"))
        ) {
            for (Map.Entry<String, Integer> entry : stats) {
                output.write(entry.getKey());
                output.write(' ');
                output.write(entry.getValue().toString());
                output.newLine();
            }
        }
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.err.println("Input and output files required as arguments.");
            return;
        }
        try {
            MakeStatistic(args[0], args[1]);
        } catch (FileNotFoundException e) {
            System.err.printf("Input file couldn't be found %s\n", e.getMessage());
        } catch (IOException e) {
            System.err.printf("I/O error: %s\n", e.getMessage());
        }
    }
}
