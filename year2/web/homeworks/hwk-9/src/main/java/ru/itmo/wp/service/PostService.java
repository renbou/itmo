package ru.itmo.wp.service;

import org.springframework.stereotype.Service;
import ru.itmo.wp.domain.Comment;
import ru.itmo.wp.domain.Post;
import ru.itmo.wp.domain.User;
import ru.itmo.wp.repository.PostRepository;

import java.util.List;

@Service
public class PostService {
    private final PostRepository postRepository;

    public PostService(PostRepository postRepository) {
        this.postRepository = postRepository;
    }

    public List<Post> findAll() {
        return postRepository.findAllByOrderByCreationTimeDesc();
    }

    public Post findById(long id) {
        return postRepository.findById(id);
    }

    public Post findById(String id) {
        try {
            return findById(Long.parseLong(id));
        } catch (NumberFormatException exception) {
            return null;
        }
    }

    public void addCommentByUser(Post post, Comment comment, User user) {
        comment.setUser(user);
        post.addComment(comment);
        postRepository.save(post);
    }
}
