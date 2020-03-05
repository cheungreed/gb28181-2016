/*
**	********************************************************************************
**
**	File		: manscdp_config.h
**	Description	: 
**	Modify		: 2020/3/2		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once

#include "common_defs.h"

namespace Zilu {
namespace Protocol {
namespace GB28181 {

struct manscdp_device_config_dialogs_t
{
    struct request_t : public common_item_t {
    public:
        request_t() {
            cmdtype = "DeviceConfig";
        }
        manscdp_basicparam_cfg_t        basicparam;
        ///TODO add SVACEncodeConfig so and on.
    };
    struct response_t : public common_answer_t {
    public:
        response_t() {
            cmdtype = "DeviceConfig";
        }
    };

public:
    request_t           request;
    response_t          response;
};

}
}
}
