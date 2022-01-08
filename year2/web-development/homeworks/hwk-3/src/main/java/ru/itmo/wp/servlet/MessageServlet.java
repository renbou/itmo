package ru.itmo.wp.servlet;

import com.google.gson.Gson;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class MessageServlet extends HttpServlet {
  private static class Message {
    public final String user;
    public final String text;

    public Message(final String user, final String text) {
      this.user = user;
      this.text = text;
    }
  }

  private final ArrayList<Message> messages = new ArrayList<>();
  private final ReadWriteLock lock = new ReentrantReadWriteLock();

  @Override
  protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws IOException {
    final String uri = req.getRequestURI().substring("/message".length());
    switch (uri) {
      case "/auth":
        handleAuth(req, resp);
        break;
      case "/findAll":
        handleFindAll(req, resp);
        break;
      case "/add":
        handleAdd(req, resp);
        break;
      default:
        resp.setStatus(HttpServletResponse.SC_NOT_FOUND);
    }
  }

  private void handleAuth(HttpServletRequest req, HttpServletResponse resp) throws IOException {
    String user = req.getParameter("user");
    if (user != null && !user.matches("^\\s*$")) {
      req.getSession().setAttribute("user", user.trim());
    } else {
      user = (String) req.getSession().getAttribute("user");
    }
    resp.setContentType("application/json");
    resp.getWriter().write(new Gson().toJson(user));
    resp.getWriter().flush();
  }

  private void handleFindAll(HttpServletRequest req, HttpServletResponse resp) throws IOException {
    if (req.getSession().getAttribute("user") == null) {
      resp.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
      return;
    }
    lock.readLock().lock();
    final String json = new Gson().toJson(messages, ArrayList.class);
    lock.readLock().unlock();
    resp.setContentType("application/json");
    resp.getWriter().print(json);
    resp.getWriter().flush();
  }

  private void handleAdd(HttpServletRequest req, HttpServletResponse resp) {
    final String user = (String) req.getSession().getAttribute("user");
    if (user == null) {
      resp.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
      return;
    }
    final String text = req.getParameter("text");
    if (text == null) {
      resp.setStatus(HttpServletResponse.SC_BAD_REQUEST);
      return;
    }
    lock.writeLock().lock();
    messages.add(new Message(user, text.trim()));
    lock.writeLock().unlock();
  }
}
