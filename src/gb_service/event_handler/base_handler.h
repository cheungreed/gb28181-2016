/*
**	********************************************************************************
**
**	File		: base_handler.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <osipparser2/osip_message.h>

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CBaseHandler
{
public:
    CBaseHandler() = default;
    virtual ~CBaseHandler() = default;

protected:
    int sendSimplyResp(const char* uname, struct eXosip_t* excontext, int tid, int status);

    int sendCallAck(struct eXosip_t *excontext, int did);

    int getStatcodeFromResp(osip_message_t* resp);

    const char* getMsgIdFromReq(osip_message_t* req);
};

}
}
}