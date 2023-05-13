package game.MNKGame.player;

import java.util.Random;

import game.Board;
import game.Move;
import game.Player;
import game.MNKGame.AbstractMNKBoard;
import game.MNKGame.MNKMove;

public class RandomPlayer implements Player {
    private final Random random;
    private final int seed;

    public RandomPlayer(int seed) {
        this.seed = seed;
        this.random = new Random(seed);
    }

    public RandomPlayer() {
        this(0);
    }

    @Override
    public Move makeMove(Board board) {
        if (!board.hasFreeCells()) {
            throw new IllegalStateException("Random player cannot make moves when there's no place left on the board.");
        }

        AbstractMNKBoard mnkBoard = AbstractMNKBoard.castBoard(board);
        int[] dimensions = mnkBoard.getDimensions();
        MNKMove move;
        do {
            move = new MNKMove(random.nextInt(dimensions[0]), random.nextInt(dimensions[1]));
        } while (mnkBoard.validMove(move));
        return move;
    }
}
