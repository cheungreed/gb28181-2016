/*
**	********************************************************************************
**
**	File		: defs.h
**	Description	: 
**	Modify		: 2020/3/4		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <memory>
#include <map>
#include <string>
using namespace std;

namespace Zilu {
namespace Protocol {

struct session_t
{
    int         cid;
    int         did;
    string      sessid;
    string      devid;

};
typedef shared_ptr<session_t> session_ptr;
typedef map<string, session_ptr> session_map;

}
}
