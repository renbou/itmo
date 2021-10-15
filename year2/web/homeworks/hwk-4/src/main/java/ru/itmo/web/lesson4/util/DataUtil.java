package ru.itmo.web.lesson4.util;

import ru.itmo.web.lesson4.model.Post;
import ru.itmo.web.lesson4.model.User;

import javax.servlet.http.HttpServletRequest;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class DataUtil {
    private static final List<User> USERS = Arrays.asList(
            new User(1, "MikeMirzayanov", "Mike Mirzayanov", User.Color.RED),
            new User(6, "pashka", "Pavel Mavrin", User.Color.RED),
            new User(9, "geranazarov555", "Georgiy Nazarov", User.Color.BLUE),
            new User(11, "tourist", "Gennady Korotkevich", User.Color.GREEN)
    );

    private static final List<Post> POSTS = Arrays.asList(
        new Post(1, "Updates to the platform",
            "Hello Codeforces!\n\n" +
            "This week some maintenance work will need to be done, so the site will be offline.\n\n" +
            "Meanhwile, goodluck!", 1),
        new Post(2, "New Algosiki Course",
            "Hi all!\n\n" +
            "Come watch my new Algorithms & Data Structures course at:\n" +
            "https://www.youtube.com/c/pavelmavrin\n\n"+
            "Hope you enjoy it :)", 6),
        new Post(3, "Long Post with >250 chars",
            "This is a post with more than 250 characters and thus "+
            "should be shortened (end should be replaced with ...). You will now see lots of " +
            "random letters which are used to reach the desired length of two hundred and fifty " +
            "characters: "+
            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
            11)
    );

    public static void addData(HttpServletRequest request, Map<String, Object> data) {
        data.put("users", USERS);
        data.put("posts", POSTS);

        for (User user : USERS) {
            if (Long.toString(user.getId()).equals(request.getParameter("logged_user_id"))) {
                data.put("user", user);
            }
        }
    }
}
