package game.geometry;

public abstract class AbstractShape {
    /// Null until is requested from this shape, used to always return the same instance of the view
    /// so that we don't create the same read-only view a million times.
    private AbstractShape unmodifiableView = null;

    /**
     * Check if a point is in this shape.
     * @return true if the point described by coordinates x, y is in the bounds of this rectangle, false otherwise
     */
    protected abstract boolean inBounds(final int x, final int y);

    /**
     * Get the value at the point described by x and y inside of this shape.
     * @return -1 (or anything <0) if the point is OutOfBounds,
     *         0 if the given point is not yet set, otherwise a positive integer
     */
    protected abstract int internalGetPoint(final int x, final int y);

    /**
     * Set the value at the point described by x and y inside of this shape to a value
     * @return @return -1 (or anything <0) if the point is OutOfBounds, any positive or zero value otherwise
     */
    protected abstract int internalSetPoint(final int x, final int y, final int value);

    /**
     * Get the edge-points of this shape, and since the shape should always start from (0, 0) we pretty much just
     * return the size of the bounding rectangle.
     */
    public abstract int[] getBounds();

    /**
     * Get the number of points this shape contains
     */
    public abstract int numberOfPoints();

    /**
     * Check if the point is inside this shape
     * @return true if the point is part of this shape
     */
    public boolean containsPoint(final int x, final int y) {
        return internalGetPoint(x, y) != -1;
    }

    /**
     * Gets the value set at the point described by the coordinates x y
     * @throws IndexOutOfBoundsException if the point is not part of this rectangle
     * @return *positive* integer - the value at the given point, 0 if nothing was previously set at this location
     */
    public int getPointValue(final int x, final int y) {
        int result = internalGetPoint(x, y);
        if (result < 0) {
            throw new IndexOutOfBoundsException(
                    "Cannot get value of point which is not inside of the bounds of the shape, check with containsPoint first.");
        }
        return result;
    }

    /**
     * Sets the value at the point described by the coordinates x y
     * @throws UnsupportedOperationException if this rectangle is read-only, doesn't support this operation
     * @throws IllegalArgumentException if the value isn't a positive integer
     * @throws IndexOutOfBoundsException if the point is not part of this field
     * @param value the value to set, must be a *positive* integer
     */
    public void setPointValue(final int x, final int y, final int value) {
        if (value < 1) {
            throw new IllegalArgumentException("Cannot set point value to a non-positive integer.");
        }

        int result = internalSetPoint(x, y, value);
        if (result < 0) {
            throw new IndexOutOfBoundsException(
                    "Cannot set value of point which is not inside of the bounds of the shape, check with containsPoint first.");
        }
    }

    /**
     * Helper class to describe immutable rectangles.
     */
    private abstract class ImmutableShape extends AbstractShape {
        /// We do not need these methods anyway
        @Override
        protected boolean inBounds(int x, int y) {
            return false;
        }

        @Override
        protected int internalGetPoint(int x, int y) {
            return 0;
        }

        @Override
        protected int internalSetPoint(int x, int y, int value) {
            return 0;
        }

        /// Can't set points in immutable shape
        public void setPointValue(final int x, final int y, final int value) {
            throw new UnsupportedOperationException(
                    "Unmodifiable view of a rectangle can't set points on that rectangle");
        }

        public AbstractShape getUnmodifiableView() {
            return this;
        }
    }

    /**
     * Method to return an unmodifiable view of the rectangle, meaning that changes to this rectangle are to be reflected
     * in the returned view of the rectangle. The returned rectangle shouldn't have setPointValue implemented, and
     * instead have it throw UnsupportedOperationException.
     * @return a read-only view of this field
     */
    public AbstractShape getUnmodifiableView() {
        if (unmodifiableView == null) {
            unmodifiableView = new ImmutableShape() {
                protected boolean inBounds(int x, int y) {
                    return AbstractShape.this.inBounds(x, y);
                }

                public boolean containsPoint(final int x, final int y) {
                    return AbstractShape.this.containsPoint(x, y);
                }

                public int getPointValue(final int x, final int y) {
                    return AbstractShape.this.getPointValue(x, y);
                }

                public int[] getBounds() {
                    return AbstractShape.this.getBounds();
                }

                public int numberOfPoints() {
                    return AbstractShape.this.numberOfPoints();
                }
            };
        }
        return unmodifiableView;
    }
}
