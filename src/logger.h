#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

bool logger_init();

std::shared_ptr<spdlog::logger> logger_instance();

#endif // LOGGER_H
