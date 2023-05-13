package game;

/**
 * Interface defining the game board, this handles only changing the actual games state with
 * the specified moves, not checking winning or handling move order. This should be given to the player so that they
 * know of the board to make intelligent moves.
 */
public interface Board {
    /**
     * Get the cell at a point on this board
     * @throws IndexOutOfBoundsException if the point is not located on the board
     * @return the cell located at that point on the board
     */
    Cell getCell(final int x, final int y);

    /**
     * Check if a point with such coordinates is part of the board
     * @return true if the point is located on the board, false otherwise
     */
    boolean pointOnBoard(final int x, final int y);

    /**
     * Pretty much a combination of previous functions to check if the move's point is on the board and that point
     * on the board is free, but can also be used to make more checks depending on the game
     * @param move the move which a player would like to make
     * @return true if a player can make a move here, i.e. this position is an empty cell
     */
    boolean validMove(final Move move);

    /**
     * Method to check if the board still has empty cells, players can use this to get out of bad situations
     * when they might be called accidentally by a game to make a move even though the game has ended.
     * @return true if the board still has empty cells, false if there are no free cells left
     */
    boolean hasFreeCells();

    /**
     * Method to get information about the dimensions of the board, since every board is a 2d shape this describes the
     * edge-points of the board.
     * @return two-dimensional array of length 2, describing the x andy edges
     */
    int[] getDimensions();

    /**
     * Method to get a human-readable representation of the board's view, which can be used by players and other classes.
     * @return string describing this board which can be immediately printed
     */
    String getHumanReadable();

    /**
     * Make a move, changing the board's state, using a move object from the player and the "color" (Cell) with which
     * to mark the change on the board
     * @throws IllegalArgumentException if for some reason the cell has a negative integer value, which is bad considering
     *                                  we agree to use positive integers for the player cell values, or if the move is for the
     *                                  wrong game, not supported by this board
     * @throws UnsupportedOperationException if this board doesn't support making moves, i.e. it is immutable
     * @param playerMove the move of a player, the board shouldn't care about whose move it is though
     * @param playerCell the cell type which is used to affect the board
     * @return true if the move has been made, false if such a move couldn't be made in the current board state
     */
    boolean makeMove(final Move playerMove, final Cell playerCell);
}
