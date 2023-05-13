package expression;

import java.util.Objects;

public abstract class AbstractOperation implements CombinedExpression {
    protected final CombinedExpression a, b;

    /**
     * Constructs an abstract operation which combines two other expressions
     */
    AbstractOperation(CombinedExpression a, CombinedExpression b) {
        this.a = a;
        this.b = b;
    }

    /**
     * @return a string of length 1 containing the symbol describing this operation
     */
    protected abstract String symbol();

    /**
     * Get the priority of this operation (PEMDAS). This might be better defined using a Map of classes in the
     * future, but currently this is simpler because there aren't many operations.
     *
     * <p>If the operation's priority is the least, that means that usually it would be completed last, and as
     * such might need to be enclosed in brackets. If the operations priority is the greatest then it will be
     * completed first and as such doesn't need brackets (unless it is non-commutative).
     *
     * @return the operation's priority, a non-negative integer
     */
    protected abstract int priority();

    /**
     * Check if the operation is commutative. Current commutative operations are Addition and Multiplication,
     * non-commutative operations are Subtraction and Division. This is also to be used for deciding whether we
     * need brackets or not.
     *
     * @return true if the operation is commutative, false otherwise
     */
    protected abstract boolean commutative();

    /**
     * Check if the operation might require brackets due to us mainly supporting integer computation. What is meant
     * is that (for division as an example) a / (b * c) != a / b * c since the division might round down.
     * @return true if we need brackets for correctly computing integer-only operations
     */
    protected abstract boolean bracketsForIntegerComputation();

    @Override
    public final String toString() {
        return "(" + a.toString() + " " + symbol() + " " + b.toString() + ")";
    }

    /**
     * Check if the other expression's priority is less than this one's, meaning that it would need to be surrounded by
     * brackets in order to retain the same order.
     *
     * @param other one of the expressions being combined by this operation
     * @return true if the other operation's priority is smaller than this one's
     */
    private boolean lowerPriority(CombinedExpression other) {
        return other instanceof AbstractOperation && ((AbstractOperation) other).priority() < priority();
    }

    /**
     * Check if the other expression would need brackets due to this operation being ordered and of the same order
     * as the other operation. This is needed, for example, when we have a / (b * c), which wouldn't be equal to
     * a / b * c == (a / b) * c.
     *
     * <p>Technically the check here should be {@code otherOperation.priority() <= priority()}, but since we should
     * check for lower priority first anyways, there's no point in the "less" part.
     *
     * @param other the right expression being combined by this operation
     * @return true if brackets should be placed due to this operation being non-commutative
     */
    private boolean needsBracketsDueToOrder(CombinedExpression other) {
        if (other instanceof AbstractOperation) {
            AbstractOperation otherOperation = (AbstractOperation) other;
            return (!commutative() || otherOperation.bracketsForIntegerComputation())
                    && otherOperation.priority() == priority();
        }
        return false;
    }

    /**
     * Formats the expression in brackets if needed. This is a helper method so that we don't need to place these
     * ternary operators everywhere multiple times.
     *
     * @return the proper miniString representation of the expression, depending on the current operation
     */
    private String formatExpression(CombinedExpression expression, boolean wrapInBrackets) {
        String representation = expression.toMiniString();
        return wrapInBrackets ? "(" + representation + ")" : representation;
    }

    @Override
    public final String toMiniString() {
        return formatExpression(a, lowerPriority(a)) + " " + symbol() + " "
                + formatExpression(b, lowerPriority(b) || needsBracketsDueToOrder(b));
    }

    @Override
    public final boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof AbstractOperation)) return false;
        AbstractOperation other = (AbstractOperation) obj;
        return getClass().equals(other.getClass()) && a.equals(other.a) && b.equals(other.b);
    }

    @Override
    public final int hashCode() {
        return Objects.hash(a, b, getClass());
    }

    /**
     * Calculate this operation for integer parameters.
     */
    protected abstract int compute(int a, int b);

    /**
     * Calculate this operation for double precision parameters.
     */
    protected abstract double compute(double a, double b);

    @Override
    public double evaluate(double x) {
        return compute(a.evaluate(x), b.evaluate(x));
    }

    @Override
    public int evaluate(int x) {
        return compute(a.evaluate(x), b.evaluate(x));
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return compute(a.evaluate(x, y, z), b.evaluate(x, y, z));
    }
}
