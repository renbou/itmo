package ru.itmo.wp.web.page;

import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.Arrays;
import java.util.Map;

@SuppressWarnings({"unused", "RedundantSuppression"})
public class TicTacToePage {
  public static int SIZE = 3;

  public static class State {
    private enum Phase {
      RUNNING, WON_X, WON_O, DRAW
    }

    private enum Value {
      X, O
    }

    private class PhaseChecker {
      public boolean win;
      public boolean draw;

      // Traverse single path and update status
      private void traverse(int fromX, int fromY, int dx, int dy) {
        boolean localWin = true;
        for (int i = 0; i < SIZE; i++) {
          int x = fromX + dx * i;
          int y = fromY + dy * i;
          localWin = localWin && (cells[x][y] == turn);
          draw = draw && (cells[x][y] != null);
        }
        win = win || localWin;
      }

      public void run() {
        win = false;
        draw = true;
        // diagonals
        traverse( 0, 0, 1, 1);
        traverse( 0, SIZE - 1, 1, -1);
        for (int i = 0; i < SIZE; i++) {
          traverse(i, 0, 0, 1); // row
          traverse( 0, i, 1, 0); // collumn
        }
      }
    }

    private final Value[][] cells;
    private Value turn;
    private Phase phase;
    final private PhaseChecker phaseChecker;

    public State() {
      cells = new Value[SIZE][SIZE];
      for (Value[] values : cells) {
        Arrays.fill(values, null);
      }
      turn = Value.X;
      phase = Phase.RUNNING;
      phaseChecker = new PhaseChecker();
    }

    public void makeMove(int x, int y) {
      if (invalidRange(x) || invalidRange(y) || phase != Phase.RUNNING || cells[x][y] != null) {
        return;
      }
      cells[x][y] = turn;
      phaseChecker.run();
      if (phaseChecker.win) {
        phase = (turn == Value.X ? Phase.WON_X : Phase.WON_O);
      } else if (phaseChecker.draw) {
        phase = Phase.DRAW;
      }
      switchTurn();
    }

    private boolean invalidRange(int coordinate) {
      return coordinate < 0 || coordinate > SIZE - 1;
    }

    private void switchTurn() {
      if (turn == Value.X) {
        turn = Value.O;
      } else {
        turn = Value.X;
      }
    }

    public int getSize() {
      return SIZE;
    }

    public Value[][] getCells() {
      return cells;
    }

    public boolean getCrossesMove() {
      return turn == Value.X;
    }

    public String getPhase() {
      return phase.toString();
    }
  }

  public void action(HttpServletRequest request, Map<String, Object> view) {
    HttpSession session = request.getSession();
    if (session.getAttribute("state") == null) {
      session.setAttribute("state", new State());
    }
    view.put("state", session.getAttribute("state"));
  }

  public void onMove(HttpServletRequest request, Map<String, Object> view) {
    final HttpSession session = request.getSession();
    State state = (State) session.getAttribute("state");
    if (state == null) {
      throw new RedirectException("/ticTacToe");
    }
    view.put("state", state);

    final Map<String, String[]> parameters = request.getParameterMap();
    String foundParameter = null;
    for (String parameterName: parameters.keySet()) {
      if (parameterName.matches("cell_\\d{2}")) {
        if (foundParameter != null) {
          // Cannot have several valid parameters
          return;
        } else {
          foundParameter = parameterName;
        }
      }
    }
    if (foundParameter == null) {
      // parameter must exist for turn to be made
      return;
    }

    int x = Character.getNumericValue(foundParameter.charAt(5));
    int y = Character.getNumericValue(foundParameter.charAt(6));
    state.makeMove(x, y);
  }

  public void newGame(HttpServletRequest request) {
    final HttpSession session = request.getSession();
    session.removeAttribute("state");
    throw new RedirectException("/ticTacToe");
  }
}
