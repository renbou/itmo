package ru.itmo.wp.model.repository;

import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.User;

import java.util.List;

public interface ArticleRepository {
  Article find(long id);
  List<Article> findByUserId(long userId);
  Article findByUserAndId(long userId, long id);
  List<Article> findAll();
  void toggleHidden(Article article);
  void save(User user, Article article);
}
