package expression;

public class Variable implements CombinedExpression {
    private final String variable;

    public Variable(String variable) {
        this.variable = variable;
    }

    @Override
    public String toString() {
        return variable;
    }

    @Override
    public String toMiniString() {
        return variable;
    }

    @Override
    public boolean equals(Object obj) {
        return obj instanceof Variable && variable.equals(((Variable)obj).variable);
    }

    @Override
    public int hashCode() {
        return variable.hashCode();
    }

    @Override
    public double evaluate(double x) {
        return x;
    }

    @Override
    public int evaluate(int x) {
        return x;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        switch (variable) {
            case "x":
                return x;
            case "y":
                return y;
            default:
                return z;
        }
    }
}
