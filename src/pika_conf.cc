#include "sys/stat.h"
#include "base_conf.h"
#include "pika_conf.h"


PikaConf::PikaConf(const char* path) :
    BaseConf(path)
{
    getConfInt("port", &port_);
    getConfInt("thread_num", &thread_num_);
    getConfStr("log_path", log_path_);
    getConfInt("log_level", &log_level_);
    getConfStr("db_path", db_path_);
    getConfInt("write_buffer_size", &write_buffer_size_);
    getConfInt("timeout", &timeout_);
    getConfStr("requirepass", requirepass_);
}
