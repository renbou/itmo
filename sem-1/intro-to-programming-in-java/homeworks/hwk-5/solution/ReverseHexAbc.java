import java.io.*;
import java.util.Arrays;

public class ReverseHexAbc {
    public static void ReverseLines(FastScanner scanner, BufferedWriter outWriter) throws IOException {
        FastScanner.MatchGroup hexNumbers = new FastScanner.MatchGroup(
                "-+", null, false, 0, 1, false
        ).setNext(new FastScanner.MatchGroup(
            "0", null, false, 1, 1, false
        )).setNext(new FastScanner.MatchGroup(
                "xX", null, false, 1, 1, false
        )).setNext(new FastScanner.MatchGroup(
                "0123456789abcdef", null, false, 1,
                FastScanner.MatchGroup.MAX_MATCH_SIZE, false
        ));

        FastScanner.MatchGroup abcNumbers = new FastScanner.MatchGroup(
                "-+", null, false, 0, 1, false
        ).setNext(new FastScanner.MatchGroup(
                "abcdefghij", null, false, 1,
                FastScanner.MatchGroup.MAX_MATCH_SIZE, false
        ));

        int hexNumberGroup = scanner.addMatcher(new FastScanner.Matcher(hexNumbers));
        int abcNumberGroup = scanner.addMatcher(new FastScanner.Matcher(abcNumbers));

        int[][] numbers = new int[2][];
        int usedSize = 0;
        while (scanner.hasNextLine()) {
            scanner.toggleOnlyCachedMode();
            if (usedSize == numbers.length) {
                numbers = Arrays.copyOf(numbers, numbers.length << 1);
            }
            numbers[usedSize] = new int[256];
            int currentSize = 0;
            while (true) {
                int curValue;
                if (scanner.hasNextInt()) {
                    curValue = scanner.nextInt();
                } else if (scanner.hasNextToken(hexNumberGroup)) {
                    String val = scanner.nextToken(hexNumberGroup);
                    curValue = Integer.parseUnsignedInt(val.substring(2), 16);
                } else if (scanner.hasNextToken(abcNumberGroup)) {
                    char[] val = scanner.nextToken(abcNumberGroup).toCharArray();
                    for (int i = (val.length > 1 && (val[0] == '+' || val[0] == '-')) ? 1 : 0; i < val.length; i++) {
                        val[i] -= 49;
                    }
                    curValue = Integer.parseInt(String.valueOf(val));
                } else {
                    break;
                }
                if (currentSize == numbers[usedSize].length) {
                    numbers[usedSize] = Arrays.copyOf(numbers[usedSize], numbers[usedSize].length << 1);
                }
                numbers[usedSize][currentSize++] = curValue;
            }
            numbers[usedSize] = Arrays.copyOf(numbers[usedSize], currentSize);
            usedSize++;
            scanner.toggleOnlyCachedMode();
        }

        for (int i = usedSize - 1; i >= 0; i--) {
            for (int j = numbers[i].length - 1; j >= 0; j--) {
                outWriter.write(numbers[i][j] + " ");
            }
            outWriter.newLine();
        }
    }

    public static void main(String[] args) {
        try (
                FastScanner scanner = new FastScanner(new InputStreamReader(System.in));
                BufferedWriter stdoutWriter = new BufferedWriter(new OutputStreamWriter(System.out));
        ) {
            ReverseLines(scanner, stdoutWriter);
            stdoutWriter.flush();
        } catch (java.io.IOException e) {
            System.err.println("Error while reading values from input: " + e.toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
