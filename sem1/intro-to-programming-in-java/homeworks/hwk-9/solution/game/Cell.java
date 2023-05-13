package game;

/**
 * Enumeration defining all the possible states of every cell
 */
public enum Cell {
    EMPTY {
        @Override
        public String toString() {
            return " ";
        }
    },
    X {
        @Override
        public String toString() {
            return "X";
        }
    },
    O {
        @Override
        public String toString() {
            return "O";
        }
    };

    // Stores all of the cell values for fast conversion from int to cell
    static private Cell[] allValues = null;

    /**
     * Static method to give the cell corresponding to an integer value. Since this is pretty much reverse of toInteger,
     * they should be implemented accordingly.
     * @throws IllegalArgumentException if a cell with such integer key doesn't exist
     */
    public static Cell fromInteger(int cellValue) {
        if (allValues == null) {
            allValues = Cell.values();
        }
        if (cellValue < 0 || cellValue >= allValues.length) {
            throw new IllegalArgumentException("Cell with such integer key does not exist.");
        }
        return allValues[cellValue];
    }

    /**
     * Method to return the integer value corresponding to a cell, this is supposed to be the opposite of fromInteger,
     * so implement them accordingly.
     *
     * Non-empty cell values must return a *positive* integer here,
     * since we agreed that all values on board field must be positive.
     *
     * The value of the empty cell must be 0.
     */
    public int toInteger() {
        return this.ordinal();
    }
}
