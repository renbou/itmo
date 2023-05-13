package expression;

public class Multiply extends AbstractOperation {
    public Multiply(CombinedExpression a, CombinedExpression b) {
        super(a, b);
    }

    @Override
    protected String symbol() {
        return "*";
    }

    @Override
    protected int priority() {
        return 1;
    }

    @Override
    protected boolean commutative() {
        return true;
    }

    @Override
    protected boolean bracketsForIntegerComputation() {
        return false;
    }

    @Override
    protected int compute(int a, int b) {
        return a * b;
    }

    @Override
    protected double compute(double a, double b) {
        return a * b;
    }
}
