package ru.itmo.web.lesson4.model;

public class User {
    private final long id;
    private final String handle;
    private final String name;
    private final Color color;

    public enum Color {
        RED, GREEN, BLUE;
    }

    public User(final long id, final String handle, final String name, final Color color) {
        this.id = id;
        this.handle = handle;
        this.name = name;
        this.color = color;
    }

    public long getId() {
        return id;
    }

    public String getHandle() {
        return handle;
    }

    public String getName() {
        return name;
    }

    public String getColor() {
        return color.toString().toLowerCase();
    }
}
