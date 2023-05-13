import java.io.*;
import java.util.*;

public class ProblemH {
    public static void main(String[] args) throws IOException {
        BufferedReader stdinReader = new BufferedReader(
                new InputStreamReader(System.in));
        BufferedWriter stdoutWriter = new BufferedWriter(
                new OutputStreamWriter(System.out)
        );

        int n = Integer.parseInt(stdinReader.readLine());
        int[] numQueries = new int[n];
        int[] cumulativeNumQueries = new int[n];
        int maxNumQueries = Integer.MIN_VALUE;
        int sumNumQueries = 0;

        cumulativeNumQueries[0] = 0;
        String[] numQueriesStrings = stdinReader.readLine().split(" ");
        for (int i = 0; i < n; i++) {
            numQueries[i] = Integer.parseInt(numQueriesStrings[i]);
            if (numQueries[i] > maxNumQueries) {
                maxNumQueries = numQueries[i];
            }
            if (i > 0) {
                cumulativeNumQueries[i] = cumulativeNumQueries[i - 1] + numQueries[i - 1];
            }
        }
        sumNumQueries = cumulativeNumQueries[n - 1] + numQueries[n - 1];

        int[] transactionNForQueryI = new int[sumNumQueries];
        int cumulativeI = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < numQueries[i]; j++, cumulativeI++) {
                transactionNForQueryI[cumulativeI] = i;
            }
        }

        int[] answers = new int[sumNumQueries + 1];
        for (int t = maxNumQueries; t <= sumNumQueries; t++) {
            int requiredQueries = 0;
            int currentBlock;
            for (currentBlock = 0;
                 cumulativeNumQueries[currentBlock] + t < sumNumQueries;
                 currentBlock = transactionNForQueryI[cumulativeNumQueries[currentBlock] + t]) {
                requiredQueries++;
            }
            requiredQueries++;
            answers[t] = requiredQueries;
        }

        int q = Integer.parseInt(stdinReader.readLine());
        String[] testsStrings = stdinReader.readLine().split(" ");
        for (int i = 0; i < q; i++) {
            int t = Integer.parseInt(testsStrings[i]);
            if (t < maxNumQueries) {
                stdoutWriter.write("Impossible");
                stdoutWriter.newLine();
                continue;
            }

            stdoutWriter.write(String.valueOf(answers[t]));
            stdoutWriter.newLine();
        }
        stdoutWriter.flush();
    }
}
