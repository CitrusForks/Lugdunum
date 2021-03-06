#include <lug/Config.hpp>
#include <lug/System/Exception.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

int main(int, const char*[]) {
    auto logger = lug::System::Logger::makeLogger("myLogger");
    logger->addHandler(lug::System::Logger::makeHandler<lug::System::Logger::StdoutHandler>("myLoggerStdout"));

    logger->info("Starting the app!");

    try {
        LUG_EXCEPT(IOException, "My exception worked");
    }
    catch (const lug::System::IOException& e) {
        logger->error("Exception at line {} ({})", e.getLine(), e.getDescription());
    }

    return 0;
}
