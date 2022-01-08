package ru.itmo.wp.lesson8.form.validator;

import org.springframework.stereotype.Component;
import org.springframework.validation.Errors;
import org.springframework.validation.Validator;
import ru.itmo.wp.lesson8.domain.User;
import ru.itmo.wp.lesson8.form.UserCredentials;
import ru.itmo.wp.lesson8.service.UserService;

@Component
public class UserCredentialsEnterValidator implements Validator {
    private final UserService userService;

    public UserCredentialsEnterValidator(UserService userService) {
        this.userService = userService;
    }

    @Override
    public boolean supports(Class<?> clazz) {
        return UserCredentials.class.equals(clazz);
    }

    @Override
    public void validate(Object target, Errors errors) {
        if (!errors.hasErrors()) {
            UserCredentials enterForm = (UserCredentials) target;
            User user = userService.findByLoginAndPassword(enterForm.getLogin(), enterForm.getPassword());
            if (user == null) {
                errors.rejectValue("password", "password.invalid-login-or-password", "invalid login or password");
            } else if (!user.getEnabled()) {
                errors.rejectValue("login", "user.disabled", "user has been disabled");
            }
        }
    }
}
