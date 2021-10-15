package ru.itmo.wp.servlet;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Files;
import java.util.ArrayList;

public class StaticServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        String uri = request.getRequestURI();
        ArrayList<File> files = new ArrayList<>();
        for (String path: uri.split("\\+")) {
            File file = this.getFile(path);
            if (!file.isFile()) {
                response.sendError(HttpServletResponse.SC_NOT_FOUND);
                return;
            }
            files.add(file);
        }
        response.setContentType(getServletContext().getMimeType(files.get(0).getPath()));
        OutputStream outputStream = response.getOutputStream();
        for (File file: files) {
            Files.copy(file.toPath(), outputStream);
        }
        outputStream.flush();
    }

    private File getFile(String path) {
        File file = new File(
            "/Users/renbou/itmo/year2/web/homeworks/hwk-3/wp3/src/main/webapp/static/" + path);
        if (!file.isFile()) {
            file = new File(getServletContext().getRealPath("/static/" + path));
        }
        return file;
    }
}
