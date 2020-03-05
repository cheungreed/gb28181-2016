/*
**	********************************************************************************
**
**	File		: call_handler.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include "base_handler.h"
#include "./sdp/call_defs.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CCallHandler : public CBaseHandler
{
public:
    CCallHandler();
    virtual ~CCallHandler();

    int HandleIncomingCall(const sip_event_sptr &e);

private:
    int on_call_play_req(const sip_event_sptr &e, const sdp_description_t &req_sdp);
    int on_call_playback_req(const sip_event_sptr &e, const sdp_description_t &req_sdp);
    int on_call_download_req(const sip_event_sptr &e, const sdp_description_t &req_sdp);
    int on_call_talk_req(const sip_event_sptr &e, const sdp_description_t &req_sdp);
};

}
}
}