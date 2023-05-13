package game.MNKGame.player;

import game.Board;
import game.Move;
import game.Player;
import game.Cell;
import game.MNKGame.AbstractMNKBoard;
import game.MNKGame.MNKMove;

public class SequentialPlayer implements Player {
    @Override
    public Move makeMove(Board board) {
        if (!board.hasFreeCells()) {
            throw new IllegalStateException("Sequential player cannot make moves when there's no place left on the board.");
        }

        AbstractMNKBoard mnkBoard = AbstractMNKBoard.castBoard(board);
        int[] dimensions = mnkBoard.getDimensions();
        MNKMove move = null;
        outer: for (int i = 0; i < dimensions[0]; i++) {
            for (int j = 0; j < dimensions[1]; j++) {
                move = new MNKMove(i, j);
                if (mnkBoard.validMove(move)) {
                    break outer;
                }
            }
        }
        return move;
    }
}
