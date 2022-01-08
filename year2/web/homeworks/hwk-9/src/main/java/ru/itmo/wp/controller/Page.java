package ru.itmo.wp.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.ModelAttribute;
import ru.itmo.wp.domain.User;
import ru.itmo.wp.service.UserService;

import javax.servlet.http.HttpSession;

public class Page {
    private static final String USER_ID_SESSION_KEY = "userId";
    private static final String MESSAGE_SESSION_KEY = "message";
    private static final String ERR_MESSAGE_SESSION_KEY = "errorMessage";

    @Autowired
    private UserService userService;

    @ModelAttribute("user")
    public User getUser(HttpSession httpSession) {
        return userService.findById((Long) httpSession.getAttribute(USER_ID_SESSION_KEY));
    }

    @ModelAttribute("message")
    public String getMessage(HttpSession httpSession) {
        String message = (String) httpSession.getAttribute(MESSAGE_SESSION_KEY);
        httpSession.removeAttribute(MESSAGE_SESSION_KEY);
        return message;
    }

    @ModelAttribute("errorMessage")
    public String getErrorMessage(HttpSession httpSession) {
        String errorMessage = (String) httpSession.getAttribute(ERR_MESSAGE_SESSION_KEY);
        httpSession.removeAttribute(ERR_MESSAGE_SESSION_KEY);
        return errorMessage;
    }

    public boolean isLoggedIn(HttpSession httpSession) {
        return httpSession.getAttribute(USER_ID_SESSION_KEY) != null;
    }

    void setUser(HttpSession httpSession, User user) {
        if (user != null) {
            httpSession.setAttribute(USER_ID_SESSION_KEY, user.getId());
        } else {
            unsetUser(httpSession);
        }
    }

    void unsetUser(HttpSession httpSession) {
        httpSession.removeAttribute(USER_ID_SESSION_KEY);
    }

    public void putError(HttpSession httpSession, String errorMessage) {
        httpSession.setAttribute(ERR_MESSAGE_SESSION_KEY, errorMessage);
    }

    public void putMessage(HttpSession httpSession, String message) {
        httpSession.setAttribute(MESSAGE_SESSION_KEY, message);
    }
}
