package game.MNKGame;

/**
 * MNKMove describing a point on the board for an MNK game
 */
public class MNKMove implements game.Move {
    public final int x;
    public final int y;

    public MNKMove(final int x, final int y) {
        this.x = x;
        this.y = y;
    }
}
