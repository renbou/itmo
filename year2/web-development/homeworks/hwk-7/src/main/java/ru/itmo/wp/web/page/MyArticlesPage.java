package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.model.service.ArticleService;
import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

public class MyArticlesPage {
  private final ArticleService articleService = new ArticleService();

  private void action(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
    User user = (User)request.getSession().getAttribute("user");
    try {
      articleService.validateArticleAccess(user);
    } catch (ValidationException exception) {
      request.getSession().setAttribute("error", exception.getMessage());
      throw new RedirectException("/index");
    }
    view.put("articles", articleService.validateAndFindByUser(user));
  }

  private void toggleHidden(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
    User user = (User)request.getSession().getAttribute("user");
    Article article = new Article();
    article.setId(articleService.validateArticleId(request.getParameter("articleId")));
    articleService.validateAndToggleHidden(user, article);
  }
}
