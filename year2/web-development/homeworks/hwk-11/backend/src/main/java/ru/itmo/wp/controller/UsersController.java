package ru.itmo.wp.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import ru.itmo.wp.domain.User;
import ru.itmo.wp.service.UserService;

import java.util.List;

@RestController
@RequestMapping("/api/1")
public class UsersController {
  private final UserService userService;

  public UsersController(final UserService userService) {
    this.userService = userService;
  }

  @GetMapping("users")
  public List<User> findUsers() {
    return userService.findAll();
  }
}
