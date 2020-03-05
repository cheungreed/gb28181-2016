/*
**	********************************************************************************
**
**	File		: xml_assembly.h
**	Description	: 
**	Modify		: 2020/3/3		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
#include "defs/defs.h"
using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CXmlAssembly
{
public:
    CXmlAssembly();
    ~CXmlAssembly() = default;

public:
    ///< response of QUERY request
    int assemble_resp_device_status(string& body, const manscdp_device_status_dialog_t& d);
    int assemble_resp_query_catalog(string& body, const manscdp_cataloginfo_dialog_t &d);
    int assemble_resp_device_info(string& body, const manscdp_devinfo_dialog_t& d);
    int assemble_resp_record_info(string& body, const manscdp_recordinfo_dialog_t& d);
    int assemble_resp_query_alarm(string& body, const manscdp_alarm_query_dialog_t& d);
    int assemble_resp_config_download(string& body, const manscdp_config_download_dialog_t& d);
    int assemble_resp_preset_query(string& body, const manscdp_preset_query_dialog_t& d);
//    int assemble_resp_mobile_position();

private:
    tinyxml_ele_t *insert_child(tinyxml_ele_t *root, const char *key, const string &value);

    int tostring(string& s);

    template <typename T>
    int assemble_common_item(tinyxml_ele_t *root, const T& d);

private:
    tinyxml_doc_t       m_tinyxml_doc;
    
};

}
}
}
