#ifndef N_IO_IMPL_H
#define N_IO_IMPL_H

class IOImpl
{
public:
    virtual char read() = 0;
    virtual void write(char) = 0;
    virtual int  read(void *buffer, size_t size) = 0;
    virtual int  write(const void *buffer, size_t size) = 0;
    virtual int  close() = 0;
    virtual ~IOImpl()
    {
    }
};

#endif
