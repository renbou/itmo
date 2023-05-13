import java.io.*;
import java.util.*;

public class WordStatIndex {
    public static void MakeStatistic(FastScanner scanner, BufferedWriter output) throws IOException {
        int wordTokenType = scanner.addMatcher(
                new FastScanner.Matcher(
                        new FastScanner.MatchGroup("\'", new byte[]{
                                Character.UPPERCASE_LETTER, Character.LOWERCASE_LETTER, Character.TITLECASE_LETTER,
                                Character.MODIFIER_LETTER, Character.OTHER_LETTER, Character.DASH_PUNCTUATION
                        }, false, 1, FastScanner.MatchGroup.MAX_MATCH_SIZE, false)
                ));
        Map<String, IntList> statisticTable = new LinkedHashMap<>();
        int wordCnt = 1;
        while (scanner.hasNextLine()) {
            scanner.toggleDelimiter();
            scanner.toggleOnlyCachedMode();
            while(scanner.hasNextToken(wordTokenType)) {
                String word = scanner.nextToken(wordTokenType).toLowerCase();
                statisticTable.merge(word, IntList.intListOf(wordCnt++), IntList::append);
            }
            scanner.toggleDelimiter();
            scanner.toggleOnlyCachedMode();
        }

        for (Map.Entry<String, IntList> entry: statisticTable.entrySet()) {
            IntList current = entry.getValue();
            output.write(entry.getKey());
            output.write(' ');
            output.write(String.valueOf(current.size()));
            for (Integer i : current) {
                output.write(' ');
                output.write(i.toString());
            }
            output.newLine();
        }
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.err.println("Input and output files required as arguments.");
            return;
        }
        try (
                FastScanner scanner = new FastScanner(
                        new InputStreamReader(new FileInputStream(args[0]), "UTF-8"));
                BufferedWriter out = new BufferedWriter(
                        new OutputStreamWriter(new FileOutputStream(args[1]), "UTF-8"))
        ) {
            MakeStatistic(scanner, out);
        } catch (FileNotFoundException e) {
            System.err.printf("Input file couldn't be found %s\n", e.getMessage());
        } catch (IOException e) {
            System.err.printf("I/O error: %s\n", e.getMessage());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
