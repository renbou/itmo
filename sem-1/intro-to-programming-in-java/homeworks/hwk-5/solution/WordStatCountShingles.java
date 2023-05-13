import java.io.*;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.Map;

class WordStatistic implements Comparable<WordStatistic> {
    String word;
    int count;

    public WordStatistic(String word_t, int count_t) {
        word = word_t;
        count = count_t;
    }

    public int compareTo(WordStatistic word) {
        if (count != word.count) {
            return count < word.count ? -1 : 1;
        }
        return 0;
    }
}

public class WordStatCountShingles {
    public static void MakeStatistic(BufferedReader input, BufferedWriter output) throws IOException {
        FastScanner scanner = new FastScanner(input);
        scanner.toggleDelimiter();
        int wordTokenType = scanner.addMatcher(
                new FastScanner.Matcher(
                        new FastScanner.MatchGroup("\'", new byte[]{
                                Character.UPPERCASE_LETTER, Character.LOWERCASE_LETTER, Character.TITLECASE_LETTER,
                                Character.MODIFIER_LETTER, Character.OTHER_LETTER, Character.DASH_PUNCTUATION
                        }, false, 1, FastScanner.MatchGroup.MAX_MATCH_SIZE, false)
                ));

        Map<String, Integer> statisticTable = new LinkedHashMap<String, Integer>();
        while (scanner.hasNextToken(wordTokenType)) {
            char[] word = scanner.nextToken(wordTokenType).toLowerCase().toCharArray();
            if (word.length < 3) {
                continue;
            }

            char[] shingle = new char[3];
            shingle[0] = word[0];
            shingle[1] = word[1];

            for (int i = 2; i < word.length; i++) {
                shingle[2] = word[i];
                statisticTable.merge(new String(shingle), 1, Integer::sum);
                shingle[0] = shingle[1];
                shingle[1] = shingle[2];
            }
        }

        WordStatistic[] stats = new WordStatistic[statisticTable.entrySet().size()];
        int index = 0;
        for (Map.Entry<String, Integer> entry: statisticTable.entrySet()) {
            stats[index] = new WordStatistic(entry.getKey(), entry.getValue());
            index += 1;
        }
        Arrays.sort(stats);

        for (WordStatistic entry : stats) {
            output.write(entry.word);
            output.write(' ');
            output.write(String.valueOf(entry.count));
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
