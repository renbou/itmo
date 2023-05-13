import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Scanner;
import java.util.Arrays;

public class Reverse {
    public static void ReverseNumbers(Scanner numberScanner) {
        int[] numbers = new int[2];
        int usedSize = 0;
        while ( numberScanner.hasNextInt() ) {
            if (usedSize == numbers.length) {
                numbers = Arrays.copyOf(numbers, numbers.length * 2);
            }
            numbers[usedSize++] = numberScanner.nextInt();
        }

        for (int i = usedSize - 1; i >= 0; i--) {
            System.out.print(numbers[i] + " ");
        }
    }

    public static void ReverseLines(BufferedReader lineReader) throws java.io.IOException {
        String line;
        String[] lines = new String[2];
        int usedSize = 0;

        while ( (line = lineReader.readLine()) != null ) {
            if (usedSize == lines.length) {
                lines = Arrays.copyOf(lines, lines.length * 2);
            }
            lines[usedSize++] = line;
        }

        for (int i = usedSize - 1; i >= 0; i--) {
            ReverseNumbers(new Scanner(lines[i]));
            System.out.println();
        }
    }

    public static void main(String args[]) {
        BufferedReader stdinReader = new BufferedReader(new InputStreamReader(System.in));
        try {
            ReverseLines(stdinReader);
        } catch (java.io.IOException e) {
            System.err.println("Error while reading values from input: " + e.toString());
        }
    }
}
