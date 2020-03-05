/*
**	********************************************************************************
**
**	File		: common.cpp
**	Description	: 
**	Modify		: 2020/3/1		zhangqiang		Create the file
**	********************************************************************************
*/
#include "common.h"
#include <iconv.h>
#include <cstring>

namespace Zilu {
namespace Protocol {

string character_gb28181_to_utf8(char *src, int srclen)
{
    iconv_t icv;

    icv = iconv_open("UTF-8", "GB2312");
    if(icv == 0)
        return std::string("");

    const char *in = src;
    size_t inlen = srclen;
    size_t outlen = inlen * 4;

    char *outbuf = (char *)malloc(outlen);
    memset(outbuf, 0, outlen);
    char *outbuf_tmp = outbuf;

    iconv(icv, &src, (size_t *)&inlen, &outbuf_tmp, &outlen);

    std::string s = outbuf;

    free(outbuf);
    iconv_close(icv);

    return s;
}

}
}

