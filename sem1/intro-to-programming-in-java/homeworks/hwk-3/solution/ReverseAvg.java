import com.sun.jdi.InvalidLineNumberException;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Scanner;
import java.util.Arrays;

public class ReverseAvg {
    public static void matrixAvgCounter(Scanner lineReader) {
        int size = 0;
        int[][] matrix = new int[2][];
        long[] rowSums = new long[2];
        long[] colSums = new long[2];
        int[] colCounts = new int[2];

        while (lineReader.hasNextLine()) {
            if (size == matrix.length) {
                matrix = Arrays.copyOf(matrix, matrix.length * 2);
                rowSums = Arrays.copyOf(rowSums, matrix.length * 2);
            }
            String line = lineReader.nextLine();
            Scanner numberScanner = new Scanner(line);
            int[] numbers = new int[2];
            int vectorSize = 0;
            while ( numberScanner.hasNextInt() ) {
                if (vectorSize == numbers.length) {
                    numbers = Arrays.copyOf(numbers, numbers.length * 2);
                }
                if (vectorSize == colSums.length) {
                    colCounts = Arrays.copyOf(colCounts, colSums.length * 2);
                    colSums = Arrays.copyOf(colSums, colSums.length * 2);
                }
                numbers[vectorSize] = numberScanner.nextInt();
                rowSums[size] += numbers[vectorSize];
                colSums[vectorSize] += numbers[vectorSize];
                colCounts[vectorSize]++;
                vectorSize++;
            }
            matrix[size++] = Arrays.copyOf(numbers, vectorSize);
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < matrix[i].length; j++) {
                System.out.printf("%d ", (rowSums[i] + colSums[j] - matrix[i][j]) /
                        (matrix[i].length + colCounts[j] - 1));
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        Scanner stdinReader = new Scanner(System.in);
        matrixAvgCounter(stdinReader);
    }
}
