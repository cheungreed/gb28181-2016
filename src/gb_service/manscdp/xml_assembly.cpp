/*
**	********************************************************************************
**
**	File		: xml_assembly.cpp
**	Description	: 
**	Modify		: 2020/3/3		zhangqiang		Create the file
**	********************************************************************************
*/
#include <Infra/PrintLog.h>
#include "xml_assembly.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

const char* result_type_str[] = {"UNKNOWN", "OK", "ERROR"};
const char* status_type_str[] = {"UNKNOWN", "ON", "OFF"};

CXmlAssembly::CXmlAssembly()
{
    tinyxml2::XMLDeclaration* dec = m_tinyxml_doc.NewDeclaration("xml version=\"1.0\"");
    m_tinyxml_doc.InsertFirstChild(dec);
}

int CXmlAssembly::assemble_resp_device_status(string &body, const manscdp_device_status_dialog_t &d)
{
    tinyxml_ele_t* ele_resp = m_tinyxml_doc.NewElement("Response");
    m_tinyxml_doc.InsertEndChild(ele_resp);

//    insert_child(ele_resp, "CmdType", d.response.cmdtype);
//    insert_child(ele_resp, "SN", d.request.sn);
//    insert_child(ele_resp, "DeviceID", d.request.devid);
    assemble_common_item(ele_resp, d);
    insert_child(ele_resp, "Result", result_type_str[d.response.result]);
    insert_child(ele_resp, "Online", d.response.onoff_line);
    insert_child(ele_resp, "Status", result_type_str[d.response.work_status]);
    insert_child(ele_resp, "Record", status_type_str[d.response.record]);

    tostring(body);

    LOG_DEBUG << "device status: " << body;

    return 0;
}

int CXmlAssembly::assemble_resp_query_catalog(string &body, const manscdp_cataloginfo_dialog_t &d)
{
    tinyxml_ele_t* ele_resp = m_tinyxml_doc.NewElement("Response");
    m_tinyxml_doc.InsertEndChild(ele_resp);

    assemble_common_item(ele_resp, d);
    insert_child(ele_resp, "SumNum", to_string(d.response.sum));
    tinyxml_ele_t* devlist = insert_child(ele_resp, "DeviceList", nullptr);
    devlist->SetAttribute("Num", int(d.response.devlist.size()));
    for (auto ii : d.response.devlist)
    {
        tinyxml_ele_t* item = insert_child(devlist, "Item", nullptr);
        insert_child(item, "DeviceID", ii.devid);
        insert_child(item, "Name", ii.name);
        insert_child(item, "Manufacturer", ii.manufacturer);
        insert_child(item, "Model", ii.model);
        insert_child(item, "Owner", ii.owner);
        insert_child(item, "CivilCode", ii.civilcode);
        insert_child(item, "Address", ii.address);
        insert_child(item, "Parental", to_string(ii.parental));
        insert_child(item, "ParentID", ii.parentid);
        insert_child(item, "RegisterWay", to_string(ii.register_way));
        insert_child(item, "Secrecy", to_string(ii.secrecty));
        insert_child(item, "Status", status_type_str[ii.status]);
        ///< TODO Info item;
    }

    tostring(body);
    LOG_DEBUG << "catalog: " << body;
    return 0;
}

int CXmlAssembly::assemble_resp_device_info(string &body, const manscdp_devinfo_dialog_t &d)
{
    tinyxml_ele_t* eleresp = m_tinyxml_doc.NewElement("Response");
    m_tinyxml_doc.InsertEndChild(eleresp);

    assemble_common_item(eleresp, d);
    insert_child(eleresp, "DeviceName", d.response.devname);
    insert_child(eleresp, "Result", result_type_str[d.response.result]);
    insert_child(eleresp, "Manufacturer", d.response.manufacturer);
    insert_child(eleresp, "Model", d.response.model);
    insert_child(eleresp, "Firmware", d.response.firmware);
    insert_child(eleresp, "Channel", to_string(d.response.channel));

    tostring(body);
    LOG_DEBUG << "devinfo: "<< body;
    return 0;
}

