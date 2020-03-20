/*
**	********************************************************************************
**
**	File		: msg_sender.cpp
**	Description	: 
**	Modify		: 2020/
 * 2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#include <string>
#include <gb_service/request_manager/base_request.h>
#include <gb_service/request_manager/request_manager.h>
#include "request_sender.h"
#include "eXosip2/eXosip.h"
using  namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

extern string g_sipfrom_ipport;
extern string g_sipfrom_domain;
extern string g_sipproxy_ipport;
extern string g_sipproxy_domain;
extern eXosip_t *g_excontext;

struct exosip_guard_t {
    explicit exosip_guard_t(eXosip_t* e) {
        m_excontext = e;
        eXosip_lock(m_excontext);
    }
    ~exosip_guard_t() {
        eXosip_unlock(m_excontext);
    }
    eXosip_t* m_excontext;
};

CMsgSender::CMsgSender()
{

}

CMsgSender::~CMsgSender()
{

}

int CMsgSender::RegisterOnline(remote_sip_t &remote)
{
    osip_message_t * reg;

    exosip_guard_t guard(g_excontext);
    int rid = eXosip_register_build_initial_register(g_excontext,
            g_sipfrom_ipport.c_str(),
            g_sipproxy_ipport.c_str(),
            g_sipfrom_ipport.c_str(), 3600, &reg);
    if (rid < 1) {
        LOG_ERROR << "build register failed, nRet: " << rid;
        return -1;
    }

    eXosip_add_authentication_info(g_excontext, remote.svr_id.c_str(), remote.svr_id.c_str(),
                                   remote.passwd.c_str(), "MD5", nullptr);

    int r = eXosip_register_send_register(g_excontext, rid, reg);
    if (r != 0) {
        LOG_ERROR << "send register failed, ret: " << r;
        return -1;
    }
    return 0;
}

int CMsgSender::Keepalive(remote_sip_t &remote, local_sip_t &local, bool bwait, CBaseRequestSPtr &request)
{
    const char* cmd_type = "Keepalive";

    char body[1024];
    memset(body, 0x0, sizeof(body));
    string msg_sn = generate_sn();

    const char* pattern = "<?xml version=\"1.0\"?>\r\n"
                          "<Notify>\r\n"
                          "<CmdType>%s</CmdType>\r\n"
                          "<SN>%s</SN>\r\n"
                          "<DeviceID>%s</DeviceID>\r\n"
                          "<Status>OK</Status>\r\n"
                          "</Notify>\r\n";
    snprintf(body, sizeof(body), pattern, cmd_type, msg_sn.c_str(), local.svr_id.c_str());

    request = make_shared<CKeepaliveRequest>(msg_sn);
    request->SetWait(bwait);
    int r = send_message(remote, body, request);
    if (r != 0) {
        return r;
    }
    return 0;
}

string CMsgSender::generate_sn()
{
    stringstream sn;
    sn << m_sn.fetch_add(1);
    return sn.str();
}

int CMsgSender::send_message(remote_sip_t &remote, const char *body, CBaseRequestSPtr &request)
{
    osip_message_t* msg = nullptr;

    exosip_guard_t guard(g_excontext);

    int r = eXosip_message_build_request(g_excontext, &msg, "MESSAGE",
            g_sipproxy_ipport.c_str(), g_sipfrom_ipport.c_str(), nullptr);
    if (r != OSIP_SUCCESS) {
        LOG_ERROR << "Build message request failed, sipto: " << g_sipproxy_ipport << ", ret: " << r;
        return -1;
    }

    osip_message_set_body(msg, body, strlen(body));
    osip_message_set_content_type(msg, "Application/MANSCDP+xml");
    int tid = eXosip_message_send_request(g_excontext, msg);
    if (tid < 1) {
        LOG_ERROR <<"Send message request failed, sipto: " << g_sipproxy_ipport << ", tid: " << tid;
        return -2;
    }

    string reqid = get_reqid_from_request(msg);
    if (request) {
        if (reqid.length() >0) {
            CRequestManager::instance()->AddRequest(reqid, request);
        }
    }
    return 0;
}

const char *CMsgSender::get_reqid_from_request(osip_message_t *msg)
{
    osip_generic_param_t* tag = nullptr;
    osip_to_get_tag(msg->from, &tag);
    if (nullptr == tag || nullptr == tag->gvalue) {
        return nullptr;
    }
    return (const char*) tag->gvalue;
}

}
}
}