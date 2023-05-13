import java.math.BigInteger;

public class SumBigIntegerSpace {
    public static boolean isSpace(char a) {
        return Character.getType(a) == Character.SPACE_SEPARATOR;
    }

    public static void main(String[] args) {
        BigInteger sum = BigInteger.ZERO;
        for (String arg : args) {
            for (int i = 0; i < arg.length(); i++) {
                int start = i;
                while (i < arg.length() && !isSpace(arg.charAt(i))) {
                    i++;
                }
                if (start < i) {
                    sum = sum.add(new BigInteger(arg.substring(start, i)));
                }
            }
        }
        System.out.println(sum);
    }
}
