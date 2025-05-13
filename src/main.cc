#include <util/Logger.h>
#include <ZincArguments.h>
#include <ZincConfig.h>
#include <network/minecraft/ZincServer.h>
#include <network/srvctl/SrvCtlServer.h>
#include <stdexcept>
#include <thread>

void ThreadMain() {
    zinc::g_zincServer.setPort(zinc::g_zincConfig.m_serverPort);
    zinc::g_zincServer.start();
}
void ThreadSrvCtl() {
    zinc::g_srvCtlServer.setPort(zinc::g_zincConfig.m_srvctlPort);
    zinc::g_srvCtlServer.start();
}

int main(int argc, char **argv) {
    try {
        zinc::Logger("Main").info("Starting Zinc server...");
        zinc::g_zincArguments.parse(argc, argv);
        zinc::g_zincConfig.load(zinc::g_zincArguments.m_workDir);
        std::thread t1 (ThreadMain);
        std::thread t2 (ThreadSrvCtl);
        t1.join();
        t2.join();
    } catch (const std::exception& e) {
        zinc::Logger("Zinc").error("Error: " + std::string(e.what()), true);
    }
    return 0;
}
