/*
**	********************************************************************************
**
**	File		: handler_manager.cpp
**	Description	: 
**	Modify		: 2020/2/27		zhangqiang		Create the file
**	********************************************************************************
*/
#include <gb_service/gb28181_svr_manager.h>
#include "handler_manager.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

#define CALLBACK_TEMPLATE(F) (std::bind(&CEventHandlerManager::F, this, std::placeholders::_1))

CEventHandlerManager::CEventHandlerManager()
{
    ///初始化Map表
    EventNameProcPair eventProcTable[EXOSIP_EVENT_COUNT] = {
            {"EXOSIP_REGISTRATION_SUCCESS",        CALLBACK_TEMPLATE(on_exosip_registration_success)},
            {"EXOSIP_REGISTRATION_FAILURE",        CALLBACK_TEMPLATE(on_exosip_registration_failure)},
            {"EXOSIP_CALL_INVITE",                 CALLBACK_TEMPLATE(on_exosip_call_invite)},
            {"EXOSIP_CALL_REINVITE",               CALLBACK_TEMPLATE(on_exosip_call_reinvite)},
            {"EXOSIP_CALL_NOANSWER",               CALLBACK_TEMPLATE(on_exosip_call_noanswer)},
            {"EXOSIP_CALL_PROCEEDING",             CALLBACK_TEMPLATE(on_exosip_call_proceeding)},
            {"EXOSIP_CALL_RINGING",                CALLBACK_TEMPLATE(on_exosip_call_ringing)},
            {"EXOSIP_CALL_ANSWERED",               CALLBACK_TEMPLATE(on_exosip_call_answered)},
            {"EXOSIP_CALL_REDIRECTED",             CALLBACK_TEMPLATE(on_exosip_call_redirected)},
            {"EXOSIP_CALL_REQUESTFAILURE",         CALLBACK_TEMPLATE(on_exosip_call_requestfailure)},
            {"EXOSIP_CALL_SERVERFAILURE",          CALLBACK_TEMPLATE(on_exosip_call_serverfailure)},
            {"EXOSIP_CALL_GLOBALFAILURE",          CALLBACK_TEMPLATE(on_exosip_call_globalfailure)},
            {"EXOSIP_CALL_ACK",                    CALLBACK_TEMPLATE(on_exosip_call_ack)},
            {"EXOSIP_CALL_CANCELLED",              CALLBACK_TEMPLATE(on_exosip_call_cancelled)},
            {"EXOSIP_CALL_MESSAGE_NEW",            CALLBACK_TEMPLATE(on_exosip_call_message_new)},
            {"EXOSIP_CALL_MESSAGE_PROCEEDING",     CALLBACK_TEMPLATE(on_exosip_call_message_proceeding)},
            {"EXOSIP_CALL_MESSAGE_ANSWERED",       CALLBACK_TEMPLATE(on_exosip_call_message_answered)},
            {"EXOSIP_CALL_MESSAGE_REDIRECTED",     CALLBACK_TEMPLATE(on_exosip_call_message_redirected)},
            {"EXOSIP_CALL_MESSAGE_REQUESTFAILURE", CALLBACK_TEMPLATE(on_exosip_call_message_requestfailure)},
            {"EXOSIP_CALL_MESSAGE_SERVERFAILURE",  CALLBACK_TEMPLATE(on_exosip_call_message_serverfailure)},
            {"EXOSIP_CALL_MESSAGE_GLOBALFAILURE",  CALLBACK_TEMPLATE(on_exosip_call_message_globalfailure)},
            {"EXOSIP_CALL_CLOSED",                 CALLBACK_TEMPLATE(on_exosip_call_closed)},
            {"EXOSIP_CALL_RELEASED",               CALLBACK_TEMPLATE(on_exosip_call_released)},
            {"EXOSIP_MESSAGE_NEW",                 CALLBACK_TEMPLATE(on_exosip_message_new)},
            {"EXOSIP_MESSAGE_PROCEEDING",          CALLBACK_TEMPLATE(on_exosip_message_proceeding)},
            {"EXOSIP_MESSAGE_ANSWERED",            CALLBACK_TEMPLATE(on_exosip_message_answered)},
            {"EXOSIP_MESSAGE_REDIRECTED",          CALLBACK_TEMPLATE(on_exosip_message_redirected)},
            {"EXOSIP_MESSAGE_REQUESTFAILURE",      CALLBACK_TEMPLATE(on_exosip_message_requestfailure)},
            {"EXOSIP_MESSAGE_SERVERFAILURE",       CALLBACK_TEMPLATE(on_exosip_message_serverfailure)},
            {"EXOSIP_MESSAGE_GLOBALFAILURE",       CALLBACK_TEMPLATE(on_exosip_message_globalfailure)},
            {"EXOSIP_SUBSCRIPTION_NOANSWER",       CALLBACK_TEMPLATE(on_exosip_subscription_noanswer)},
            {"EXOSIP_SUBSCRIPTION_PROCEEDING",     CALLBACK_TEMPLATE(on_exosip_subscription_proceeding)},
            {"EXOSIP_SUBSCRIPTION_ANSWERED",       CALLBACK_TEMPLATE(on_exosip_subscription_answered)},
            {"EXOSIP_SUBSCRIPTION_REDIRECTED",     CALLBACK_TEMPLATE(on_exosip_subscription_redirected)},
            {"EXOSIP_SUBSCRIPTION_REQUESTFAILURE", CALLBACK_TEMPLATE(on_exosip_subscription_requestfailure)},
            {"EXOSIP_SUBSCRIPTION_SERVERFAILURE",  CALLBACK_TEMPLATE(on_exosip_subscription_serverfailure)},
            {"EXOSIP_SUBSCRIPTION_GLOBALFAILURE",  CALLBACK_TEMPLATE(on_exosip_subscription_globalfailure)},
            {"EXOSIP_SUBSCRIPTION_NOTIFY",         CALLBACK_TEMPLATE(on_exosip_subscription_notify)},
            {"EXOSIP_IN_SUBSCRIPTION_NEW",         CALLBACK_TEMPLATE(on_exosip_in_subscription_new)},
            {"EXOSIP_NOTIFICATION_NOANSWER",       CALLBACK_TEMPLATE(on_exosip_notification_noanswer)},
            {"EXOSIP_NOTIFICATION_PROCEEDING",     CALLBACK_TEMPLATE(on_exosip_notification_proceeding)},
            {"EXOSIP_NOTIFICATION_ANSWERED",       CALLBACK_TEMPLATE(on_exosip_notification_answered)},
            {"EXOSIP_NOTIFICATION_REDIRECTED",     CALLBACK_TEMPLATE(on_exosip_notification_redirected)},
            {"EXOSIP_NOTIFICATION_REQUESTFAILURE", CALLBACK_TEMPLATE(on_exosip_notification_requestfailure)},
            {"EXOSIP_NOTIFICATION_SERVERFAILURE",  CALLBACK_TEMPLATE(on_exosip_notification_serverfailure)},
            {"EXOSIP_NOTIFICATION_GLOBALFAILURE",  CALLBACK_TEMPLATE(on_exosip_notification_globalfailure)}
    };
    for (uint32_t i = 0; i < EXOSIP_EVENT_COUNT; ++i)
    {
        m_eventProcMap.insert(std::make_pair(i, eventProcTable[i]));
    }
}

