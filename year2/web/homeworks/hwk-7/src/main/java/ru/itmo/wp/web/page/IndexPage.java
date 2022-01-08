package ru.itmo.wp.web.page;

import com.google.common.base.Strings;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.Map;

/** @noinspection unused*/
public class IndexPage {
    private void action(HttpServletRequest request, Map<String, Object> view) {
        putNotifications(request, view);
    }

    private void putNotifications(HttpServletRequest request, Map<String, Object> view) {
        HttpSession session = request.getSession();
        putNotification(session, view, "message");
        putNotification(session, view, "error");
    }

    private void putNotification(HttpSession session, Map<String, Object> view, String key) {
        String notification = (String)session.getAttribute(key);
        if (!Strings.isNullOrEmpty(notification)) {
            view.put(key, notification);
            session.removeAttribute(key);
        }
    }
}
