#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <QDateTime>

std::shared_ptr<spdlog::logger> file_logger;

bool logger_init()
{
    QString filename = "quick-printer-server-";
    filename.append(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    filename.append(".log");
    try {
        file_logger = spdlog::rotating_logger_mt("quick-printer-server", filename.toStdString(), 1024 * 1024 * 5, 3);
        file_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");
    } catch (const spdlog::spdlog_ex& ex) {
        qDebug("Log initialization failed: %s", ex.what());
        return false;
    }
    return true;
}

std::shared_ptr<spdlog::logger> logger_instance()
{
    return file_logger;
}
