/*
**	********************************************************************************
**
**	File		: xml_parser.cpp
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#include <utils/common.h>
#include "xml_parser.h"
#include "defs/defs.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {


int CXmlParser::Load(const char *p_data, int p_size, tinyxml_doc_t &doc)
{
    if (nullptr == p_data || p_size <= 0)
        return -1;
    doc.Parse(p_data, p_size);
    if (doc.Error()) {
        LOG_INFO << "Load xml failed, error: "<<doc.ErrorID();
        return -1;
    }
    tinyxml_ele_t *ele = doc.RootElement();
    if (nullptr == ele || ele->Value() == nullptr) {
        LOG_INFO << "Load xml failed, error: root element does not exist.";
        return -2;
    }
    return 0;
}

int CXmlParser::ParseXmlHeader(tinyxml_doc_t &doc, manscdp_msgbody_header_t &bodyheader)
{
    tinyxml_ele_t * ele = doc.RootElement();

    string category_name;
    int r = parse_cmdcategory(ele, bodyheader.cmd_category, category_name);
    if (r != 0)
        return r;

    string cmdtype_name;
    r = parse_cmdtype(ele, bodyheader, cmdtype_name);
    if (r !=0)
        return r;

    r = ParseEleStr(ele, "DeviceID", bodyheader.devid, true);
    if (r !=0)
        return r;

    r = ParseEleStr(ele, "SN", bodyheader.sn, true);
    if (r != 0)
        return r;

    LOG_DEBUG << "[CXmlParser] ParseXmlHeader, category: " << category_name
              << ", cmdtype: " << cmdtype_name
              << ", devid: "<< bodyheader.devid
              << ", sn: " << bodyheader.sn;
    return 0;
}

int CXmlParser::parse_cmdcategory(tinyxml_ele_t *ele, manscdp_cmd_category_e &category, string &name)
{
    const char* ele_text = ele->Value();
    if (nullptr == ele_text) {
        LOG_INFO << "[CXmlMsgParser] parse_cmdcategory failed, error: root element does not have value";
        return -1;
    }

    name = ele_text;
    if (name == "Control")
        category = MANSCDP_CMD_CATEGORY_CONTROL;
    else if (name == "Query")
        category = MANSCDP_CMD_CATEGORY_QUERY;
    else if (name == "Notify")
        category = MANSCDP_CMD_CATEGORY_NOTIFY;
    else if (name == "Response")
        category = MANSCDP_CMD_CATEGORY_RESPONSE;
    else {
        category = MANSCDP_CMD_CATEGORY_UNKNOWN;
        return -1;
    }
    return 0;
}

int CXmlParser::parse_cmdtype(tinyxml_ele_t *ele, manscdp_msgbody_header_t &bh, string &textstr)
{
    int r = ParseEleStr(ele, "CmdType", textstr, true);
    if (r != 0)
        return r;

    if (textstr == "DeviceControl") {
        bh.cmd_type = MANSCDP_CONTROL_CMD_DEVICE_CONTROL;
        is_member(ele, "PTZCmd") ? bh.devctl_subcmd.push_back(PTZCmd) : void ();
        is_member(ele, "TeleBoot") ? bh.devctl_subcmd.push_back(TeleBoot) : void ();
        is_member(ele, "RecordCmd") ? bh.devctl_subcmd.push_back(RecordCmd) : void ();
        is_member(ele, "GuardCmd") ? bh.devctl_subcmd.push_back(GuardCmd) : void ();
        is_member(ele, "AlarmCmd") ? bh.devctl_subcmd.push_back(AlarmCmd) : void ();
        is_member(ele, "IFameCmd") ? bh.devctl_subcmd.push_back(IFrameCmd) : void ();
        is_member(ele, "DragZoomIn") ? bh.devctl_subcmd.push_back(DragZoomIn) : void ();
        is_member(ele, "DragZoomOut") ? bh.devctl_subcmd.push_back(DragZoomOut) : void ();
        is_member(ele, "HomePosition") ? bh.devctl_subcmd.push_back(HomePosition) : void ();
    }
    else if (textstr == "DeviceConfig") {
        bh.cmd_type = MANSCDP_CONTROL_CMD_DEVICE_CONFIG;
        is_member(ele, "BasicParam") ? bh.devcfg_subcmd.push_back(BasicParam) : void();
        is_member(ele, "SVACEncodeConfig") ? bh.devcfg_subcmd.push_back(SVACEncodeConfig) : void();
        is_member(ele, "SVACDecodeConfig") ? bh.devcfg_subcmd.push_back(SVACDecodeConfig) : void();
    }
    else if (textstr == "DeviceStatus")
        bh.cmd_type = MANSCDP_QUERY_CMD_DEVICE_STATUS;
    else if (textstr == "Catalog")
        bh.cmd_type = MANSCDP_QUERY_CMD_CATALOG;
    else if (textstr == "DeviceInfo")
        bh.cmd_type = MANSCDP_QUERY_CMD_DEVICE_INFO;
    else if (textstr == "RecordInfo")
        bh.cmd_type = MANSCDP_QUERY_CMD_RECORD_INFO;
    else if (textstr == "Alarm" && bh.cmd_category == MANSCDP_CMD_CATEGORY_QUERY)
        bh.cmd_type = MANSCDP_QUERY_CMD_ALARM;
    else if (textstr == "ConfigDownload")
        bh.cmd_type = MANSCDP_QUERY_CMD_CONFIG_DOWNLOAD;
    else if (textstr == "PresetQuery")
        bh.cmd_type = MANSCDP_QUERY_CMD_PRESET_QUERY;
    else if (textstr == "MobilePosition")
        bh.cmd_type = MANSCDP_QUERY_CMD_MOBILE_POSITION;
    else if (textstr == "Keepalive")
        bh.cmd_type = MANSCDP_NOTIFY_CMD_KEEPALIVE;
    else if (textstr == "Alarm" && bh.cmd_category == MANSCDP_CMD_CATEGORY_NOTIFY)
        bh.cmd_type = MANSCDP_NOTIFY_CMD_ALARM;
    else if (textstr == "MediaStatus")
        bh.cmd_type = MANSCDP_NOTIFY_CMD_MEDIA_STATUS;
    else if (textstr == "Broadcast")
        bh.cmd_type = MANSCDP_NOTIFY_CMD_BROADCASE;
    else if (textstr == "MobilePosition")
        bh.cmd_type = MANSCDP_NOTIFY_CMD_MOBILE_POSITION;

    return 0;
}

int CXmlParser::ParseEleStr(tinyxml_ele_t *ele, const char *name, string &textstr, bool brequired, bool bto_utf8)
{
    tinyxml_ele_t* child_ele = ele->FirstChildElement(name);
    if (nullptr == child_ele) {
        if (brequired) {
            LOG_INFO << "ParseEleStr: "<< name << " does not exist.";
        }
        return brequired ? -1 : 0;
    }

    const char * ele_text = child_ele->GetText();
    if (nullptr == ele_text) {
        if (brequired) {
            LOG_INFO << "ParseEleStr: "<<name << "does not have value.";
        }
        return brequired ? -1 : 0;
    }
    if (bto_utf8) {
        textstr = character_gb28181_to_utf8((char*)ele_text, strlen(ele_text));
    }
    else {
        textstr = ele_text;
    }
    return 0;
}

int CXmlParser::ParseEleInt(tinyxml_ele_t *ele, const char *name, int &value, bool brequired)
{
    tinyxml_ele_t* child_ele = ele->FirstChildElement(name);
    if (nullptr == child_ele) {
        if (brequired) {
            LOG_INFO <<"parse_ele_int: "<<name<< " does not exist.";
        }
        return brequired ? -1 : 0;
    }
    int ele_value;
    if (child_ele->QueryIntText(&ele_value) != tinyxml2::XML_SUCCESS) {
        if (brequired) {
            LOG_INFO << "parse_ele_int: " <<name<< " has invalid value.";
        }
        return brequired ? -1 : 0;
    }
    value = ele_value;
    return 0;
}

int CXmlParser::parse_ele_double(tinyxml_ele_t *ele, const char *name, double &value, bool brequired)
{
    tinyxml_ele_t* child_ele = ele->FirstChildElement(name);
    if (nullptr == child_ele) {
        if (brequired) {
            LOG_INFO <<"parse_ele_int: "<<name<< " does not exist.";
        }
        return brequired ? -1 : 0;
    }
    double ele_value;
    if (child_ele->QueryDoubleText(&ele_value) != tinyxml2::XML_SUCCESS) {
        if (brequired) {
            LOG_INFO << "parse_ele_int: " <<name<< " has invalid value.";
        }
        return brequired ? -1 : 0;
    }
    value = ele_value;
    return 0;
}

bool CXmlParser::is_member(tinyxml_ele_t *ele, const char *name)
{
    ///TODO confirm correct implement
    tinyxml_ele_t* child_ele = ele->FirstChildElement(name);
    return child_ele != nullptr;
}

}
}
}