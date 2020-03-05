/*
**	********************************************************************************
**
**	File		: message_handler.cpp
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#include <gb_service/request_manager/request_manager.h>
#include <Infra/PrintLog.h>
#include <gb_service/gb28181_svr_manager.h>
#include "message_handler.h"
#include "osipparser2/osip_const.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

#define MSGPROC_TEMPLATE(F) (bind(&CMessageHandler::F, this, placeholders::_1, placeholders::_2, placeholders::_3))

CMessageHandler::CMessageHandler()
{
    m_devctlproc.insert(make_pair(PTZCmd, MSGPROC_TEMPLATE(on_devctrl_ptzcmd)));
    m_devctlproc.insert(make_pair(TeleBoot, MSGPROC_TEMPLATE(on_devctrl_teleboot)));
    m_devctlproc.insert(make_pair(RecordCmd, MSGPROC_TEMPLATE(on_devctrl_recordcmd)));
    m_devctlproc.insert(make_pair(GuardCmd, MSGPROC_TEMPLATE(on_devctrl_guardcmd)));
    m_devctlproc.insert(make_pair(AlarmCmd, MSGPROC_TEMPLATE(on_devctrl_alarmcmd)));
    m_devctlproc.insert(make_pair(IFrameCmd, MSGPROC_TEMPLATE(on_devctrl_iframecmd)));
    m_devctlproc.insert(make_pair(DragZoomIn, MSGPROC_TEMPLATE(on_devctrl_dragzoomin)));
    m_devctlproc.insert(make_pair(DragZoomOut, MSGPROC_TEMPLATE(on_devctrl_dragzoomout)));
    m_devctlproc.insert(make_pair(HomePosition, MSGPROC_TEMPLATE(on_devctrl_homeposition)));

    m_devcfgproc.insert(make_pair(BasicParam, MSGPROC_TEMPLATE(on_devcfg_basicparam)));
    m_devcfgproc.insert(make_pair(SVACEncodeConfig, MSGPROC_TEMPLATE(on_devcfg_svacencodeconfig)));
    m_devcfgproc.insert(make_pair(SVACDecodeConfig, MSGPROC_TEMPLATE(on_devcfg_svacdeconfigconfig)));

    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_ALARM, MSGPROC_TEMPLATE(on_query_alarm)));
    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_CATALOG, MSGPROC_TEMPLATE(on_query_catalog)));
    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_DEVICE_INFO, MSGPROC_TEMPLATE(on_query_device_info)));
    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_RECORD_INFO, MSGPROC_TEMPLATE(on_query_record_info)));
    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_DEVICE_STATUS, MSGPROC_TEMPLATE(on_query_device_status)));
    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_CONFIG_DOWNLOAD, MSGPROC_TEMPLATE(on_query_config_download)));
    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_PRESET_QUERY, MSGPROC_TEMPLATE(on_query_preset_query)));
    m_queryproc.insert(make_pair(MANSCDP_QUERY_CMD_MOBILE_POSITION, MSGPROC_TEMPLATE(on_query_mobile_position)));
}

int CMessageHandler::HandleIncomingReq(const sip_event_sptr &e)
{
    const char* username = e->exevent->request->from->url->username;
    const char* host = e->exevent->request->from->url->host;

    osip_body_t* body = nullptr;
    osip_message_get_body(e->exevent->request, 0, &body);
    if (body == nullptr) {
        sendSimplyResp(username, e->excontext, e->exevent->tid, SIP_BAD_REQUEST);
        return -1;
    }
    int r;

    LOG_DEBUG << "incoming request body: " << body->body;

    tinyxml_doc_t doc;
    r = m_xmlparser.Load(body->body, body->length, doc);
    if (r !=0) {
        sendSimplyResp(username, e->excontext, e->exevent->tid, SIP_BAD_REQUEST);
    }

    manscdp_msgbody_header_t bodyheader;
    r = m_xmlparser.ParseXmlHeader(doc, bodyheader);
    if (r !=0) {
        sendSimplyResp(username, e->excontext, e->exevent->tid, SIP_BAD_REQUEST);
        return r;
    }

    LOG_DEBUG << "Revice message, category: " << bodyheader.cmd_category <<", cmd_type: "
            << bodyheader.cmd_type << " from "<<username << ", host: "<< host;

    switch (bodyheader.cmd_category)
    {
        case MANSCDP_CMD_CATEGORY_CONTROL:
            r = handle_incoming_req_control(e, doc, bodyheader);
            break;
        case MANSCDP_CMD_CATEGORY_QUERY:
            r = handle_incoming_req_query(e, doc, bodyheader);
            break;
        case MANSCDP_CMD_CATEGORY_NOTIFY:

            break;
        case MANSCDP_CMD_CATEGORY_RESPONSE:

            break;
        default:
            break;
    }
    return 0;
}

int CMessageHandler::HandleResponseSuccess(const sip_event_sptr &e)
{
    int statcode = getStatcodeFromResp(e->exevent->response);
    string reqid = getMsgIdFromReq(e->exevent->request);

    CRequestManager::instance()->HandleMsgResponse(reqid, statcode);
    return 0;
}

int CMessageHandler::HandleResponseFailure(const sip_event_sptr &e)
{
    HandleResponseSuccess(e);
    return 0;
}

int CMessageHandler::handle_incoming_req_control(const sip_event_sptr &e, tinyxml_doc_t &doc,
                                                 manscdp_msgbody_header_t &bh)
{
    if (bh.cmd_type == MANSCDP_CONTROL_CMD_DEVICE_CONTROL) {
        for (auto cmd : bh.devctl_subcmd) {
            m_devctlproc[cmd](e, doc, bh);
        }
    }
    else if (bh.cmd_type == MANSCDP_CONTROL_CMD_DEVICE_CONFIG) {
        for (auto cmd : bh.devcfg_subcmd) {
            m_devcfgproc[cmd](e, doc, bh);
        }
    }
    return 0;
}

int CMessageHandler::handle_incoming_req_query(const sip_event_sptr &e, tinyxml_doc_t &doc,
                                               manscdp_msgbody_header_t &bh)
{
    auto proc = m_queryproc.find(bh.cmd_type);
    if (proc == m_queryproc.end()) {
        LOG_WARN << "Not found proc, cmd_type: " << bh.cmd_type;
        sendSimplyResp(e->name, e->excontext, e->exevent->tid, SIP_BAD_REQUEST);
        return -1;
    }
    m_queryproc[bh.cmd_type](e, doc, bh);
    return 0;
}

int CMessageHandler::handle_incoming_req_notify(const sip_event_sptr &e, tinyxml_doc_t &doc,
                                                manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::handle_incoming_req_response(const sip_event_sptr &e, tinyxml_doc_t &doc,
                                                  manscdp_msgbody_header_t &bh)
{
    return 0;
}

///< DeviceControl REQUEST; Control action
int CMessageHandler::on_devctrl_ptzcmd(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    const char* username = e->exevent->request->from->url->username;
    control_cmd_t ctrlcmd;
    memset(&ctrlcmd, 0x0, sizeof(ctrlcmd));

    string cmdstr;
    int r = m_xmlparser.ParseEleStr(doc.RootElement(), "PTZCmd", cmdstr, true);
    if (r != 0) {
        LOG_WARN << "ParseEleStr PTZCMD failed, ret: " << r;
        sendSimplyResp(username, e->excontext, e->exevent->tid, SIP_BAD_REQUEST);
        return -1;
    }

    int statcode = SIP_OK;
    m_ptzparser.ParseControlCmd(ctrlcmd, cmdstr);
    r = CGB28181SvrManager::instance()->HandlePTZControl(ctrlcmd, bh.devid);
    if (r !=0) {
        statcode = SIP_BAD_REQUEST;
    }
    sendSimplyResp(username, e->excontext, e->exevent->tid, statcode);
    return 0;
}

int CMessageHandler::on_devctrl_teleboot(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    devctrl_subcmd(e, bh.devid, TeleBoot, SWITCH_ON);
    return 0;
}

int CMessageHandler::on_devctrl_recordcmd(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    string value;
    m_xmlparser.ParseEleStr(doc.RootElement(), "RecordCmd", value, true);

    manscdp_switch_status_e onoff;
    if (value == "Record")
        onoff = SWITCH_ON;
    else                        ///< StopRecord
        onoff = SWITCH_OFF;
    devctrl_subcmd(e, bh.devid, RecordCmd, onoff);
    return 0;
}

int CMessageHandler::on_devctrl_guardcmd(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    string value;
    m_xmlparser.ParseEleStr(doc.RootElement(), "GuardCmd", value, true);

    manscdp_switch_status_e  onoff;
    if (value == "SetGuard")
        onoff = SWITCH_ON;
    else                        ///< ResetGuard
        onoff = SWITCH_OFF;
    devctrl_subcmd(e, bh.devid, GuardCmd, onoff);
    return 0;
}

int CMessageHandler::on_devctrl_alarmcmd(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    devctrl_subcmd(e, bh.devid, AlarmCmd, SWITCH_ON);
    return 0;
}

int CMessageHandler::on_devctrl_iframecmd(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    devctrl_subcmd(e, bh.devid, IFrameCmd, SWITCH_ON);
    return 0;
}

int CMessageHandler::on_devctrl_dragzoomin(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_devctrl_dragzoomout(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_devctrl_homeposition(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

///< DeviceConfig QUERST; Control action
int CMessageHandler::on_devcfg_basicparam(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int
CMessageHandler::on_devcfg_svacencodeconfig(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int
CMessageHandler::on_devcfg_svacdeconfigconfig(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::devctrl_subcmd(const sip_event_sptr &e, string &devid, manscdp_devicecontrol_subcmd_e subcmd,
                                    manscdp_switch_status_e onoff)
{
    ///< TODO follow-up work£¬ carry response;
    const char* username = e->exevent->request->from->url->username;

    int statcode = OSIP_SUCCESS;
    int r = CGB28181SvrManager::instance()->HandleDeviceControl(subcmd, devid, SWITCH_ON);
    if (r != 0) {
        statcode = SIP_BAD_REQUEST;
    }
    sendSimplyResp(username, e->excontext, e->exevent->tid, statcode);
    return 0;
}

///< Query action
int CMessageHandler::on_query_device_status(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    manscdp_device_status_dialog_t dialog;
    m_xmlparser.ParseEleStr(doc.RootElement(), "SN", dialog.request.sn, true);
    m_xmlparser.ParseEleStr(doc.RootElement(), "DeviceID", dialog.request.devid, true);


    return 0;
}

int CMessageHandler::on_query_catalog(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_query_device_info(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_query_record_info(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_query_alarm(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_query_config_download(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_query_preset_query(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

int CMessageHandler::on_query_mobile_position(const sip_event_sptr &e, tinyxml_doc_t &doc, manscdp_msgbody_header_t &bh)
{
    return 0;
}

}
}
}