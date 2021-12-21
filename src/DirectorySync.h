#pragma once

#include <filesystem>

#include <string>

#include "Logger.h"

namespace fs = std::filesystem;

class DirectorySync {
public:
	DirectorySync(const fs::path&, const fs::path&);
	~DirectorySync();

	void StartSync();

	std::vector<fs::path> TargetDirContentList();
	std::vector<fs::path> BackupDirContentList();

private:
	fs::path	m_target_dir;
	fs::path	m_backup_dir;

	Logger*		m_log;
};

DirectorySync::DirectorySync(const fs::path& _target, const fs::path& _backup) {
	m_target_dir = _target;
	m_backup_dir = _backup;
	m_log = new Logger();
	m_log->Info(std::cout, "Created target/backup pair: ", m_target_dir, m_backup_dir);
}

DirectorySync::~DirectorySync() {
	delete m_log;
}

void DirectorySync::StartSync() {
	m_log->Info(std::cout, "Start sync");

	for (auto p : TargetDirContentList()) {
		fs::path r_path = p.string().erase(0, m_target_dir.string().length());
		fs::path backup_path = m_backup_dir.string() + r_path.string();

		std::error_code EC;
		if (!fs::exists(backup_path)) {
			m_log->Info(std::cout, "Copying: ", p);
			fs::copy(p, backup_path, EC);
		}
		else if (fs::last_write_time(p) > fs::last_write_time(backup_path)) {
			m_log->Info(std::cout, "Updating: ", backup_path);
			fs::copy(p, backup_path, fs::copy_options::update_existing, EC);
		}
		if (EC)
			m_log->Error(std::cerr, EC.message());
	}
	
	for (auto p : BackupDirContentList()) {
		fs::path r_path = p.string().erase(0, m_backup_dir.string().length());
		fs::path target_path = m_target_dir.string() + r_path.string();
		if (!fs::exists(target_path)) {
			m_log->Info(std::cout, "Removing: ", p);
			std::error_code EC;
			if (fs::is_directory(p))
				fs::remove_all(p, EC);
			else
				fs::remove(p, EC);
			if (EC) {
				m_log->Error(std::cerr, EC.message());
			}
		}
	}
	
	m_log->Info(std::cout, "Sync Done");
}

std::vector<fs::path> DirectorySync::TargetDirContentList() {
	std::vector<fs::path> content;
	for (const auto& entry : fs::recursive_directory_iterator(m_target_dir))
		content.push_back(entry.path());

	return content;
}

std::vector<fs::path> DirectorySync::BackupDirContentList() {
	std::vector<fs::path> content;
	for (const auto& entry : fs::recursive_directory_iterator(m_backup_dir))
		content.push_back(entry.path());

	return content;
}