/*
**	********************************************************************************
**
**	File		: register_handler.h
**	Description	: 
**	Modify		: 2020/2/28		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include "base_handler.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

class CRegisterHandler : public CBaseHandler
{
public:
    CRegisterHandler();
    virtual ~CRegisterHandler();

};

}
}
}