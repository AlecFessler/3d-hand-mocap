#include "Logger.h"

#include <cstring>
#include <cstdio>
#include <ctime>
#include <fcntl.h>
#include <sys/time.h>
#include <stdexcept>
#include <unistd.h>

Logger::Logger(const char* filename) {
  fd_ = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (fd_ < 0)
    throw std::runtime_error("Failed to open log file");
}

Logger::~Logger() {
  if (fd_ >= 0) close(fd_);
}

static const char* levelToStr(Logger::Level level) {
  constexpr const char* levels[] = {"INFO", "WARNING", "ERROR", "UNKNOWN"};
  switch (level) {
    case Logger::Level::INFO: return levels[0];
    case Logger::Level::WARNING: return levels[1];
    case Logger::Level::ERROR: return levels[2];
    default: return levels[3];
  }
}

void Logger::log(Logger::Level level, const char* file, int line, const char* message) {
  if (fd_ < 0) return;

  struct timeval tv;
  gettimeofday(&tv, nullptr);
  struct tm tm;
  gmtime_r(&tv.tv_sec, &tm);

  char buffer[256];
  int len = snprintf(buffer, sizeof(buffer),
                     "%04d-%02d-%02d %02d:%02d:%02d.%06ldZ [%s] %s:%d - %s\n",
                     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                     tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec,
                     levelToStr(level), file, line, message);

  if (len >= static_cast<int>(sizeof(buffer))) {
    len = sizeof(buffer) - 1;
    buffer[len] = '\n';
  }

  write(fd_, buffer, len);
}
