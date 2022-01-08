package ru.itmo.wp.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import ru.itmo.wp.security.Guest;

@Controller
public class PostsPage extends Page {
    @GetMapping("/posts")
    public String posts() {
        return "PostsPage";
    }
}
