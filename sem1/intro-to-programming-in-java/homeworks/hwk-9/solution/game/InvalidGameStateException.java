package game;

/**
 * Class defining exceptions thrown by a GameRunner when the game has reached an invalid
 * state and cannot be run any further, meaning that we shouldn't be trying to do any more turns
 * since they will just always cause an error
 */
public class InvalidGameStateException extends Exception {
    public InvalidGameStateException() {
        super();
    }

    /**
     * Construct a more informative message defining what has actually gone wrong
     *
     * @param s the message defining the actual state reached
     */
    public InvalidGameStateException(String s) {
        super(s);
    }

    /**
     * Construct an even more informative message which also tells you how this invalid state has
     * been reached in the cases where it has been caused by other exceptions.
     *
     * @param message the detailed message
     * @param cause   the original cause of the error which then caused this invalid state
     */
    public InvalidGameStateException(String message, Throwable cause) {
        super(message, cause);
    }

    /**
     * Like previous but without an additional message
     */
    public InvalidGameStateException(Throwable cause) {
        super(cause);
    }
}