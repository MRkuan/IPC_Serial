#ifndef COBSTOOL_H
#define COBSTOOL_H

#define FinishBlock(X) (*code_ptr = (X),code_ptr = dst++,code = 0x01 )

class COBSTool
{
public:
    COBSTool();
    ~COBSTool();

    void StuffData(const unsigned char *ptr, unsigned long length,unsigned char *dst);
    void UnStuffData(const unsigned char *ptr, unsigned long length,unsigned char *dst);

};

#endif // COBSTOOL_H
