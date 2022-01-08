package ru.itmo.wp.model.service;

import com.google.common.base.Strings;
import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.model.repository.ArticleRepository;
import ru.itmo.wp.model.repository.UserRepository;
import ru.itmo.wp.model.repository.impl.ArticleRepositoryImpl;
import ru.itmo.wp.model.repository.impl.UserRepositoryImpl;

import java.util.List;
import java.util.stream.Collectors;

public class ArticleService {
  private final ArticleRepository articleRepository = new ArticleRepositoryImpl();
  private final UserRepository userRepository = new UserRepositoryImpl();

  private boolean stringIsEmpty(String string) {
    return Strings.isNullOrEmpty(string) || string.matches("\\s+");
  }

  public void validateArticleAccess(User user) throws ValidationException {
    if (user == null) {
      throw new ValidationException("Must be authorized to work with articles");
    }
  }

  public void validateArticleCreation(User user, Article article) throws ValidationException {
    validateArticleAccess(user);

    if (stringIsEmpty(article.getTitle())) {
      throw new ValidationException("Title is required");
    }

    if (stringIsEmpty(article.getText())) {
      throw new ValidationException("Article must contain a non-empty body");
    }
  }

  public void create(User user, Article article) {
    articleRepository.save(user, article);
  }

  private void ensureArticleHaveUser(Article article) {
    article.setUser(userRepository.find(article.getUser().getId()));
  }

  public List<Article> findAll() {
    List<Article> articles = articleRepository.findAll();
    articles.forEach(this::ensureArticleHaveUser);
    return articles.stream().filter(article -> !article.getHidden()).collect(Collectors.toList());
  }

  public void validateAndToggleHidden(User user, Article article) throws ValidationException {
    validateArticleAccess(user);

    article = articleRepository.findByUserAndId(user.getId(), article.getId());
    if (article == null) {
      throw new ValidationException("Article doesn't exist or can't be accessed by user");
    }
    articleRepository.toggleHidden(article);
  }

  public long validateArticleId(String articleId) throws ValidationException {
    try {
      return Long.parseLong(articleId);
    } catch (NumberFormatException exception) {
      throw new ValidationException("articleId must represent a valid number");
    }
  }

  public List<Article> validateAndFindByUser(User user) throws ValidationException {
    if (user == null) {
      throw new ValidationException("Must be authorized to list user's articles");
    }
    return articleRepository.findByUserId(user.getId());
  }
}
