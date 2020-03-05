/*
**	********************************************************************************
**
**	File		: message_request.cpp
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#include <Infra/PrintLog.h>
#include "message_request.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

int CKeepaliveRequest::HandleResponse(int statcode)
{
    LOG_INFO << "Keepalive: "<<  m_reqid<<", statcode: " << statcode;
    m_statcode = statcode;
    onRequestFinished();

    return 0;
}

int CKeepaliveRequest::GetStatCode()
{
    return m_statcode;
}

}
}
}
