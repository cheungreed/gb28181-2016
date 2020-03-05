/*
**	********************************************************************************
**
**	File		: message_handler.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include "gb_service/defs.h"
#include "base_handler.h"
#include "gb_service/manscdp/xml_parser.h"
#include "gb_service/manscdp/defs/defs.h"
#include "utils/ptz/ptz_parser.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CMessageHandler : public CBaseHandler
{
public:
    CMessageHandler();

    ~CMessageHandler() final = default;

    int HandleIncomingReq(const sip_event_sptr& e);

    typedef function<int(const sip_event_sptr&, tinyxml_doc_t&, manscdp_msgbody_header_t&)> msg_event_proc;

public:
    ///< handle response of outgoing request
    int HandleResponseSuccess(const sip_event_sptr& e);
    int HandleResponseFailure(const sip_event_sptr& e);

private:
    int handle_incoming_req_control(const sip_event_sptr& e, tinyxml_doc_t& doc,
                                            manscdp_msgbody_header_t& bh);

    int handle_incoming_req_query(const sip_event_sptr& e, tinyxml_doc_t& doc,
                                            manscdp_msgbody_header_t& bh);

    int handle_incoming_req_notify(const sip_event_sptr& e, tinyxml_doc_t& doc,
                                   manscdp_msgbody_header_t& bh);

    int handle_incoming_req_response(const sip_event_sptr& e, tinyxml_doc_t& doc,
                                            manscdp_msgbody_header_t& bh);

private:
    int on_devctrl_ptzcmd(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_teleboot(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_recordcmd(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_guardcmd(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_alarmcmd(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_iframecmd(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_dragzoomin(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_dragzoomout(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devctrl_homeposition(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);

private:
    int on_devcfg_basicparam(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devcfg_svacencodeconfig(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_devcfg_svacdeconfigconfig(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);

    int devctrl_subcmd(const sip_event_sptr &e, string &devid, manscdp_devicecontrol_subcmd_e subcmd,
                       manscdp_switch_status_e onoff);

private:
    int on_query_device_status(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_query_catalog(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_query_device_info(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_query_record_info(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_query_alarm(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_query_config_download(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_query_preset_query(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);
    int on_query_mobile_position(const sip_event_sptr& e, tinyxml_doc_t& doc, manscdp_msgbody_header_t& bh);

private:
    CXmlParser                                          m_xmlparser;
    CPtzParser                                          m_ptzparser;
    ///< control type
    map<manscdp_devicecontrol_subcmd_e, msg_event_proc> m_devctlproc;
    map<manscdp_deviceconfig_subcmd_e, msg_event_proc>  m_devcfgproc;

    ///< query type
    map<manscdp_cmdtype_e, msg_event_proc>              m_queryproc;
};

}
}
}