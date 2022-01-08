package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.model.service.ArticleService;
import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.Map;

public class ArticlePage {
  private final ArticleService articleService = new ArticleService();

  private void action(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
    User user = (User)request.getSession().getAttribute("user");
    try {
      articleService.validateArticleAccess(user);
    } catch (ValidationException exception) {
      request.getSession().setAttribute("error", exception.getMessage());
      throw new RedirectException("/index");
    }
  }

  private void create(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
    final HttpSession session = request.getSession();
    User user = (User)session.getAttribute("user");
    Article article = new Article();
    article.setTitle(request.getParameter("title"));
    article.setText(request.getParameter("text"));

    articleService.validateArticleCreation(user, article);
    articleService.create(user, article);

    session.setAttribute("message", "Article successfully created");
    throw new RedirectException("/index");
  }

  private void findAll(HttpServletRequest request, Map<String, Object> view) {
    view.put("articles", articleService.findAll());
  }
}
