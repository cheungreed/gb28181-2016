/*
**	********************************************************************************
**
**	File		: safe_queue.h
**	Description	: 
**	Modify		: 2020/2/27		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <queue>
#include "Infra/Mutex.h"

namespace Zilu {
namespace Protocol {

template <typename T>
class CSafeQueue
{
public:
    CSafeQueue() = default;

    ~CSafeQueue() = default;

    void Push(const T& in)
    {
        Infra::CGuard g(m_mutex);
        m_queue.push(in);
    }

    bool Pop(T& out)
    {
        Infra::CGuard g(m_mutex);
        if (m_queue.empty())
        {
            return false;
        }
        out = m_queue.front();
        m_queue.pop();
        return true;
    }

private:
    std::queue<T> m_queue;
    Infra::CMutex m_mutex;
};

}
}
