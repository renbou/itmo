package ru.itmo.wp.lesson8.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.InitBinder;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import ru.itmo.wp.lesson8.domain.User;
import ru.itmo.wp.lesson8.form.UserCredentials;
import ru.itmo.wp.lesson8.form.UserStatusModification;
import ru.itmo.wp.lesson8.form.validator.UserStatusModificationValidator;
import ru.itmo.wp.lesson8.service.UserService;

import javax.validation.Valid;

@Controller
public class UsersPage extends Page {
    private final UserService userService;
    private final UserStatusModificationValidator userStatusModificationValidator;

    public UsersPage(UserService userService, UserStatusModificationValidator userStatusModificationValidator) {
        this.userService = userService;
        this.userStatusModificationValidator = userStatusModificationValidator;
    }

    @InitBinder("modifyForm")
    public void initBinder(WebDataBinder binder) {
        binder.addValidators(userStatusModificationValidator);
    }

    @GetMapping("/users/all")
    public String users(Model model) {
        model.addAttribute("users", userService.findAll());
        model.addAttribute("modifyForm", new UserStatusModification());
        return "UsersPage";
    }

    @PostMapping("/users/all")
    public String modifyUsers(@Valid @ModelAttribute("modifyForm") UserStatusModification userStatusModification,
                              BindingResult bindingResult) {
        if (!bindingResult.hasErrors()) {
            userService.setStatus(userStatusModification.getId(), userStatusModification.getAction().equals("enable"));
        }
        return "redirect:/users/all";
    }
}
