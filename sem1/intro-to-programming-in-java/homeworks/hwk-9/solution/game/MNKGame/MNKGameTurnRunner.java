package game.MNKGame;

import java.util.List;
import game.*;

/**
 * The game runner for an MNK game, containing a single board and running until k-in-a-row
 */
public class MNKGameTurnRunner implements GameTurnRunner {
    /// Length of line for bonus turn
    private final static int BONUS_TURN_K = 4;

    /// Make sure that the board is an MNKBoard, so that we don't have to check the figure or the dimensions constantly
    private final MNKBoard board;
    private final int k;
    /// If the bonus turn modification (if player turn results in a new k=4 sequence) is enabled
    private final boolean enableBonusTurns;

    private GameState state = GameState.RUNNING;

    public MNKGameTurnRunner(final MNKBoard board, final int k, final boolean enableBonusTurns) {
        this.board = board;
        this.k = k;
        this.enableBonusTurns = enableBonusTurns;

        /// If we have enabled bonus turns but they could never be reached
        if (enableBonusTurns && k < BONUS_TURN_K) {
            throw new IllegalArgumentException("An MNK game with bonus turns where k is less than the required "
                    + "length for a bonus turn (=" + BONUS_TURN_K + ") will never result in a bonus turn.");
        }
    }

    /**
     * Check if the point on the board belongs to the current player.
     */
    private boolean checkPoint(final int x, final int y, final Cell playerCell) {
        return board.pointOnBoard(x, y) && board.getCell(x, y) == playerCell;
    }

    /**
     * Helper method to be used by countLineLength and countNewLineLength, because the latter needs to know about
     * the lengths of the left and right sides in order to decide correctly. This doesn't count the point (x,y) itself,
     * only to the left and right of it.
     */
    private int[] countLeftAndRightLineLength(final int x, final int y, final int dx, final int dy, final int k,
                                              final Cell playerCell) {
        int leftLineLength = 0;
        for (int t = -1; t >= -k; t--) {
            int curX = x + t * dx;
            int curY = y + t * dy;
            if (checkPoint(curX, curY, playerCell)) {
                leftLineLength++;
            } else {
                break;
            }
        }

        int rightLineLength = 0;
        for (int t = 1; t <= k; t++) {
            int curX = x + t * dx;
            int curY = y + t * dy;
            if (checkPoint(curX, curY, playerCell)) {
                rightLineLength++;
            } else {
                break;
            }
        }

        return new int[]{leftLineLength, rightLineLength};
    }

    /**
     * Counts the length of the line colored by playerCell starting from (x,y) on the board and
     * going with the step (dx, dy), until at least length k is reached. If the line's length is actually >k, then it
     * will return an integer >k, if line length is <k, will return an int <k, if it is equal to k will return k.
     * @param x the x coordinate of where to start counting from
     * @param y the y coordinate of where to start counting from
     * @param dx the change in x describing the line
     * @param dy the change in y describing the line
     * @param k the *desired* length of the line, so that we don't count infinitely
     * @return the length of the line marked by the given playerCell
     */
    private int countLineLength(final int x, final int y, final int dx, final int dy, final int k,
                                       final Cell playerCell) {
        int[] leftAndRightSegmentLengths = countLeftAndRightLineLength(x, y, dx, dy, k, playerCell);
        /// left segment + current point + right segment
        int lineLength = leftAndRightSegmentLengths[0] + 1 + leftAndRightSegmentLengths[1];
        return lineLength;
    }

    /**
     * Like countLineLength, but counts the length of a *new* line (i.e. if the lines before marking the point (x,y)
     * weren't already of length k or more). If there is no new line it will return 0.
     */
    private int countNewLineLength(final int x, final int y, final int dx, final int dy, final int k,
                                final Cell playerCell) {
        int[] leftAndRightSegmentLengths = countLeftAndRightLineLength(x, y, dx, dy, k, playerCell);

        /// If the left and right parts were already proper lines of length k or more
        if (leftAndRightSegmentLengths[0] >= k || leftAndRightSegmentLengths[1] >= k) {
            return 0;
        }

        /// left segment + current point + right segment
        int lineLength = leftAndRightSegmentLengths[0] + 1 + leftAndRightSegmentLengths[1];
        return lineLength;
    }

