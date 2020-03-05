/*
**	********************************************************************************
**
**	File		: config_manager.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
#include "yaml-cpp/yaml.h"

namespace Zilu {
namespace Protocol {

struct local_sip_t {
    uint16_t        svr_port;
    std::string     svr_id;
    std::string     svr_ip;
    std::string     svr_domain;
};

struct remote_sip_t : public local_sip_t {
    std::string     passwd;
};

struct media_port_pool_t {
    uint32_t start;
    uint32_t end;
};

struct log_config_t {
    bool        enable;
    int         level;
    std::string path;
    std::string logName;
};

class CConfigManager
{
public:
    CConfigManager() = default;
    ~CConfigManager() = default;

    static CConfigManager* instance();

    ///\brief º”‘ÿ≈‰÷√Œƒº˛
    bool Load(const std::string& path);

    bool GetLocalSip(local_sip_t& localcfg);

    bool GetRemoteSip(remote_sip_t& remotecfg);

    bool GetLogConfig(log_config_t& logcfg);

    bool GetMediaPortPool(media_port_pool_t& portpool);

private:
    YAML::Node  m_node;
};

}
}