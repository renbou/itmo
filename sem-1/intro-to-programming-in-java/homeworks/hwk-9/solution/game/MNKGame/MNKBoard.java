package game.MNKGame;

import game.Cell;
import game.Move;
import game.geometry.AbstractShape;
import game.geometry.Rectangle;

/**
 * Class defining the actual board for a typical two-dimensional MNK game. We limit the fields here to be located in the
 * first quadrant of the cartesian coordinate system, meaning x>=0,y>=0.
 */
public class MNKBoard extends AbstractMNKBoard {
    /// Save the view so that we create it only once after initialization
    private AbstractMNKBoard view = null;

    /// number of free cells left, to optimize hasFreeCells
    private int cellsLeft;

    /**
     * Construct the MNK game board using the given field. We can't make a constructor from n, m since the field shape
     * can be whatever the player wants.
     * @throws IllegalArgumentException if the boardField isn't located in the first quadrant of coordinates, or isn't 2d
     */
    public MNKBoard(final AbstractShape boardShape) {
        super(boardShape);
        cellsLeft = boardShape.numberOfPoints();
    }

    @Override
    public boolean hasFreeCells() {
        return cellsLeft > 0;
    }

    @Override
    public boolean makeMove(Move playerMove, Cell playerCell) {
        MNKMove mnkPlayerMove = castMove(playerMove);

        try {
            if (validMove(mnkPlayerMove)) {
                boardShape.setPointValue(mnkPlayerMove.x, mnkPlayerMove.y, playerCell.toInteger());
                cellsLeft--;
            } else {
                return false;
            }
        } catch (IllegalArgumentException exception) {
            throw new IllegalArgumentException(
                    "Int corresponding to cell is a negative number, please make sure that all cell keys are positive ints.");
        }
        return true;
    }

    @Override
    public AbstractMNKBoard getView() {
        if (view == null) {
            view = new AbstractMNKBoard(boardShape.getUnmodifiableView()) {
                @Override
                public AbstractMNKBoard getView() {
                    return this;
                }

                @Override
                public boolean hasFreeCells() {
                    return MNKBoard.this.hasFreeCells();
                }

                @Override
                public boolean makeMove(Move playerMove, Cell playerCell) {
                    return MNKBoard.this.makeMove(playerMove, playerCell);
                }
            };
        }

        return view;
    }
}
