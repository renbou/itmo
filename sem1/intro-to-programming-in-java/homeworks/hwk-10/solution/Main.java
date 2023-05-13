package expression;

public class Main {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Unable to compute expression with no x variable value specified.");
            return;
        }

        final double x;
        try {
            x = Double.parseDouble(args[0]);
        } catch (NumberFormatException exception) {
            System.out.println("Cannot evaluate exception with non-number argument.");
            return;
        }


        CombinedExpression expression = new Add(
                new Subtract(
                        new Multiply(
                                new Variable("x"),
                                new Variable("x")
                        ),
                        new Multiply(
                                new Const(2),
                                new Variable("x")
                        )),
                new Const(1)
        );

        System.out.println("Full-bracket representation: " + expression.toString());
        System.out.println("Mini-bracket representation: " + expression.toMiniString());
        System.out.println("Expression value = " + expression.evaluate(x));
    }
}
