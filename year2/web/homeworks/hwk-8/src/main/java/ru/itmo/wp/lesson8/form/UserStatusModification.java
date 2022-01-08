package ru.itmo.wp.lesson8.form;

import javax.persistence.Enumerated;
import javax.validation.constraints.NotEmpty;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Pattern;
import javax.validation.constraints.Size;

@SuppressWarnings("unused")
public class UserStatusModification {
  @NotNull
  @NotEmpty
  private String id;

  @NotNull
  @NotEmpty
  @Pattern(regexp = "enable|disable")
  private String action;

  public String getId() {
    return id;
  }

  public void setId(String id) {
    this.id = id;
  }

  public String getAction() {
    return action;
  }

  public void setAction(String action) {
    this.action = action;
  }
}
