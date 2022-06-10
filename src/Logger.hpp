#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

class Logger {
public:
	Logger();
	Logger(const std::string&);
	~Logger();

	template<typename... Args>
	std::string Info(const Args&...);

	template<typename... Args>
	std::string Error(const Args&...);

	template<typename... Args>
	std::string Write(const Args&...);

	std::string LastMessage();
	std::string CurrentTime();

private:
	fs::path		m_log_path;
	std::string		m_last_msg;
};

std::string Logger::LastMessage() {
	return m_last_msg;
}

std::string Logger::CurrentTime() {
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	return std::ctime(&time);
}

Logger::Logger() {
	m_log_path = "Log.txt";
	this->Info("Log created: ", m_log_path);
}

Logger::Logger(const std::string& _log_path) : m_log_path(_log_path) {
	this->Info("Session started.", m_log_path);
}

Logger::~Logger() {
	this->Info("Session done.");
}

template<typename... Args>
std::string Logger::Write(const Args&... _args) {
	std::stringstream ss;
	((ss << _args << ' '), ...);
	m_last_msg = ss.str();
	return m_last_msg;
}

template<typename... Args>
std::string Logger::Info(const Args&... _args) {
	Write(CurrentTime(), " [INFO] ", _args...);
	return m_last_msg;
}

template<typename... Args>
std::string Logger::Error(const Args&... _args) {
	Write(CurrentTime(), " [ERROR] ", _args...);
	return m_last_msg;
}

