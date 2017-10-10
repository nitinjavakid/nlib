#ifndef N_BUFFER_H
#define N_BUFFER_H

class Buffer
{
public:
    Buffer(size_t bufsize)
        : size(bufsize)
    {
        buffer = (int *) malloc(size * sizeof(int));
        ridx = 0;
        widx = 0;
        datacount = 0;
    }

    void putch(int byte)
    {
        if(datacount == size)
            return;

        buffer[widx] = byte;
        widx = (widx + 1) % size;
        ++datacount;
    }

    size_t available()
    {
        return datacount;
    }

    int getch()
    {
        char retval = buffer[ridx];
        ridx = (ridx + 1) % size;
        --datacount;
        return retval;
    }

    ~Buffer()
    {
        free(buffer);
    }

private:
    int   *buffer;
    volatile size_t size;
    volatile size_t widx;
    volatile size_t ridx;
    volatile size_t datacount;
};

#endif
