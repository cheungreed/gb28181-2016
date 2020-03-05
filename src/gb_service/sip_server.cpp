/*
**	********************************************************************************
**
**	File		: sip_server.h
**	Description	: 
**	Modify		: 2020/2/27		zhangqiang		Create the file
**	********************************************************************************
*/
#include <netinet/in.h>
#include "sip_server.h"
#include "Infra/PrintLog.h"
#include "Infra/ThreadPool.h"
#include "event_handler/handler_manager.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

CSipServer *CSipServer::instance()
{
    static CSipServer server;
    return &server;
}

CSipServer::CSipServer() : m_sipPort(5060), m_eventId(1000000000), m_excontext(nullptr) {}

CSipServer::~CSipServer() = default;

int CSipServer::Init()
{
    m_excontext = eXosip_malloc();

    int ret = eXosip_init(m_excontext);
    if (ret != 0) {
        LOG_INFO << "eXosip_init failed, ret=" << ret;
        return -1;
    }
    LOG_INFO << "eXosip_init successfully!";

    return 0;
}

int CSipServer::SetLocalConfig(const std::string &sip_id, const std::string &host, uint16_t port)
{
    m_sipId = sip_id;
    m_sipHost = host;
    m_sipPort = port;

    return 0;
}

int CSipServer::Start(const std::string &user_agent)
{
    int ret = eXosip_listen_addr(m_excontext, IPPROTO_UDP,
            m_sipHost.c_str(), m_sipPort, AF_INET, 0);
    if (ret != 0) {
        eXosip_quit(m_excontext);
        LOG_INFO << "eXosip_listen_addr failed, ret: " << ret;
        return -1;
    }

    if (user_agent.length() > 0)
        eXosip_set_user_agent(m_excontext, user_agent.c_str());

    Infra::ThreadTask recvTask = std::bind(&CSipServer::DoReceiveEvents, this);
    Infra::ThreadTask procTask1 = std::bind(&CSipServer::DoProcessEvents, this);

    Infra::CThreadPool::instance()->run(recvTask);
    Infra::CThreadPool::instance()->run(procTask1);

    return 0;
}

int CSipServer::Quit()
{
    eXosip_quit(m_excontext);
    return 0;
}

int CSipServer::DoReceiveEvents()
{
    eXosip_event_t *exosip_event;

    exosip_event = eXosip_event_wait(m_excontext, 0, 1);

//    eXosip_lock(m_excontext);
//    eXosip_automatic_action(m_excontext);
//    eXosip_unlock(m_excontext);

    if (exosip_event == nullptr)
        return 0;

    sip_event_sptr sipg_event = new_event(m_excontext, exosip_event);
    if (nullptr == sipg_event)
        return 0;

    m_eventQueue.Push(sipg_event);
    LOG_DEBUG << "Push event: " << sipg_event->name << ", id=" << sipg_event->id << " to queue successfully";
    return 0;
}

int CSipServer::DoProcessEvents()
{
    const char *event_name;
    uint64_t event_id;

    sip_event_sptr sipg_event;
    if (!m_eventQueue.Pop(sipg_event))
        return 0;

    event_name = sipg_event->name;
    event_id = sipg_event->id;

    sipg_event->proc(sipg_event);
    eXosip_event_free(sipg_event->exevent);
    return 0;
}

sip_event_sptr CSipServer::new_event(eXosip_t *exosip_context, eXosip_event_t *exosip_event)
{
    if (exosip_event == nullptr)
        return nullptr;

    if (exosip_event->type < EXOSIP_REGISTRATION_SUCCESS || exosip_event->type > EXOSIP_NOTIFICATION_GLOBALFAILURE)
        return nullptr;

    sip_event_sptr event(new sip_event_t);// = std::make_shared(SipEvent)();
    CEventHandlerManager::EventNameProcPair pair = m_eventHandle.GetEventProc(exosip_event->type);
    if (pair.name == nullptr)
        return nullptr;

    event->name = pair.name;
    event->proc = pair.proc;
    event->excontext = exosip_context;
    event->exevent = exosip_event;
    event->id = m_eventId++;

    return event;
}

}
}
}
