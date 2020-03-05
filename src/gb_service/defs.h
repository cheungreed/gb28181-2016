/*
**	********************************************************************************
**
**	File		: defs.h
**	Description	: 
**	Modify		: 2020/2/27		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <memory>
#include "eXosip2/eXosip.h"
using namespace std;

namespace Zilu {
namespace Protocol {
namespace GB28181 {

struct sip_event_t;
typedef shared_ptr <sip_event_t> sip_event_sptr;
typedef function<int(const sip_event_sptr &)> event_proc;

struct sip_event_t
{
    int                 value;
    const char*         name;
    event_proc          proc;
    struct eXosip_t*    excontext;
    eXosip_event_t*     exevent;
    uint64_t            id;

    sip_event_t() = default;
};

}
}
}
