#include <iostream>

#include "utils/logger.hpp"

using namespace logger;
using namespace util;


int main() {


    LoggerConfig config;
    config.log_to_file = false;
    config.use_color = true;
    config.ts_type = time::timestamp_t::Partial;

    Logger::singleton().config(config);


    Logger::log(StringLog("Message"));

    return 0;
}
