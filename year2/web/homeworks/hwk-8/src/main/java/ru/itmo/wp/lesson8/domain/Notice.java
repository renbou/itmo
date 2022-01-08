package ru.itmo.wp.lesson8.domain;

import org.hibernate.annotations.CreationTimestamp;

import javax.persistence.*;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.NotBlank;
import java.util.Date;

@Entity
@Table(
    indexes = @Index(columnList = "creationTime")
)
public class Notice {
  @Id
  @GeneratedValue
  private long id;

  @Lob
  @NotNull
  @NotBlank
  private String content;

  @CreationTimestamp
  private Date creationTime;

  public String getContent() {
    return this.content;
  }

  public Date getCreationTime() {
    return this.creationTime;
  }

  public void setContent(String content) {
    this.content = content;
  }
}
