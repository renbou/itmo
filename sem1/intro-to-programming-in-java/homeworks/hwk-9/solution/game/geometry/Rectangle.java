package game.geometry;

/**
 * Cartesian two-dimensional closed rectangle with sides n and m
 * This can also be used to implement pretty much any two dimensional shape with the cartesian coordinate system,
 * simply by overriding the inBounds method
 */
public class Rectangle extends AbstractShape {
    private int[][] points;

    /**
     * Creates a rectangular shape with n rows and m columns
     * @throws IllegalArgumentException if one of the parameters is not a positive number
     */
    public Rectangle(final int n, final int m) {
        if (n < 1 || m < 1) {
            throw new IllegalArgumentException("Cannot create rectangle shape with null or negative sized sides.");
        }

        points = new int[n][m];
    }

    protected boolean inBounds(final int x, final int y) {
        return x >= 0 && x < points.length && y >= 0 && y < points[0].length;
    }

    protected int internalGetPoint(final int x, final int y) {
        if (!inBounds(x, y)) {
            return -1;
        }
        return points[x][y];
    }

    protected int internalSetPoint(final int x, final int y, final int value) {
        if (!inBounds(x, y)) {
            return -1;
        }
        points[x][y] = value;
        return 1;
    }

    public int[] getBounds() {
        return new int[]{points.length, points[0].length};
    }

    public int numberOfPoints() {
        return points.length * points[0].length;
    }
}
