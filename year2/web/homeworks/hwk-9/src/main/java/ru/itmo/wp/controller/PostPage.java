package ru.itmo.wp.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import ru.itmo.wp.domain.Comment;
import ru.itmo.wp.domain.Post;
import ru.itmo.wp.form.CommentText;
import ru.itmo.wp.security.Guest;
import ru.itmo.wp.service.PostService;

import javax.servlet.http.HttpSession;
import javax.validation.Valid;

@Controller
public class PostPage extends Page {
  private final PostService postService;

  public PostPage(PostService postService) {
    this.postService = postService;
  }

  @Guest
  @GetMapping("/post/{id}")
  public String post(@PathVariable String id, Model model) {
    model.addAttribute("post", postService.findById(id));
    model.addAttribute("commentForm", new CommentText());
    return "PostPage";
  }

  @PostMapping("/post/{id}")
  public String comment(@PathVariable String id, @Valid @ModelAttribute("commentForm") CommentText commentText,
                        BindingResult bindingResult, Model model, HttpSession httpSession) {
    Post post = postService.findById(id);
    if (post == null) {
      return "redirect:";
    }
    model.addAttribute("post", post);

    if (bindingResult.hasErrors()) {
      return "PostPage";
    }

    Comment comment = new Comment();
    comment.setText(commentText.getText());

    postService.addCommentByUser(post, comment, getUser(httpSession));
    return "redirect:/post/" + id;
  }
}
