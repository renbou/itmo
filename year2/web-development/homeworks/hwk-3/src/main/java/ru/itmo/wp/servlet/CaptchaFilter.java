package ru.itmo.wp.servlet;

import ru.itmo.wp.util.ImageUtils;

import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.http.HttpFilter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.util.Base64;
import java.util.Random;

public class CaptchaFilter extends HttpFilter {
  @Override
  protected void doFilter(HttpServletRequest request, HttpServletResponse response, FilterChain chain) throws IOException, ServletException {
    final HttpSession session = request.getSession();
    // Skip logic if user is already verified or request isn't get
    if (!request.getMethod().equals("GET") ||
        session.getAttribute("verified") != null) {
      chain.doFilter(request, response);
      return;
    }

    // If user is unverified but already trying to submit captcha
    //   then try to verify him and reset captcha on fail
    Integer assignedCaptcha = (Integer)session.getAttribute("captcha");
    final String guessedCaptchaStr = request.getParameter("captcha");
    if (assignedCaptcha != null && guessedCaptchaStr != null) {
      if (verifyCaptcha(assignedCaptcha, guessedCaptchaStr)) {
        session.setAttribute("verified", true);
        chain.doFilter(request, response);
        return;
      } else {
        assignedCaptcha = null;
      }
    }
    // Assign captcha if new user or if verification failed
    if (assignedCaptcha == null) {
      assignedCaptcha = assignCaptcha(session);
    }

    // Send captcha and don't continue filter chain
    sendCaptcha(response.getWriter(),
        request.getServletContext().getRealPath("/template/captcha.html"),
        assignedCaptcha);
  }

  private boolean verifyCaptcha(Integer assignedCaptcha, String guessedCaptchaStr) {
    try {
      Integer guessedCaptcha = Integer.valueOf(guessedCaptchaStr);
      return assignedCaptcha.equals(guessedCaptcha);
    } catch (NumberFormatException exception) {
      return false;
    }
  }

  private int assignCaptcha(final HttpSession session) {
    final Random rng = new Random();
    final int captcha = 100 + rng.nextInt(900);
    session.setAttribute("captcha", captcha);
    return captcha;
  }

  private void sendCaptcha(PrintWriter out, String captchaFilePath, Integer captcha) throws IOException {
    final File captchaTemplateFile = new File(captchaFilePath);
    if (!captchaTemplateFile.isFile()) {
      throw new IOException("Captcha file /template/captcha.html is not a file");
    }
    String captchaTemplate = new String(Files.readAllBytes(captchaTemplateFile.toPath()));

    final byte[] captchaPng = ImageUtils.toPng(captcha.toString());
    final String encodedCaptchaPng = Base64.getEncoder().encodeToString(captchaPng);
    final String formattedCaptcha = captchaTemplate.replaceAll("\\{captcha}", encodedCaptchaPng);
    out.write(formattedCaptcha);
    out.flush();
  }
}
