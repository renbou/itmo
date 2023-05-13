package game.geometry;

/**
 * Cartesian two-dimensional closed rhombus with side d
 */
public class Rhombus extends AbstractShape {
    private final int d;
    private int[][] points;

    /**
     * Creates a rhombus shape with side d
     * @throws IllegalArgumentException if d is not a positive number
     */
    public Rhombus(final int d) {
        if (d < 1) {
            throw new IllegalArgumentException("Cannot create rectangle shape with null or negative sized sides.");
        }

        points = new int[d * 2 - 1][];
        for (int i = 1; i <= points.length; i++) {
            points[i - 1] = new int[(d - Math.abs(d - i)) * 2 - 1];
        }

        this.d = d;
    }

    @Override
    protected boolean inBounds(final int x, final int y) {
        return Math.abs(d - x - 1) + Math.abs(d - y - 1) < d;
    }

    @Override
    protected int internalGetPoint(final int x, final int y) {
        if (!inBounds(x, y)) {
            return -1;
        }
        return points[x][y - Math.abs(d - x - 1)];
    }

    @Override
    protected int internalSetPoint(final int x, final int y, final int value) {
        if (!inBounds(x, y)) {
            return -1;
        }
        points[x][y - Math.abs(d - x - 1)] = value;
        return 1;
    }

    @Override
    // Rhombus has n*n + (n-1)*(n-1) points, so we can simplify that for less multiplications
    public int numberOfPoints() {
        return 2*(d*d - d) + 1;
    }

    @Override
    public int[] getBounds() {
        return new int[]{points.length, points.length};
    }
}
