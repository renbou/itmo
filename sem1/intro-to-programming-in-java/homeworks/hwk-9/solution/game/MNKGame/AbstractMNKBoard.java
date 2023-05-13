package game.MNKGame;

import game.Board;
import game.Cell;
import game.Move;
import game.geometry.AbstractShape;
import game.geometry.Rectangle;

/**
 * Abstract class defining pretty the board and an immutable board view for the MNK game. For an MNK game, to prevent cheating,
 * this correctly defines the player's view of the board.
 */
public abstract class AbstractMNKBoard implements Board {
    /// Can be the actual board's rectangle or the unmodifiable view
    protected final AbstractShape boardShape;

    /**
     * Create an AbstractMNKBoard using the board's field
     * @param boardShape the board's rectangle, either a view or the actual rectangle
     */
    public AbstractMNKBoard(final AbstractShape boardShape) {
        this.boardShape = boardShape;
    }

    @Override
    public Cell getCell(final int x, final int y) {
        try {
            return Cell.fromInteger(boardShape.getPointValue(x, y));
        } catch (IndexOutOfBoundsException exception) {
            throw new IndexOutOfBoundsException(
                    "Cannot get cell value at a non-existent point on the board, check with pointOnBoard first.");
        }
    }

    @Override
    public boolean pointOnBoard(final int x, final int y) {
        return boardShape.containsPoint(x, y);
    }

    @Override
    public boolean validMove(final Move move) {
        MNKMove mnkMove = castMove(move);
        return boardShape.containsPoint(mnkMove.x, mnkMove.y) && getCell(mnkMove.x, mnkMove.y) == Cell.EMPTY;
    }

    @Override
    public int[] getDimensions() {
        return boardShape.getBounds();
    }

    /**
     * Calculate the width of any integer if we were to print it as a decimal string
     * @param a any integer
     * @return the width of that integer using the specified representation
     */
    private static int getIntWidth(int a) {
        if (a == 0)
            return 1;
        return (int) Math.ceil(Math.log10(Math.abs(a + 1)))
                + (a < 0 ? 1 : 0); /// the sign
    }


    @Override
    public String getHumanReadable() {
        StringBuilder representation = new StringBuilder();
        int[] boardDimensions = getDimensions();
        int OxAxeLabelPadding = getIntWidth(boardDimensions[1]) + 1;
        int OyAxeLabelPadding = getIntWidth(boardDimensions[0]);

        for (int i = boardDimensions[0]; i > 0; i--) {
            representation.append(" ".repeat(
                    OyAxeLabelPadding - getIntWidth(i)) /// repeat a space enough times to match the padding of Oy labels
            ).append(i).append(" ");

            for (int j = 0; j < boardDimensions[1]; j++) {
                final String pointValue;
                if (pointOnBoard(i - 1, j)) {
                    representation.append(" ".repeat((OxAxeLabelPadding - 1) / 2)) /// repeat half with round-down
                            .append(getCell(i - 1, j).toString())
                            .append(" ".repeat((OxAxeLabelPadding) / 2)); /// repeat half with round-up
                } else {
                    representation.append("▓".repeat(
                            OxAxeLabelPadding)); /// repeat a space enough times to match the padding of Ox labels
                }
            }
            representation.append('\n');
        }
        representation.append(" ".repeat(OyAxeLabelPadding + 1));

        for (int j = 1; j <= boardDimensions[1]; j++) {
            int width = getIntWidth(j);
            representation.append(" ".repeat((OxAxeLabelPadding - width) / 2))
                    .append(j)
                    .append(" ".repeat((OxAxeLabelPadding - width + 1) / 2));
        }
        return representation.toString();
    }

    /**
     * Method specific to the MNK-game board, which will construct a move given x and y.
     */
    public static MNKMove constructMove(int x, int y) {
        return new MNKMove(x, y);
    }

    /**
     * Method specific to the MNK-game board, which will try to cast a move instance to MNKMove
     * @throws IllegalArgumentException if the move isn't suitable for an MNK game (instance of wrong class)
     */
    public static MNKMove castMove(Move move) {
        if (move instanceof MNKMove)
            return (MNKMove) move;
        throw new IllegalArgumentException("Cannot cast passed move instance to an MNKMove.");
    }

    /**
     * Method specific to the MNK-game board, which will try to cast a given board to this board type
     * @throws IllegalArgumentException if the move isn't suitable for an MNK game (instance of wrong class)
     */
    public static AbstractMNKBoard castBoard(Board board) {
        if (board instanceof AbstractMNKBoard)
            return (AbstractMNKBoard) board;
        throw new IllegalArgumentException("Cannot cast passed board instance to an AbstractMNKBoard.");
    }

    /**
     * Get the immutable view of this board, pretty much what should be given to the player makeMove method
     */
    public abstract AbstractMNKBoard getView();
}
