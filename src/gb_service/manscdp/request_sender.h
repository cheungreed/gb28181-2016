/*
**	********************************************************************************
**
**	File		: msg_sender.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <atomic>
#include <osipparser2/osip_message.h>
#include "config_manager.h"
#include "gb_service/request_manager/base_request.h"
using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CMsgSender
{
public:
    CMsgSender();
    ~CMsgSender();

    int RegisterOnline(remote_sip_t &remote);

    int Keepalive(remote_sip_t &remote, local_sip_t &local, bool bwait, CBaseRequestSPtr &request);

private:
    string generate_sn();

    int send_message(remote_sip_t &remote, const char *body, CBaseRequestSPtr &request);

    const char * get_reqid_from_request(osip_message_t* msg);

private:
    atomic<uint64_t > m_sn;
};

}
}
}