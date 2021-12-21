#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

namespace fs = std::filesystem;

class Logger {
public:
	Logger();
	Logger(std::string);
	~Logger();

	template<typename Msg, typename... Args>
	std::string Write(Msg, Args...);

	template<typename Msg, typename... Args>
	std::string Info(Msg, Args...);

	template<typename Msg, typename... Args>
	std::string Info(std::ostream&, Msg, Args...);

	template<typename Msg, typename... Args>
	std::string Error(Msg, Args...);

	template<typename Msg, typename... Args>
	std::string Error(std::ostream&, Msg, Args...);

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
	auto t = time(nullptr);
	std::string current_time = (std::asctime(std::localtime(&t)));
	current_time.erase(current_time.length() - 1, 1); //Erase \n

	return current_time;
}

Logger::Logger() {
	m_log_path = "Log.txt";

	this->Info("Log created: ", m_log_path);
}

Logger::Logger(std::string _log_path) : m_log_path(_log_path) {
	this->Info("Session started.", m_log_path);
}

Logger::~Logger() {
	this->Info("Session done.");
}

template<typename Msg, typename... Args >
std::string Logger::Write(Msg _msg, Args... _args) {
	std::ostringstream stream;

	stream << std::forward<Msg>(_msg);
	std::string sep = " ";
	((stream << sep << std::forward<Args>(_args)), ...);

	m_last_msg = stream.str();

	std::fstream m_log;
	m_log.open(m_log_path, std::fstream::in | std::fstream::app);
	m_log << m_last_msg << std::endl;
	m_log.close();

	return m_last_msg;
}

template<typename Msg, typename... Args>
std::string Logger::Info(Msg _msg, Args... _args) {
	return this->Write(CurrentTime(), " [INFO] ", _msg, _args...);
}

template<typename Msg, typename... Args>
std::string Logger::Info(std::ostream& _out, Msg _msg, Args... _args) {
	_out << this->Write(CurrentTime(), " [INFO] ", _msg, _args...) << std::endl;
	return m_last_msg;
}

template<typename Msg, typename... Args>
std::string Logger::Error(Msg _msg, Args... _args) {
	return this->Write(CurrentTime(), " [ERROR] ", _msg, _args...);;
}

template<typename Msg, typename... Args>
std::string Logger::Error(std::ostream& _out, Msg _msg, Args... _args) {
	_out << this->Write(CurrentTime(), " [ERROR] ", _msg, _args...) << std::endl;
	return m_last_msg;
}