    /**
     * Counts the maximum line length of color playerCell starting from x, y. Goes up until k+1, in order to identify
     * if the length is actually longer or equal to k.
     */
    private int countMaxLineLength(final int x, final int y, final int k, final Cell playerCell) {
        int maxLineLength = List.of(
                countLineLength(x, y, 1, 0, k, playerCell),
                countLineLength(x, y, 0, 1, k, playerCell),
                countLineLength(x, y, 1, 1, k, playerCell),
                countLineLength(x, y, -1, 1, k, playerCell)).stream().max(Integer::compare).get();
        return maxLineLength;
    }

    /**z
     * Counts the maximum *new* line length of color playerCell starting from x, y. Goes up until k+1, in order to identify
     * if the length is actually longer or equal to k.
     */
    private int countMaxNewLineLength(final int x, final int y, final int k, final Cell playerCell) {
        int maxLineLength = List.of(
                countNewLineLength(x, y, 1, 0, k, playerCell),
                countNewLineLength(x, y, 0, 1, k, playerCell),
                countNewLineLength(x, y, 1, 1, k, playerCell),
                countNewLineLength(x, y, -1, 1, k, playerCell)).stream().max(Integer::compare).get();
        return maxLineLength;
    }

    /**
     * Check the current state of the board and edit this runner's state accordingly
     * This checks the state in O(k) using the information about the most recent player's turn
     */
    private void checkState(final MNKMove playerMove, final Cell playerCell) {
        int playerFormedLineLength = countMaxLineLength(playerMove.x, playerMove.y, k, playerCell);
        /// The current player has won
        if (playerFormedLineLength >= k) {
            state = GameState.WIN;
        /// Otherwise check for bonus turn
        } else if (enableBonusTurns) {
            int newPlayerFormedLineLength = countMaxNewLineLength(playerMove.x, playerMove.y, BONUS_TURN_K, playerCell);
            if (newPlayerFormedLineLength >= BONUS_TURN_K) {
                state = GameState.BONUS_TURN;
            }
        }

        // if the board doesn't have any more free cells and we have no winner then the game must be a draw
        if (!board.hasFreeCells()) {
            state = GameState.DRAW;
        }
    }

    public boolean isRunning() {
        return state == GameState.RUNNING || state == GameState.BONUS_TURN;
    }

    @Override
    public GameState doTurn(final Player player, final Cell playerCell) throws InvalidGameStateException {
        if (!isRunning()) {
            throw new IllegalStateException(
                    "GameRunner can't run any more turns after the game has ended or come to an invalid state.");
        }

        Move originalPlayerMove = player.makeMove(board.getView());
        if (!(originalPlayerMove instanceof MNKMove)) {
            state = GameState.INVALID;
            throw new InvalidGameStateException(
                    "Player " + playerCell + " has tried making a move for the wrong game.");
        }

        MNKMove playerMove = (MNKMove) originalPlayerMove;

        if (playerMove == null) {
            throw new InvalidGameStateException(
                    "Player " + playerCell + " didn't make a move.");
        }
        try {
            if (board.makeMove(playerMove, playerCell)) {
                /// Reset the state to normal running state
                state = GameState.RUNNING;
                /// And then check if it changed
                checkState(playerMove, playerCell);
                return state;
            } else {
                state = GameState.INVALID;
                throw new InvalidGameStateException(
                        "Player " + playerCell + " has tried making a move in an invalid cell.");
            }
        } catch (IndexOutOfBoundsException exception) {
            state = GameState.INVALID;
            throw new InvalidGameStateException(
                    "Player " + playerCell + " has tried to make a move in a non-existent location.",
                    exception);
        } catch (IllegalArgumentException exception) {
            state = GameState.INVALID;
            throw new InvalidGameStateException(
                    "Player " + playerCell + " has tried to make a move not suitable for the current board.",
                    exception);
        }
    }

    @Override
    public String getHumanReadableState() {
        StringBuilder representation = new StringBuilder();
        if (isRunning()) {
            representation.append("Game is up and running!\n");
        } else {
            representation.append("Game has finished. The last player's turn resulted in ");
            switch (state) {
                case WIN:
                    representation.append("a win!\n");
                    break;
                case DRAW:
                    representation.append("a draw...\n");
                    break;
                case INVALID:
                    representation.append("an invalid state, so the game simply cannot continue.\n");
                    break;
            }
        }
        representation.append("The last turn's board:\n");
        representation.append(board.getHumanReadable());
        return representation.toString();
    }
}
