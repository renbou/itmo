package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.model.service.UserService;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

/** @noinspection unused*/
public class UsersPage {
    private final UserService userService = new UserService();

    private void action(HttpServletRequest request, Map<String, Object> view) {
        // No operations.
    }

    private void findAll(HttpServletRequest request, Map<String, Object> view) {
        view.put("users", userService.findAll());
    }

    private void findUser(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
        view.put("user",
                userService.find(userService.validateUserId(request.getParameter("userId"))));
    }

    private void toggleAdmin(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
        User admin = (User)request.getSession().getAttribute("user");
        User user = new User();
        user.setId(userService.validateUserId(request.getParameter("userId")));
        userService.validateAndToggleAdmin(admin, user);
    }
}
