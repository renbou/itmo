import java.io.*;
import java.util.*;

public class WordStatCountFirstIndex {
    public static void MakeStatistic(String inputFileName, String outputFileName) throws Exception {
        Map<String, WordCountList> statisticTable = new LinkedHashMap<>();

        try(FastScanner scanner = new FastScanner(
                new InputStreamReader(new FileInputStream(inputFileName), "UTF-8"))
        ) {
            int wordTokenType = scanner.addMatcher(
                    new FastScanner.Matcher(
                            new FastScanner.MatchGroup("\'", new byte[]{
                                    Character.UPPERCASE_LETTER, Character.LOWERCASE_LETTER, Character.TITLECASE_LETTER,
                                    Character.MODIFIER_LETTER, Character.OTHER_LETTER, Character.DASH_PUNCTUATION
                            }, false, 1, FastScanner.MatchGroup.MAX_MATCH_SIZE, false)
                    ));

            int lineCnt = 1;
            while (scanner.hasNextLine()) {
                int wordIndex = 1;
                scanner.toggleDelimiter();
                scanner.toggleOnlyCachedMode();

                while(scanner.hasNextToken(wordTokenType)) {
                    String word = scanner.nextToken(wordTokenType).toLowerCase();
                    statisticTable.merge(word, new WordCountList(lineCnt, wordIndex), WordCountList::mergeLists);
                    wordIndex++;
                }

                scanner.toggleDelimiter();
                scanner.toggleOnlyCachedMode();
                lineCnt++;
            }
        }

        List<Map.Entry<String, WordCountList>> stats = new ArrayList<>(statisticTable.entrySet());
        stats.sort(Map.Entry.comparingByValue());

        try(BufferedWriter output = new BufferedWriter(
                new OutputStreamWriter(new FileOutputStream(outputFileName), "UTF-8"))
        ) {
            for (Map.Entry<String, WordCountList> entry: stats) {
                WordCountList current = entry.getValue();
                output.write(entry.getKey());
                output.write(' ');
                output.write(String.valueOf(current.getTotalCount()));
                for (Integer i : current.getUnderlyingList()) {
                    output.write(' ');
                    output.write(i.toString());
                }
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
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
