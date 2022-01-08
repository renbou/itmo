package ru.itmo.wp.lesson8.form;

import javax.validation.constraints.NotBlank;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

@SuppressWarnings("unused")
public class NoticeText {
  @NotNull
  @NotBlank(message = "Notice text must not be blank")
  @Size(max=1024)
  private String text;

  public String getText() {
    return text;
  }

  public void setText(String text) {
    this.text = text;
  }
}
