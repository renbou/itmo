package ru.itmo.wp.lesson8.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import ru.itmo.wp.lesson8.service.UserService;

@Controller
public class UserPage extends Page {
  private final UserService userService;

  public UserPage(UserService userService) {
    this.userService = userService;
  }

  @GetMapping("/user/{id}")
  public String users(@PathVariable String id, Model model) {
    model.addAttribute("user", userService.findById(id));
    return "UserPage";
  }
}
