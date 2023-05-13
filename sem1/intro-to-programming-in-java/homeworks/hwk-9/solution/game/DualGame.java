package game;

import java.io.OutputStream;
import java.io.PrintWriter;

/**
 * Class defining a typical game for two players
 */
public class DualGame implements Game {
    private final static Cell[] PLAYER_MARKS = {Cell.X, Cell.O};

    private final GameTurnRunner gameTurnRunner;
    private final Player[] players;
    private final PrintWriter writer;

    /**
     * Create a dual game given a game turn runner
     * @param gameTurnRunner the game turn runner which will be used to run turns
     * @param outputStream output stream to which to write information about the game, can be null if you don't want output
     */
    public DualGame(final GameTurnRunner gameTurnRunner, final OutputStream outputStream) {
        this.gameTurnRunner = gameTurnRunner;
        players = new Player[2];

        if (outputStream != null) {
            writer = new PrintWriter(outputStream, true);
        } else {
            writer = null;
        }
    }

    @Override
    public void addPlayer(Player player) {
        if (player == null) {
            throw new NullPointerException("Cannot add a null player to a game.");
        }

        if (players[0] == null) {
            players[0] = player;
        } else if (players[1] == null) {
            players[1] = player;
        } else {
            throw new IllegalStateException("Cannot add more than 2 players to a dual game.");
        }
    }

    public int run() {
        if (players[0] == null || players[1] == null) {
            throw new IllegalStateException("Cannot run game which hasn't been initialized with enough players.");
        }

        int i = 0;
        int result = 0;
        while (gameTurnRunner.isRunning()) {
            Cell currentPlayerMark = PLAYER_MARKS[i];
            Player currentPlayer = players[i];

            if (writer != null) {
                writer.println(gameTurnRunner.getHumanReadableState());
                writer.println("Now it is " + currentPlayerMark + "'s turn.");
            }
            try {
                GameState turnResult = gameTurnRunner.doTurn(currentPlayer, currentPlayerMark);
                if (turnResult == GameState.BONUS_TURN) {
                    writer.println("Player " + currentPlayerMark + " gets a bonus turn!");
                    i ^= 1;
                } else if (turnResult == GameState.WIN) {
                    writer.println("Player " + currentPlayerMark + " has won the game!");
                    writer.println(gameTurnRunner.getHumanReadableState());
                    result = i + 1;
                } else if (turnResult == GameState.DRAW) {
                    writer.println("The game resulted in a draw. Nobody has won.");
                }
            } catch (InvalidGameStateException exception) {
                writer.println("Player " + currentPlayerMark + " has made a bad move, cannot continue the game anymore.");
                exception.printStackTrace(writer);
                result = -1;
            }

            i ^= 1;
        }
        return result;
    }
}
