package ru.itmo.wp.lesson8.form.validator;

import org.springframework.stereotype.Component;
import org.springframework.validation.Errors;
import org.springframework.validation.Validator;
import ru.itmo.wp.lesson8.domain.User;
import ru.itmo.wp.lesson8.form.UserStatusModification;
import ru.itmo.wp.lesson8.service.UserService;

import java.util.List;

@Component
public class UserStatusModificationValidator implements Validator {
  private final UserService userService;

  public UserStatusModificationValidator(UserService userService) {
    this.userService = userService;
  }

  @Override
  public boolean supports(Class<?> clazz) {
    return UserStatusModification.class.equals(clazz);
  }

  @Override
  public void validate(Object target, Errors errors) {
    if (!errors.hasErrors()) {
      UserStatusModification userStatusModification = (UserStatusModification) target;
      if (userService.findById(userStatusModification.getId()) == null) {
        errors.rejectValue("id", "id.invalid-id", "invalid id");
      }
    }
  }
}