int CXmlAssembly::assemble_resp_record_info(string &body, const manscdp_recordinfo_dialog_t &d)
{
    tinyxml_ele_t* eleresp = m_tinyxml_doc.NewElement("Response");
    m_tinyxml_doc.InsertEndChild(eleresp);

    assemble_common_item(eleresp, d);
    insert_child(eleresp, "Name", d.response.name);
    insert_child(eleresp, "SumNum", to_string(d.response.sum));
    tinyxml_ele_t* recordlist = insert_child(eleresp, "RecordList", nullptr);
    recordlist->SetAttribute("Num", (int)d.response.recordlist.size());
    for (auto i : d.response.recordlist)
    {
        tinyxml_ele_t* item = insert_child(recordlist, "Item", nullptr);
        insert_child(item, "DeviceID", i.devid);
        insert_child(item, "Name", i.devname);
        insert_child(item, "FilePath", i.filepath);
        insert_child(item, "Address", i.address);
        insert_child(item, "StartTime", i.starttime);
        insert_child(item, "EndTime", i.endtime);
        insert_child(item, "Secrecy", to_string(i.secrecy));
        insert_child(item, "Type", i.type);
        insert_child(item, "RecorderID", i.recordid);
        insert_child(item, "FileSize", to_string(i.filesize));
    }

    tostring(body);
    LOG_DEBUG << "recordInfo: " << body;
    return 0;
}

int CXmlAssembly::assemble_resp_query_alarm(string &body, const manscdp_alarm_query_dialog_t &d)
{
    return 0;
}

int CXmlAssembly::assemble_resp_config_download(string &body, const manscdp_config_download_dialog_t &d)
{
    tinyxml_ele_t* eleresp = m_tinyxml_doc.NewElement("Response");
    m_tinyxml_doc.InsertEndChild(eleresp);

    assemble_common_item(eleresp, d);
    insert_child(eleresp, "Result", result_type_str[d.response.result]);

    tinyxml_ele_t* basic = insert_child(eleresp, "BasicParam", nullptr);
    insert_child(basic, "Name", d.response.basic.basic_cfg.devname);
    insert_child(basic, "Expiration", to_string(d.response.basic.basic_cfg.expire));
    insert_child(basic, "HeartBeatInterval", to_string(d.response.basic.basic_cfg.heartbeat_interval));
    insert_child(basic, "HeartBeatCount", to_string(d.response.basic.basic_cfg.heartbeat_cnt));
    insert_child(basic, "PositionCapability", to_string(d.response.basic.position_capability));
    insert_child(basic, "Longitude", to_string(d.response.basic.longitude));
    insert_child(basic, "Latitude", to_string(d.response.basic.latitude));

    tostring(body);
    LOG_DEBUG << "config download: " << body;
    return 0;
}

int CXmlAssembly::assemble_resp_preset_query(string &body, const manscdp_preset_query_dialog_t &d)
{
    tinyxml_ele_t* eleresp = m_tinyxml_doc.NewElement("Response");
    m_tinyxml_doc.InsertEndChild(eleresp);

    assemble_common_item(eleresp, d);
    tinyxml_ele_t* presetlist = insert_child(eleresp, "PresetList", nullptr);
    presetlist->SetAttribute("Num", (int)d.response.preset_list.size());
    for (auto i : d.response.preset_list)
    {
        tinyxml_ele_t* item = insert_child(presetlist, "Item", nullptr);
        insert_child(item, "PresetID", i.preset_id);
        insert_child(item, "PresetName", i.preset_name);
    }
    tostring(body);
    LOG_DEBUG << "preset query: " << body;
    return 0;
}

tinyxml_ele_t *CXmlAssembly::insert_child(tinyxml_ele_t *root, const char *key, const string &value)
{
    if (root == nullptr || key == nullptr)
        return nullptr;

    tinyxml_ele_t* child_ele = m_tinyxml_doc.NewElement(key);
    if (!value.empty()) {
        child_ele->SetText(value.c_str());
    }
    root->InsertEndChild(child_ele);

    return child_ele;
}

int CXmlAssembly::tostring(string &s)
{
    tinyxml2::XMLPrinter p;
    m_tinyxml_doc.Print(&p);
    s = p.CStr();
    return 0;
}

template <typename T>
int CXmlAssembly::assemble_common_item(tinyxml_ele_t *root, const T& d)
{
    insert_child(root, "CmdType", d.response.cmdtype);
    insert_child(root, "SN", d.request.sn);
    insert_child(root, "DeviceID", d.request.devid);
}

}
}
}
