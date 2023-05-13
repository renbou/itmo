package game;

/**
 * Interface defining classes which are meant to run game turns on their board
 * Classes implementing this should not care about what player is making their turn, their sole purpose
 * is to make that move and check if the board's state changes to an ending state depending on the game.
 */
public interface GameTurnRunner {
    /**
     * @throws UnsupportedOperationException if this game operates differently, i.e. needs different information
     *                                       to make another turn (for example a more game-specific Player class)
     * @throws IllegalStateException if this method has been called at an unsuitable time, i.e. after the game has ended
     * @throws InvalidGameStateException if the game has reached an invalid state and can no longer continue,
     *                                   don't call this method after this error has been throw
     * @param player the player whose turn it currently is, according to the caller of this function
     * @param playerCell the cell to make the current player's turn with
     * @return the resulting state of the current board/game
     */
    GameState doTurn(final Player player, final Cell playerCell) throws InvalidGameStateException;

    /**
     * @return true if the game is still running, false if it cannot be continued or has ended
     */
    boolean isRunning();

    /**
     * Method to generate a human-readable representation of the state of the game, this can be different for different games
     * but should probably represent the board and have something along the lines of "Game has ended" or "Game is running"
     * @return the string which can be immediately printed
     */
    String getHumanReadableState();
}
