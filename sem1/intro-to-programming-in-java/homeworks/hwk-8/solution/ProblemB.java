import java.util.Scanner;

public class ProblemB {
    public static void main(String[] args) {
        Scanner stdinScanner = new Scanner(System.in);

        int n = stdinScanner.nextInt();
        int currentNumber = 710 * -25000;
        for (int i = 0; i < n; i++) {
            System.out.println(currentNumber);
            currentNumber += 710;
        }
    }
}
