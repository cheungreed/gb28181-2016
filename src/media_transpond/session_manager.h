/*
**	********************************************************************************
**
**	File		: session_manager.h
**	Description	: 
**	Modify		: 2020/3/4		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include "defs.h"

namespace Zilu {
namespace Protocol {

class CSessionManager
{
public:
    CSessionManager();
    ~CSessionManager();

    static CSessionManager* instance();

    int CreateSession(const string &sess_id, session_ptr &s);

private:
    session_map     m_sessmap;

};

}
}
