#include "pika_command.h"
#include "pika_server.h"
#include <algorithm>
#include <map>

extern PikaServer *g_pikaServer;
extern std::map<std::string, Cmd *> g_pikaCmd;

void SetCmd::Do(std::list<std::string> &argv, std::string &ret) {
    if ((arity > 0 && argv.size() != arity) || (arity < 0 && argv.size() < -arity)) {
        ret = "-ERR wrong number of arguments for ";
        ret.append(argv.front());
        ret.append(" command\r\n");
        return;
    }
    argv.pop_front();
    std::string key = argv.front();
    argv.pop_front();
    std::string value = argv.front();
    argv.pop_front();
    if (argv.size() > 0) {
        std::string opt = argv.front();
        argv.pop_front();
        transform(opt.begin(), opt.end(), opt.begin(), ::tolower);
        if (opt == "xx") {
            ret = "+OK\r\n";
            return;
        } else if (opt == "nx" && argv.empty()) {
            ret = "+OK\r\n";
        } else if (opt == "ex") {
            if (argv.size() != 1) {
                ret = "-ERR syntax error\r\n";
                return;
            }
            ret = "+OK\r\n";
        } else if (opt == "px") {
            if (argv.size() != 1) {
                ret = "-ERR syntax error\r\n";
                return;
            }
            ret = "+OK\r\n";
        } else {
            ret = "-ERR syntax error\r\n";
            return;
        }
    }
    nemo::Status s = g_pikaServer->GetHandle()->Set(key, value);
    if (s.ok()) {
        ret = "+OK\r\n";
    } else {
        ret.append("-ERR ");
        ret.append(s.ToString().c_str());
        ret.append("\r\n");
    }
}

void GetCmd::Do(std::list<std::string> &argv, std::string &ret) {
    if ((arity > 0 && argv.size() != arity) || (arity < 0 && argv.size() < -arity)) {
        ret = "-ERR wrong number of arguments for ";
        ret.append(argv.front());
        ret.append(" command\r\n");
        return;
    }
    argv.pop_front();
    std::string key = argv.front();
    argv.pop_front();
    std::string value;
    nemo::Status s = g_pikaServer->GetHandle()->Get(key, &value);
    if (s.ok()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "$%d\r\n", (int)value.size());
        ret.append(buf);
        ret.append(value.data(), value.size());
        ret.append("\r\n");
    } else if (s.IsNotFound()) {
        ret.append("$-1\r\n");
    } else {
        ret.append("-ERR ");
        ret.append(s.ToString().c_str());
        ret.append("\r\n");
    }
}