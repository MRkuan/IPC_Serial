/**

* @file       COBSTool.h

* @brief      COBS转译类.

* @details 对COBS转译进行封装.

* @author     Albert

*/
#ifndef COBSTOOL_H
#define COBSTOOL_H



class COBSTool
{
public:
    COBSTool();
    ~COBSTool();

    void StuffData(const unsigned char *ptr, unsigned long length,unsigned char *dst);
    void UnStuffData(const unsigned char *ptr, unsigned long length,unsigned char *dst);

};

#endif // COBSTOOL_H
