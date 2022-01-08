package ru.itmo.wp.lesson8.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.InitBinder;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import ru.itmo.wp.lesson8.form.NoticeText;
import ru.itmo.wp.lesson8.form.UserCredentials;
import ru.itmo.wp.lesson8.form.validator.UserCredentialsEnterValidator;
import ru.itmo.wp.lesson8.service.NoticeService;
import ru.itmo.wp.lesson8.service.UserService;

import javax.servlet.http.HttpSession;
import javax.validation.Valid;

@Controller
public class NoticePage extends Page {
  private final NoticeService noticeService;


  public NoticePage(final NoticeService noticeService) {
    this.noticeService = noticeService;
  }

  @GetMapping("/notice")
  public String register(Model model, HttpSession httpSession) {
    model.addAttribute("noticeForm", new NoticeText());
    return "NoticePage";
  }

  @PostMapping("/notice")
  public String register(@Valid @ModelAttribute("noticeForm") NoticeText noticeText,
                         BindingResult bindingResult) {
    if (bindingResult.hasErrors()) {
      return "NoticePage";
    }

    noticeService.createNotice(noticeText);

    return "redirect:/notice";
  }
}
