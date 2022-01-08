package ru.itmo.wp.domain;

import javax.persistence.*;
import javax.validation.constraints.NotBlank;
import javax.validation.constraints.NotNull;

@Entity
@Table(
    uniqueConstraints = @UniqueConstraint(columnNames = "name")
)
public class Tag {
  @Id
  @GeneratedValue
  private long id;

  @NotNull
  @NotBlank
  private String name;

  public long getId() {
    return id;
  }

  public void setId(long id) {
    this.id = id;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  @Override
  public String toString() {
    return getName();
  }
}
