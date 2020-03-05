/*
**	********************************************************************************
**
**	File		: session_manager.cpp
**	Description	: 
**	Modify		: 2020/3/4		zhangqiang		Create the file
**	********************************************************************************
*/
#include "session_manager.h"

namespace Zilu {
namespace Protocol {

CSessionManager::CSessionManager()
{

}

CSessionManager::~CSessionManager()
{

}

CSessionManager *CSessionManager::instance()
{
    static CSessionManager _inst;
    return &_inst;
}

int CSessionManager::CreateSession(const string &sess_id, session_ptr &s)
{
    s = make_shared<session_t>();
    s->sessid = sess_id;

    m_sessmap.insert(make_pair(sess_id, s));
    return 0;
}

}
}

