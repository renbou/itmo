import java.io.*;
import java.util.Arrays;

public class Reverse {
    public static void ReverseLines(FastScanner scanner, BufferedWriter outWriter) throws IOException {
        int[][] numbers = new int[2][];
        int usedSize = 0;
        while (scanner.hasNextLine()) {
            scanner.toggleOnlyCachedMode();
            if (usedSize == numbers.length) {
                numbers = Arrays.copyOf(numbers, numbers.length << 1);
            }
            numbers[usedSize] = new int[256];
            int currentSize = 0;
            while (scanner.hasNextInt()) {
                if (currentSize == numbers[usedSize].length) {
                    numbers[usedSize] = Arrays.copyOf(numbers[usedSize], numbers[usedSize].length << 1);
                }
                numbers[usedSize][currentSize++] = scanner.nextInt();
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

    public static void main(String args[]) {
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
