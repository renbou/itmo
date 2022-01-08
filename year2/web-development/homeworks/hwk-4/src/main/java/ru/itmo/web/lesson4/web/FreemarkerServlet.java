package ru.itmo.web.lesson4.web;

import freemarker.template.*;
import ru.itmo.web.lesson4.util.DataUtil;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.io.IOException;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

public class FreemarkerServlet extends HttpServlet {
    private static final String UTF_8 = StandardCharsets.UTF_8.name();
    private static final String idRegex = "^[0-9]+$";
    private final Configuration freemarkerConfiguration = new Configuration(Configuration.VERSION_2_3_31);

    @Override
    public void init() throws ServletException {
        super.init();

        File dir = new File(getServletContext().getRealPath("."), "../../../src/main/webapp/WEB-INF/templates");
        try {
            freemarkerConfiguration.setDirectoryForTemplateLoading(dir);
        } catch (IOException e) {
            throw new ServletException("Unable to set template directory [dir=" + dir + "].", e);
        }

        freemarkerConfiguration.setDefaultEncoding(UTF_8);
        freemarkerConfiguration.setTemplateExceptionHandler(TemplateExceptionHandler.HTML_DEBUG_HANDLER);
        freemarkerConfiguration.setLogTemplateExceptions(false);
        freemarkerConfiguration.setWrapUncheckedExceptions(true);
        freemarkerConfiguration.setFallbackOnNullLoopVariable(false);
    }

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        request.setCharacterEncoding(UTF_8);
        response.setCharacterEncoding(UTF_8);

        Template template;

        // First of all soft redirect, afterwards handle templating
        String uri = URLDecoder.decode(request.getRequestURI(), UTF_8);
        uri = Paths.get(uri).normalize().toString();
        if (uri.isEmpty() || uri.equals("/")) {
            response.sendRedirect("/index");
            return;
        }

        try {
            template = freemarkerConfiguration.getTemplate(uri + ".ftlh");
        } catch (TemplateNotFoundException ignored) {
            template = freemarkerConfiguration.getTemplate("notfound.ftlh");
            response.setStatus(HttpServletResponse.SC_NOT_FOUND);
        }
        Map<String, Object> data = getData(request);

        response.setContentType("text/html");
        try {
            template.process(data, response.getWriter());
        } catch (TemplateException e) {
            e.printStackTrace();
            response.setStatus(HttpServletResponse.SC_INTERNAL_SERVER_ERROR);
        }
    }

    private Map<String, Object> getData(HttpServletRequest request) {
        Map<String, Object> data = new HashMap<>();

        for (Map.Entry<String, String[]> e : request.getParameterMap().entrySet()) {
            String[] values = e.getValue();
            if (values != null && values.length == 1) {
                if (e.getKey().endsWith("_id")) {
                    // Only put valid numbers in data
                    if (isNumber(values[0])) {
                        data.put(e.getKey(), Long.parseLong(values[0]));
                    }
                } else {
                    data.put(e.getKey(), values[0]);
                }
            }
        }

        data.put("requestPath", request.getRequestURI());
        DataUtil.addData(request, data);
        return data;
    }

    private boolean isNumber(String value) {
        try {
            Long.parseLong(value);
            return true;
        } catch(NumberFormatException e) {
            return false;
        }
    }
}
