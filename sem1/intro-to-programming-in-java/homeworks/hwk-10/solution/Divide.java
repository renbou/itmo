package expression;

public class Divide extends AbstractOperation {
    public Divide(CombinedExpression a, CombinedExpression b) {
        super(a, b);
    }

    @Override
    protected String symbol() {
        return "/";
    }

    @Override
    protected int priority() {
        return 1;
    }

    @Override
    protected boolean commutative() {
        return false;
    }

    @Override
    protected boolean bracketsForIntegerComputation() {
        return true;
    }

    @Override
    protected int compute(int a, int b) {
        return a / b;
    }

    @Override
    protected double compute(double a, double b) {
        return a / b;
    }
}
