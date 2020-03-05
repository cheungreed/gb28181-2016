/*
**	********************************************************************************
**
**	File		: xml_parser.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <tinyxml2/tinyxml2.h>
#include "defs/defs.h"

using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CXmlParser
{
public:
    CXmlParser() = default;
    ~CXmlParser() = default;

    int Load(const char* p_data, int p_size, tinyxml_doc_t &doc);

    int ParseXmlHeader(tinyxml_doc_t& doc, manscdp_msgbody_header_t& bodyheader);

    int ParseEleStr(tinyxml_ele_t *ele, const char* name, string& textstr, bool brequired, bool bto_utf8= false);

    int ParseEleInt(tinyxml_ele_t *ele, const char* name, int& value, bool brequired);

private:
    int parse_cmdcategory(tinyxml_ele_t *ele, manscdp_cmd_category_e& category, string& name);

    int parse_cmdtype(tinyxml_ele_t *ele, manscdp_msgbody_header_t &bh, string& textstr);

//    int ParseEleStr(tinyxml_ele_t *ele, const char* name, string& textstr, bool brequired, bool bto_utf8= false);
//
//    int parse_ele_int(tinyxml_ele_t *ele, const char* name, int& value, bool brequired);

    int parse_ele_double(tinyxml_ele_t *ele, const char* name, double& value, bool brequired);

    bool is_member(tinyxml_ele_t *ele, const char *name);
};

}
}
}
