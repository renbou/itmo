package game;

/**
 * Interface defining a game class which controls the order of player turns, the number of players,
 * as well as any outputting and pretty much anything else needed
 */
public interface Game {
    /**
     * Starts up the game and runs it using the previously added players until the game has finished,
     * so you might want to run this on a different thread if you want to run a couple games in parallel
     * @throws IllegalStateException if the game has not yet been initialized with enough playerss
     * @return the number of the player who won the game, 0 if the game resulted in a draw, or -1 if the game ended badly
     * (however different games might possibly have more return values to specify the outcome, but they should be a negative int)
     */
    int run();

    /**
     * Add this player to the current game.
     * @throws IllegalStateException if there are already enough players added to the game
     * @throws NullPointerException if the passed player object is a null
     */
    void addPlayer(Player player);
}
