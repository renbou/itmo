package game;

/**
 * Interface defining a player type, and since a player doesn't really need to do anything except make
 * turns depending on their view of the board, move is the only meaningful function we can make here.
 */
public interface Player {
    /**
     * Ask the player to make a move given this view of the board. This method MUST NOT throw any exceptions
     * except the ones defined here, and if it does then the game's behaviour is undefined
     * @throws IllegalStateException if there's no place on the board to make a move
     * @param board the player's view of the board
     * @return the player's move
     */
    Move makeMove(final Board board);
}
