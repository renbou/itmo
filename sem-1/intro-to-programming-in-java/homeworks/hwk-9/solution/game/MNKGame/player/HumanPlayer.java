package game.MNKGame.player;

import java.io.OutputStream;
import java.io.PrintStream;
import java.util.Scanner;

import game.Board;
import game.Move;
import game.Player;
import game.MNKGame.AbstractMNKBoard;

/**
 * Class defining a human player for the MNK game
 */
public class HumanPlayer implements Player {
    private final PrintStream output;
    private final Scanner input;

    public HumanPlayer(final Scanner input, final OutputStream output) {
        this.output = new PrintStream(output);
        this.input = input;
    }

    public HumanPlayer() {
        this(new Scanner(System.in), System.out);
    }

    @Override
    public Move makeMove(final Board board) {
        AbstractMNKBoard mnkBoard = AbstractMNKBoard.castBoard(board);
        output.print("Your move (x, y): ");

        while (!input.hasNextInt()) {
            output.print("You must enter an integer number as the first coordinate.");
        }
        int y = input.nextInt() - 1;

        while (!input.hasNextInt()) {
            output.print("You must enter an integer number as the second coordinate.");
        }
        int x = input.nextInt() - 1;

        return mnkBoard.constructMove(x, y);
    }
}
