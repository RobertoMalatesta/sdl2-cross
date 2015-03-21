#include <util/LogManager.h>

#include <iomanip>
#include <sstream>
#include <string>

#include <util/BashColor.h>
#include <util/Clock.h>
#include <util/File.h>
#include <util/FileInfo.h>
#include <util/FileUtil.h>
#include <util/SDL.h>
#include <util/Assert.h>
#include <util/gitrev.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

LogManager::LogManager()
  : LogManager(LEVEL_NONE,
               LEVEL_NONE,
               COLORMODE_NONE)
{
}


LogManager::LogManager(LogLevel fileLogLevel,
                       LogLevel streamLogLevel,
                       ColorMode colorMode)
  : fileLogLevel_(fileLogLevel),
    streamLogLevel_(streamLogLevel),
    streamColorMode_(colorMode),
    logfileName_(""),
    androidLoggerTag_("SWL"),
    out_(nullptr)
{
}


LogManager::~LogManager()
{
}


void LogManager::logColumnHeaders() const
{
  // Print columns for the rest of the log
  const std::string header =
    " GIT ID | LEVEL | DATE     | TIME    | LOGGER NAME      | MESSAGE";
  const std::string separator =
    "--------+-------+----------+---------+------------------+"
    "--------------------------------------";
  if (streamLogLevel_ < LEVEL_NONE) {
    log2Stream(LEVEL_INFO, "\n" + separator);
    log2Stream(LEVEL_INFO, header);
    log2Stream(LEVEL_INFO, separator);
  }

  if (fileLogLevel_ < LEVEL_NONE) {
    log2File(separator);
    log2File(header);
    log2File(separator);
  }
}


void LogManager::log(LogLevel level,
                     const std::string& loggerName,
                     const std::string& message) const
{
#ifndef LOGGING_DISABLED
  if (level < streamLogLevel_ &&
      level < fileLogLevel_)
    return;

  Clock clock;
  std::stringstream ss;

  ss << " "
     << CURRENT_GIT_REV << " "
     << logLevelAsString(level) << clock.getTimeStamp() << "  "
     << std::left << std::setw(18)
     << loggerName  << "  " << message;
  const std::string formatted(ss.str());

  if (level >= fileLogLevel_)
    log2File(formatted);

  if (level >= streamLogLevel_) {
    if (streamColorMode_ == COLORMODE_BASH) {
      const std::string bashFormatted =
        BashColor::setColor(formatted, level);
      log2Stream(level, bashFormatted);
    } else {
      log2Stream(level, formatted);
    }
  }
#else
  (void)level;
  (void)loggerName;
  (void)message;
#endif
}


void LogManager::log2Stream(
      LogLevel level,
      const std::string& formatted) const
{
#ifndef LOG2STREAM_DISABLED
  #ifdef __ANDROID__
  log2AndroidStream(level, formatted);
  #endif  // __ANDROID__
  if (out_)
    *out_ << formatted << std::endl;
  (void)level;
#else
  (void)formatted;
#endif
}


void LogManager::log2AndroidStream(LogLevel level,
                                   const std::string& formatted) const
{
#ifdef __ANDROID__
  int priority = 0;
  switch (level) {
  case LEVEL_DEBUG:   priority = ANDROID_LOG_DEBUG;  break;
  case LEVEL_INFO:    priority = ANDROID_LOG_INFO;   break;
  case LEVEL_WARNING: priority = ANDROID_LOG_WARN;   break;
  case LEVEL_ERROR:   priority = ANDROID_LOG_ERROR;  break;
  case LEVEL_NONE:    priority = ANDROID_LOG_SILENT; break;
  }
  __android_log_write(priority,
                      androidLoggerTag_.c_str(),
                      formatted.c_str());
#else
  (void)level; (void)formatted;
#endif
}


void LogManager::log2File(const std::string& formatted) const
{
#ifndef LOG2FILE_DISABLED
  if (logfileName_.empty())
    return;
  FileUtil::append(logfileName_, formatted, FileInfo::TYPE_WRITABLE);
  FileUtil::append(logfileName_, "\n", FileInfo::TYPE_WRITABLE);
#else
  (void)formatted;
#endif
}


LogManager::LogLevel LogManager::getFileLogLevel() const
{
  return fileLogLevel_;
}


void LogManager::setFileLogLevel(LogManager::LogLevel fileLogLevel)
{
  fileLogLevel_ = fileLogLevel;
}


LogManager::LogLevel LogManager::getStreamLogLevel() const
{
  return streamLogLevel_;
}


void LogManager::setStreamLogLevel(LogLevel streamLogLevel)
{
  streamLogLevel_ = streamLogLevel;
}


const std::string& LogManager::getLogfileName() const
{
  return logfileName_;
}


void LogManager::setLogfileName(std::string logfileName,
                                int id)
{
  std::string logfileNameUnique {logfileName};
  if (id > 0)
    logfileNameUnique += std::string(".") + std::to_string(id);

  File logFile(logfileNameUnique, FileInfo::TYPE_WRITABLE);
  if (logFile.exists() && fileLogLevel_ < LEVEL_NONE) {
    setLogfileName(logfileName, ++id);
  } else {
    logfileName_ = logfileNameUnique;
  }
}

std::string LogManager::getLogfilePath() const
{
  return FileUtil::prefixPath(logfileName_, FileInfo::TYPE_WRITABLE);
}


LogManager::ColorMode LogManager::getStreamColorMode() const
{
  return streamColorMode_;
}


void LogManager::setStreamColorMode(LogManager::ColorMode streamColorMode)
{
  streamColorMode_ = streamColorMode;
}


void LogManager::setStreamTarget(std::ostream& out)
{
  out_ = &out;
}


std::string LogManager::logLevelAsString(LogLevel level)
{
  switch (level) {
  case LEVEL_DEBUG:
    return std::string("DEBUG   ");
    break;
  case LEVEL_INFO:
    return std::string("INFO    ");
    break;
  case LEVEL_WARNING:
    return std::string("WARNING ");
    break;
  case LEVEL_ERROR:
    return std::string("ERROR   ");
    break;
  case LEVEL_NONE:
  default:
    assert(false && "Should never be queried");  // LCOV_EXCL_LINE
    return "";
  }
}


LogManager& LogManager::instance()
{
  static LogManager instance(LogManager::LEVEL_DEBUG,
                             LogManager::LEVEL_DEBUG,
                             LogManager::COLORMODE_BASH);
  return instance;
}
