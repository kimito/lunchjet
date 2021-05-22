#pragma once

#include <fstream>
#include <string>
#include <map>
#include <memory>

#include "debug_log.h"

namespace lunchjet {

/**
 * a class to store settings for the system
 */
class SettingVariables {

    public:
    SettingVariables() {
        std::ifstream ifs("/etc/lunchjet/lunchjet.conf");
        if(!ifs){
            debug_notice("configuration file reading failed");
            return;
        }

        int no_lines = 1;
        auto buf = std::make_unique<char[]>(BUF_LEN);
        while(!ifs.eof()) {
            ifs.getline(buf.get(), BUF_LEN);
            if(ifs.bad()) {
                debug_warning("too long configuration string in line %d, skipping...", no_lines);
            }
            else {
                add_key_value(std::string(buf.get()), no_lines);
            }
        }
    }

    std::string operator[](const std::string &key) {
        return vars[key];
    }

    private:

    static const int BUF_LEN = 1024;
    std::map<std::string, std::string> vars;

    void add_key_value(const std::string str, const int line) {
        if(str.empty() || str[0] == '#') { //a line to be skipped
            return;
        }

        auto pos = str.find("=");
        if(pos == std::string::npos) {
            debug_warning("line %d unrecognized", line);
            return;
        }

        std::string key(str, 0, pos);
        std::string value(str, pos+1, str.size()-(pos+1));
        debug_notice("setting variable : %s=%s", key.c_str(), value.c_str());
        vars.insert(std::make_pair(key, value));
    }
};


} // namespace lunchjet


