/*
**	********************************************************************************
**
**	File		: handler_manager.h
**	Description	: 
**	Modify		: 2020/2/27		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <map>
#include <eXosip2/eXosip.h>
#include "gb_service/defs.h"
#include "call_handler.h"
#include "message_handler.h"
#include "register_handler.h"
#include "subscribe_handler.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

/// 事件处理对象，每种Event，对应一个处理方法
class CEventHandlerManager
{
public:
    CEventHandlerManager();

    ~CEventHandlerManager();

    typedef struct
    {
        const char *name;
        event_proc proc;
    } EventNameProcPair;

public:
    EventNameProcPair GetEventProc(eXosip_event_type_t type);

public:
    /* REGISTER related events */
    int on_exosip_registration_success(const sip_event_sptr &event);

    int on_exosip_registration_failure(const sip_event_sptr &event);

public:
    /* INVITE related events within calls */
    int on_exosip_call_invite(const sip_event_sptr &event);

    int on_exosip_call_reinvite(const sip_event_sptr &event);

    int on_exosip_call_noanswer(const sip_event_sptr &event);

    int on_exosip_call_proceeding(const sip_event_sptr &event);

    int on_exosip_call_ringing(const sip_event_sptr &event);

    int on_exosip_call_answered(const sip_event_sptr &event);

    int on_exosip_call_redirected(const sip_event_sptr &event);

    int on_exosip_call_requestfailure(const sip_event_sptr &event);

    int on_exosip_call_serverfailure(const sip_event_sptr &event);

    int on_exosip_call_globalfailure(const sip_event_sptr &event);

    int on_exosip_call_ack(const sip_event_sptr &event);

    int on_exosip_call_cancelled(const sip_event_sptr &event);

public:
    /* request related events within calls (except INVITE) */
    int on_exosip_call_message_new(const sip_event_sptr &event);

    int on_exosip_call_message_proceeding(const sip_event_sptr &event);

    int on_exosip_call_message_answered(const sip_event_sptr &event);

    int on_exosip_call_message_redirected(const sip_event_sptr &event);

    int on_exosip_call_message_requestfailure(const sip_event_sptr &event);

    int on_exosip_call_message_serverfailure(const sip_event_sptr &event);

    int on_exosip_call_message_globalfailure(const sip_event_sptr &event);

    int on_exosip_call_closed(const sip_event_sptr &event);

public:
    /* for both UAS & UAC events */
    int on_exosip_call_released(const sip_event_sptr &event);

public:
    /* events received for request outside calls */
    int on_exosip_message_new(const sip_event_sptr &event);

    int on_exosip_message_proceeding(const sip_event_sptr &event);

    int on_exosip_message_answered(const sip_event_sptr &event);

    int on_exosip_message_redirected(const sip_event_sptr &event);

    int on_exosip_message_requestfailure(const sip_event_sptr &event);

    int on_exosip_message_serverfailure(const sip_event_sptr &event);

    int on_exosip_message_globalfailure(const sip_event_sptr &event);

public:
    /* Presence and Instant Messaging */
    int on_exosip_subscription_noanswer(const sip_event_sptr &event);

    int on_exosip_subscription_proceeding(const sip_event_sptr &event);

    int on_exosip_subscription_answered(const sip_event_sptr &event);

    int on_exosip_subscription_redirected(const sip_event_sptr &event);

    int on_exosip_subscription_requestfailure(const sip_event_sptr &event);

    int on_exosip_subscription_serverfailure(const sip_event_sptr &event);

    int on_exosip_subscription_globalfailure(const sip_event_sptr &event);

    int on_exosip_subscription_notify(const sip_event_sptr &event);

public:
    int on_exosip_in_subscription_new(const sip_event_sptr &event);

public:
    int on_exosip_notification_noanswer(const sip_event_sptr &event);

    int on_exosip_notification_proceeding(const sip_event_sptr &event);

    int on_exosip_notification_answered(const sip_event_sptr &event);

    int on_exosip_notification_redirected(const sip_event_sptr &event);

    int on_exosip_notification_requestfailure(const sip_event_sptr &event);

    int on_exosip_notification_serverfailure(const sip_event_sptr &event);

    int on_exosip_notification_globalfailure(const sip_event_sptr &event);

private:
    std::map<uint32_t , EventNameProcPair> m_eventProcMap;
//    CEventHandlerRegister *m_RegisterHandler;
    CMessageHandler         m_msghandler;
//    CEventHandlerCall *m_CallHandler;
//    CEventHandlerSubscribe *m_pSubscribeHandler;
};

}
}
}