CEventHandlerManager::~CEventHandlerManager()
{
}

CEventHandlerManager::EventNameProcPair CEventHandlerManager::GetEventProc(eXosip_event_type_t type)
{
    if (type > EXOSIP_EVENT_COUNT)
    {
        EventNameProcPair pair = {"", nullptr};
        LOG_INFO << "Event Type: " << type << " don't exist!";
        return pair;
    }

    auto value = m_eventProcMap.find(type);
    return value->second;
}

/* REGISTER related events */
int CEventHandlerManager::on_exosip_registration_success(const sip_event_sptr &event)
{
    //TODO 通知注册成功
    CGB28181SvrManager::instance()->OnRegisterSuccess(event->exevent->rid);
    return 0;
}

int CEventHandlerManager::on_exosip_registration_failure(const sip_event_sptr &event)
{
    ///TODO REGISTER send twice, this is question!
    LOG_WARN << "register id: " << event->exevent->rid << " need authentication!";
    eXosip_lock(event->excontext);
    eXosip_automatic_action(event->excontext);
    eXosip_unlock(event->excontext);
    return 0;
}

/* INVITE related events within calls */
int CEventHandlerManager::on_exosip_call_invite(const sip_event_sptr &event)
{
//    m_CallHandler->HandleInviteRequest(event);
    return 0;
}

int CEventHandlerManager::on_exosip_call_reinvite(const sip_event_sptr &event)
{
    return 0;
}


