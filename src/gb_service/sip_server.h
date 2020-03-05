/*
**	********************************************************************************
**
**	File		: sip_server.cpp
**	Description	: 
**	Modify		: 2020/2/27		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
#include "eXosip2/eXosip.h"
#include "utils/safe_queue.h"
#include "defs.h"
#include "event_handler/handler_manager.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CSipServer
{
public:
    CSipServer();

    ~CSipServer();

    static CSipServer *instance();

    int Init();

    int SetLocalConfig(const std::string &sip_id, const std::string &host, uint16_t port);

    int Start(const std::string &user_agent);

    int Quit();

    eXosip_t *GetExosipContext()
    { return m_excontext; }

public:
    int DoReceiveEvents();

    int DoProcessEvents();

private:
    sip_event_sptr new_event(eXosip_t *exosip_context, eXosip_event_t *exosip_event);

private:
    uint16_t m_sipPort;
    uint64_t m_eventId;
    std::string m_sipId;
    std::string m_sipHost;
    eXosip_t *m_excontext;
    CSafeQueue <sip_event_sptr> m_eventQueue;
    CEventHandlerManager m_eventHandle;
};

}
}
}
