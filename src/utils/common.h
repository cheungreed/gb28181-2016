/*
**	********************************************************************************
**
**	File		: common.h
**	Description	: 
**	Modify		: 2020/3/1		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include <string>
using namespace std;

namespace Zilu {
namespace Protocol {

extern string character_gb28181_to_utf8(char *src, int srclen);

}
}