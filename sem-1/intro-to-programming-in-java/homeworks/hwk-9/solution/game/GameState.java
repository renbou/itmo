package game;

/**
 * Enumeration defining the possible current state of the board/game, this is pretty much the
 * internal state of a GameRunner
 */
public enum GameState {
    WIN, /// last player won
    DRAW, /// the game is a draw after the last turn
    RUNNING, /// the game is still good and running
    BONUS_TURN, /// the player received a bonus turn
    INVALID /// the game reached an invalid state and can no longer continue
}
