/*
**	********************************************************************************
**
**	File		: gb28181_svr_manager.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <gb_service/event_handler/sdp/call_defs.h>
#include "gb_service/manscdp/request_sender.h"
#include "config_manager.h"
#include "Infra/Thread.h"
#include "gb_service/manscdp/defs/defs.h"
#include "utils/ptz/defs.h"
#include "defs.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CGB28181SvrManager
{
public:
    CGB28181SvrManager();
    ~CGB28181SvrManager();

    static CGB28181SvrManager* instance();

    int Init();
    int Start();

    int OnRegisterSuccess(int rid);

    int HandleDeviceControl(manscdp_devicecontrol_subcmd_e cmd, string &devid,
                            manscdp_switch_status_e onoff);

    int HandlePTZControl(control_cmd_t &ctrlcmd, string &devid);

    ///< Play Download Talk Playback
    int HandleMediaRequest(sdp_description_t &req_sdp, const sip_event_sptr &e);

private:
    int register_online();

    void heartbeat_proc(void *param);

    int keepalive();

private:
    bool                m_bRegok;           //×¢²á³É¹¦
    local_sip_t         m_localsipcfg;
    remote_sip_t        m_remotesipcfg;

    CMsgSender          m_msgsender;
    Infra::CThread      m_heartbeat;
};

}
}
}
