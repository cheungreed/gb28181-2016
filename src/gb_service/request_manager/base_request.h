/*
**	********************************************************************************
**
**	File		: base_request.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <memory>
#include <map>
#include <condition_variable>
#include "defs.h"
using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CBaseRequest
{
public:
    explicit CBaseRequest(outcoming_req_type_e reqtype);
    virtual ~CBaseRequest();

    virtual int HandleResponse(int statcode);

    void SetWait(bool bwait);

    void WaitResult();

    bool IsFinished();

    int SetReqid(string& id);

    outcoming_req_type_e GetReqType();

    time_t GetReqtime();

protected:
    int onRequestFinished();

private:
    void finished();

protected:
    string              m_reqid;

private:
    bool                m_bfinished;
    bool                m_bwait;
    time_t              m_reqtime;
    outcoming_req_type_e      m_reqtype;
    mutex               m_mutex;
    condition_variable  m_cond;
};
typedef shared_ptr<CBaseRequest> CBaseRequestSPtr;
typedef map<string, CBaseRequestSPtr> BaseRequestMap;

struct base_request_map_t
{
    mutex m;
    BaseRequestMap req_map;
};

}
}
}
