#include "tiny_file_io.h"
namespace tiny {

tiny_conf_t tiny_read_conf(const tiny_string_t &filename) {
    tiny_conf_t conf;
    tiny_dict_t kv;
    tiny_array_string_t cfgs = tiny_file_readlines(filename);
    for (auto & cfg : cfgs) {
        if (cfg.front() == '#')
            continue;
        int i = cfg.find_first_of('=');
        if (i == std::string::npos)
            continue;
        cfg[i] = ' ';
        std::istringstream buf(cfg);
        tiny_string_t key, val;
        buf >> key >> val;
        kv[key] = val;
    }
    conf.port = stoi(kv["port"]);
    conf.logpath = kv["logpath"];     
    conf.loglevel = (logger::Level)stoi(kv["loglevel"]);
    conf.assets = kv["assets"];
    conf.tpl = kv["template"];
    return conf;
}
std::ostream &operator<<(std::ostream &out, tiny_conf_t const &conf){
    std::ostringstream buf;
    buf << "[Config]" << " port: " << conf.port << '\n';
    buf << "[Config]" << " loglevel: " << conf.loglevel << '\n';
    buf << "[Config]" << " logpath: " << conf.logpath << '\n';
    buf << "[Config]" << " assets: " << conf.assets << '\n';
    buf << "[Config]" << " template: " << conf.tpl << '\n';
    return out << buf.str();
}

tiny_int_t tiny_file_read(const tiny_string_t &filename,
                          tiny_string_t &userbuf) {
    std::ifstream file("./" + filename,
                       std::ifstream::binary | std::ifstream::in);
    if (!file)
        return TINY_ERROR;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    userbuf = buffer.str();
    return TINY_SUCCESS;
}

tiny_array_string_t tiny_file_readlines(const tiny_string_t &filename) {
    tiny_array_string_t elems;
    tiny_string_t content;
    int n = tiny_file_read(filename, content);
    std::stringstream ss(content);
    tiny_string_t item;
    while (getline(ss, item)) {
        elems.push_back(item);
    }
    return elems;
}
}
