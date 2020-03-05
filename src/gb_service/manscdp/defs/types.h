/*
**	********************************************************************************
**
**	File		: types.h
**	Description	: 
**	Modify		: 2020/3/2		zhangqiang		Create the file
**	********************************************************************************
*/
#pragma once
namespace Zilu {
namespace Protocol {
namespace GB28181 {

enum deviceid_type_e
{

};

enum status_type_e
{
    STATUS_UNKNOWN = 0,
    ON,
    OFF,
};


enum result_type_e
{
    RESULT_UNKNOWN = 0,
    OK,
    ERROR
};

enum ptz_type_e
{

};

enum record_type_e
{
    RECORD_UNKNOWN = 0,
    RECORD,
    STOP_RECORD
};

enum guard_type_e
{
    GUARD_UNKNON =0,
    SET_GUARD,
    RESET_GUARD
};

enum item_type_e
{

};

enum item_file_type_e
{

};

}
}
}
