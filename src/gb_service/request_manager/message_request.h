/*
**	********************************************************************************
**
**	File		: message_request.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
#include "base_request.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CMessageRequest : public CBaseRequest
{
public:
    CMessageRequest(outcoming_req_type_e reqtype, const string& reqsn) : CBaseRequest(reqtype), m_reqsn(reqsn) {}
    virtual ~CMessageRequest() {}

    const string& GetReqSn() {
        return m_reqsn;
    }

private:
    string m_reqsn;
};
typedef shared_ptr<CMessageRequest> CMessageRequestSptr;

///< ±£»îÇëÇó
class CKeepaliveRequest : public CMessageRequest
{
public:
    CKeepaliveRequest(const string& reqsn) : CMessageRequest(OUTCOMING_REQU_TYPE_KEEPALIVE, reqsn) {}
    virtual ~CKeepaliveRequest() {}

    virtual int HandleResponse(int statcode);

    /// -1: keepalive failed; 200: OK
    int GetStatCode();

private:
    int m_statcode;
};

}
}
}
