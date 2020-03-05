/*
**	********************************************************************************
**
**	File		: request_manager.cpp
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#include <Infra/ThreadPool.h>
#include <Infra/PrintLog.h>
#include "request_manager.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

extern const char* request_type_str[OUTCOMING_REQ_TYPE_MAX];

CRequestManager *CRequestManager::instance()
{
    static CRequestManager _ins;
    return &_ins;
}

int CRequestManager::Start()
{
    Infra::ThreadTask task = bind(&CRequestManager::check_requet_timeout, this);
    Infra::CThreadPool::instance()->run(task);
    return 0;
}

int CRequestManager::AddRequest(string &reqid, CBaseRequestSPtr &req)
{
    outcoming_req_type_e reqtype = req->GetReqType();
    base_request_map_t* p_reqmap = get_base_request_map(reqtype);
    if (p_reqmap == nullptr) {
        LOG_WARN << "[CRequestManager] AddRequest: " << request_type_str[reqtype] <<
        ", reqid: " << reqid << " failed, no this request type in center.";
        return -1;
    }

    lock_guard<mutex> guard(p_reqmap->m);
    if (p_reqmap->req_map.find(reqid) != p_reqmap->req_map.end()) {
        LOG_WARN << "[CRequestManager] AddRequest: " << request_type_str[reqtype] <<
                 ", reqid: " << reqid << " already added.";
        return -2;
    }
    req->SetReqid(reqid);
    p_reqmap->req_map[reqid] = req;

    LOG_WARN << "[CRequestManager] AddRequest: " << request_type_str[reqtype] <<
             ", reqid: " << reqid << " added success.";

    return 0;
}

int CRequestManager::DelRequest(outcoming_req_type_e reqtype, string &reqid)
{
    base_request_map_t* p_reqmap = get_base_request_map(reqtype);
    if (nullptr == p_reqmap) {
        return -1;
    }

    lock_guard<mutex> guard(p_reqmap->m);
    if (p_reqmap->req_map.find(reqid) == p_reqmap->req_map.end()) {
        LOG_WARN << "delete request: " << request_type_str[reqtype] << ", reqid: "<< reqid << " does not exist.";
        return -2;
    }

    p_reqmap->req_map.erase(reqid);
    LOG_WARN << "delete request: " << request_type_str[reqtype] << ", reqid: "<<reqid << " success.";
    return 0;
}

int CRequestManager::GetMsgRequestBySn(const string &reqsn, CMessageRequestSptr& msgreq)
{
    lock_guard<mutex> guard(m_messagemap.m);
    BaseRequestMap::const_iterator itr;
    for (; itr != m_messagemap.req_map.end(); ++itr) {
        msgreq = dynamic_pointer_cast<CMessageRequest>(itr->second);
        if (msgreq->GetReqSn() == reqsn) {
            return 0;
        }
    }
    return -1;
}

int CRequestManager::HandleMsgResponse(string &reqid, int status_code)
{
    return handle_response(m_messagemap, reqid, status_code);
}

int CRequestManager::check_requet_timeout()
{
    ///超时时间 秒
    int timeout = 6;
    base_request_map_t* p_reqmap[] = {&m_messagemap,};
    int p_size = sizeof(p_reqmap) / sizeof(base_request_map_t*);

    time_t now = time(nullptr);
    for (int i=0; i < p_size; ++i) {
        base_request_map_t* reqmap = p_reqmap[i];
        lock_guard<mutex> guard(reqmap->m);

        auto itr = reqmap->req_map.begin();
        for (; itr != reqmap->req_map.end();) {
            ///定时清除超时请求 6s
            if (now - itr->second->GetReqtime() > timeout) {
                itr->second->HandleResponse(-1);
                LOG_INFO << "check_request_timeout: " << request_type_str[itr->second->GetReqType()] << " timeout";
                itr = reqmap->req_map.erase(itr);
            }
            else {
                ++itr;
            }
        }
    }
    return 0;
}

base_request_map_t * CRequestManager::get_base_request_map(outcoming_req_type_e requtype)
{
    switch (requtype)
    {
        case OUTCOMING_REQU_TYPE_KEEPALIVE:
            return &m_messagemap;

        default:
            return nullptr;
    }
}

int CRequestManager::handle_response(base_request_map_t &reqmap, string &reqid, int status_code)
{
    if (reqid.length() <1) {
        return 0;
    }
    lock_guard<mutex> guard(reqmap.m);
    auto itr = reqmap.req_map.find(reqid);
    if (itr == reqmap.req_map.end()) {
        LOG_DEBUG << "handle_response: " << reqid <<" does not exist, it doesn't need to be handled!";
        return 0;
    }

    switch (itr->second->GetReqType())
    {
        //TODO
        case OUTCOMING_REQU_TYPE_KEEPALIVE:
            itr->second->HandleResponse(status_code);
            break;
        default:
            return -1;
    }

    if (itr->second->IsFinished()) {
        reqmap.req_map.erase(reqid);
        LOG_INFO << "handel_response: " << request_type_str[itr->second->GetReqType()] << ", reqid: "
                    << reqid << " finished, delete from request center.";
    }
    return 0;
}

}
}
}
