public class Sum {
    public static void main(String args[]) {
        int sum = 0, i, start;
        for (String argument: args) {
            for (i = 0; i < argument.length(); i++) {
                if ( Character.isWhitespace(argument.charAt(i)) )
                    continue;
                start = i;
                while ( (i < argument.length()) && !(Character.isWhitespace(argument.charAt(i))) ) {
                    i++;
                }
                sum += Integer.parseInt(argument, start, i, 10);
            }
        }
        System.out.println(sum);
    }
}
