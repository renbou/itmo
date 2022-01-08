package ru.itmo.wp.lesson8.service;

import org.springframework.stereotype.Service;
import ru.itmo.wp.lesson8.domain.Notice;
import ru.itmo.wp.lesson8.form.NoticeText;
import ru.itmo.wp.lesson8.repository.NoticeRepository;

import java.util.List;

@Service
public class NoticeService {
  private final NoticeRepository noticeRepository;

  public NoticeService(final NoticeRepository noticeRepository) {
    this.noticeRepository = noticeRepository;
  }

  public void createNotice(NoticeText noticeText) {
    Notice notice = new Notice();
    notice.setContent(noticeText.getText());
    noticeRepository.save(notice);
  }

  public List<Notice> findAll() {
    return this.noticeRepository.findAllByOrderByCreationTimeDesc();
  }
}
