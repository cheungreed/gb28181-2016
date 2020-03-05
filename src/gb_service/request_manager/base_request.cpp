/*
**	********************************************************************************
**
**	File		: base_request.cpp
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#include "base_request.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

const char *request_type_str[OUTCOMING_REQ_TYPE_MAX] = {
        "unknown",
        "keepalive"
};

CBaseRequest::CBaseRequest(outcoming_req_type_e reqtype)
    : m_bfinished(false), m_bwait(false), m_reqtime(time(nullptr)), m_reqtype(reqtype)
{
}

CBaseRequest::~CBaseRequest()= default;

int CBaseRequest::HandleResponse(int statcode)
{
    return 0;
}

void CBaseRequest::SetWait(bool bwait)
{
    m_bwait = bwait;
}

void CBaseRequest::WaitResult()
{
    unique_lock<mutex> lock(m_mutex);
    m_cond.wait(lock);
}

bool CBaseRequest::IsFinished()
{
    return m_bfinished;
}

int CBaseRequest::onRequestFinished()
{
    m_bfinished = true;
    if (m_bwait) {
        finished();
    }
    return 0;
}

void CBaseRequest::finished()
{
    if (!m_bwait)
        return;

    unique_lock<mutex> lock(m_mutex);
    lock.unlock();
    m_cond.notify_one();
    m_bwait = false;
}

int CBaseRequest::SetReqid(string &id)
{
    m_reqid = id;
    return 0;
}

outcoming_req_type_e CBaseRequest::GetReqType()
{
    return m_reqtype;
}

time_t CBaseRequest::GetReqtime()
{
    return m_reqtime;
}

}
}
}
