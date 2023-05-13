import game.*;
import game.geometry.*;
import game.MNKGame.*;
import game.MNKGame.player.*;


public class MNK {
    public static void main(String[] args) throws Exception {
        MNKBoard rectangularBoard = new MNKBoard(new Rhombus(8));
        MNKGameTurnRunner gameTurnRunner = new MNKGameTurnRunner(rectangularBoard, 6, true);
        DualGame dualGame = new DualGame(gameTurnRunner, System.out);
        dualGame.addPlayer(new SequentialPlayer());
        dualGame.addPlayer(new HumanPlayer());
        dualGame.run();
    }
}
