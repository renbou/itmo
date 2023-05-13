import java.io.*;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.Map;

public class WordStatInput {
    public static boolean isWordChar(char c) {
        return Character.isLetter(c) || c == '\'' || Character.getType(c) == Character.DASH_PUNCTUATION;
    }

    public static void MakeStatistic(BufferedReader input, BufferedWriter output) throws IOException {
        boolean running = true;
        Map<String, Integer> statisticTable = new LinkedHashMap<String, Integer>();
        while (running) {
            StringBuilder currentWord = new StringBuilder();
            while (true) {
                int charValue = input.read();
                if (charValue == -1) {
                    running = false;
                    break;
                }

                char character = (char)charValue;
                if (isWordChar(character)) {
                    currentWord.append(character);
                } else if (currentWord.length() != 0) {
                    statisticTable.merge(currentWord.toString().toLowerCase(), 1, Integer::sum);
                    break;
                }
            }
        }

        for (Map.Entry<String, Integer> entry: statisticTable.entrySet()) {
            output.write(entry.getKey());
            output.write(' ');
            output.write(String.valueOf(entry.getValue()));
            output.newLine();
        }
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.err.println("Input and output files required as arguments.");
            return;
        }
        try (
                BufferedReader in = new BufferedReader(
                        new InputStreamReader(new FileInputStream(args[0]), "UTF-8"));
                BufferedWriter out = new BufferedWriter(
                        new OutputStreamWriter(new FileOutputStream(args[1]), "UTF-8"))
        ) {
            MakeStatistic(in, out);
        } catch (FileNotFoundException e) {
            System.err.printf("Input file couldn't be found %s\n", e.getMessage());
        } catch (IOException e) {
            System.err.printf("I/O error: %s\n", e.getMessage());
        }
    }
}