int CEventHandlerManager::on_exosip_call_noanswer(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_proceeding(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseProcessing(event);
//    m_CallHandler->HandleResponseSuccess(event);
    return 0;
}


int CEventHandlerManager::on_exosip_call_ringing(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseProcessing(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_answered(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseSuccess(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_redirected(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_requestfailure(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_serverfailure(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_globalfailure(const sip_event_sptr &event)
{
//    m_CallHandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_ack(const sip_event_sptr &event)
{
    return 0;
}


int CEventHandlerManager::on_exosip_call_cancelled(const sip_event_sptr &event)
{
//    m_CallHandler->HandleClose(event);

    return 0;
}

/* request related events within calls (except INVITE) */
int CEventHandlerManager::on_exosip_call_message_new(const sip_event_sptr &event)
{
    eXosip_event_t *exosip_event = event->exevent;

    if (!strncmp(exosip_event->request->sip_method, "MESSAGE", strlen("MESSAGE")))
    {
        m_msghandler.HandleIncomingReq(event);
    }
    return 0;
}

int CEventHandlerManager::on_exosip_call_message_proceeding(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_call_message_answered(const sip_event_sptr &event)
{
//    m_CallHandler->HandleInfoResponseSuccess(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_message_redirected(const sip_event_sptr &event)
{
//    m_CallHandler->HandleInfoResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_message_requestfailure(const sip_event_sptr &event)
{
//    m_CallHandler->HandleInfoResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_message_serverfailure(const sip_event_sptr &event)
{
//    m_CallHandler->HandleInfoResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_call_message_globalfailure(const sip_event_sptr &event)
{
//    m_CallHandler->HandleInfoResponseFailure(event);

    return 0;
}


int CEventHandlerManager::on_exosip_call_closed(const sip_event_sptr &event)
{
//    m_CallHandler->HandleClose(event);

    return 0;
}


/* for both UAS & UAC events */
int CEventHandlerManager::on_exosip_call_released(const sip_event_sptr &event)
{
//    m_CallHandler->HandleClose(event);

    return 0;
}


/* events received for request outside calls */
int CEventHandlerManager::on_exosip_message_new(const sip_event_sptr &event)
{
    eXosip_event_t *exosip_event = event->exevent;

    if (!strncmp(exosip_event->request->sip_method, "REGISTER", strlen("REGISTER")))
    {
//        m_RegisterHandler->HandleRequest(event);
    }
    else if (!strncmp(exosip_event->request->sip_method, "MESSAGE", strlen("MESSAGE")))
    {
        m_msghandler.HandleIncomingReq(event);
    }

    return 0;
}

int CEventHandlerManager::on_exosip_message_proceeding(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_message_answered(const sip_event_sptr &event)
{
    m_msghandler.HandleResponseSuccess(event);

    return 0;
}

int CEventHandlerManager::on_exosip_message_redirected(const sip_event_sptr &event)
{
//    m_msghandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_message_requestfailure(const sip_event_sptr &event)
{
//    m_msghandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_message_serverfailure(const sip_event_sptr &event)
{
//    m_msghandler->HandleResponseFailure(event);

    return 0;
}

int CEventHandlerManager::on_exosip_message_globalfailure(const sip_event_sptr &event)
{
//    m_msghandler->HandleResponseFailure(event);

    return 0;
}

/* Presence and Instant Messaging */
int CEventHandlerManager::on_exosip_subscription_noanswer(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_subscription_proceeding(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_subscription_answered(const sip_event_sptr &event)
{
//    m_pSubscribeHandler->HandleResponseSuccess(event);
    return 0;
}

int CEventHandlerManager::on_exosip_subscription_redirected(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_subscription_requestfailure(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_subscription_serverfailure(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_subscription_globalfailure(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_subscription_notify(const sip_event_sptr &event)
{
    m_msghandler.HandleIncomingReq(event);
    return 0;
}

int CEventHandlerManager::on_exosip_in_subscription_new(const sip_event_sptr &event)
{
    return 0;
}


int CEventHandlerManager::on_exosip_notification_noanswer(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_notification_proceeding(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_notification_answered(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_notification_redirected(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_notification_requestfailure(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_notification_serverfailure(const sip_event_sptr &event)
{
    return 0;
}

int CEventHandlerManager::on_exosip_notification_globalfailure(const sip_event_sptr &event)
{
    return 0;
}

}
}
}
