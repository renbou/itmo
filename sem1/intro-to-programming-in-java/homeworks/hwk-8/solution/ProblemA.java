import java.util.Scanner;
import java.lang.Math;

public class ProblemA {
    public static void main(String[] args) {
        Scanner stdinScanner = new Scanner(System.in);
        int a = stdinScanner.nextInt();
        int b = stdinScanner.nextInt();
        int n = stdinScanner.nextInt();

        int moveLength = b - a;
        int totalMoveLength = n - b;
        int totalMoves = 2 * ((totalMoveLength + moveLength - 1) / moveLength) + 1;
        System.out.println(totalMoves);
    }
}
