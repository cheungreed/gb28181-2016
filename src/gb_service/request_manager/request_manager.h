/*
**	********************************************************************************
**
**	File		: request_manager.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
#include "base_request.h"
#include "message_request.h"

using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CRequestManager
{
public:
    CRequestManager() = default;
    ~CRequestManager() = default;

    static CRequestManager * instance();

    int Start();

    int AddRequest(string& reqid, CBaseRequestSPtr& req);

    int DelRequest(outcoming_req_type_e reqtype, string& reqid);

    int GetMsgRequestBySn(const string& reqsn, CMessageRequestSptr& msgreq);

    int HandleMsgResponse(string& reqid, int status_code);

private:
    int check_requet_timeout();

    base_request_map_t * get_base_request_map(outcoming_req_type_e requtype);

    int handle_response(base_request_map_t& reqmap, string& reqid, int status_code);

private:
    base_request_map_t m_messagemap;
};

}
}
}
