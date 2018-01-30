#include "COBSTool.h"
#define FinishBlock(X) (*code_ptr = (X),code_ptr = dst++,code = 0x01 )
COBSTool::COBSTool()
{

}

COBSTool::~COBSTool()
{

}


/**
 * @brief StuffData 编码
 * StuffData byte stuffs “length” bytes of
 * data at the location pointed to by “ptr”,
 * writing the output to the location pointed
 * to by “dst”.
 * @param ptr
 * @param length
 * @param dst
 */
void COBSTool::StuffData(const unsigned char *ptr, unsigned long length,unsigned char *dst)
{
    const unsigned char *end = ptr + length;
    unsigned char *code_ptr = dst++;
    unsigned char code = 0x01;
    while (ptr < end)
    {
        if (*ptr == 0)
            FinishBlock(code);
        else
        {
            *dst++ = *ptr;
            code++;
            if (code == 0xFF)
                FinishBlock(code);
        }
        ptr++;
    }
    FinishBlock(code);
}

/**
 * @brief UnStuffData 解码
 * UnStuffData decodes “length” bytes of
 * data at the location pointed to by “ptr”,
 * writing the output to the location pointed
 * to by “dst”.
 * @param ptr
 * @param length
 * @param dst
 */
void COBSTool::UnStuffData(const unsigned char *ptr, unsigned long length,unsigned char *dst)
{
    const unsigned char *end = ptr + length;
    while (ptr < end)
    {
        int i, code = *ptr++;
        for (i = 1; i < code; i++)
            *dst++ = *ptr++;
        if (code < 0xFF)
            *dst++ = 0;
    }
}
