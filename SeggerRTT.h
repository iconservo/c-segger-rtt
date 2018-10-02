#ifndef _SEGGER_RTT_H_
#define _SEGGER_RTT_H_

#include <SEGGER_RTT_Conf.h>
#include <SEGGER_RTT.h>

class SeggerRTT : public FileHandle {

public:

    SeggerRTT(unsigned rtt_buffer_index = 0) : _rtt_buffer_index(rtt_buffer_index)
    {
        /* Initialize SEGGER RTT using init function. */
        SEGGER_RTT_Init();
    }

    virtual ssize_t write(const void *buffer, size_t size)
    {
        return SEGGER_RTT_Write(_rtt_buffer_index, buffer, size);
    }

    virtual ssize_t read(void *buffer, size_t size)
    {
        return SEGGER_RTT_Read(_rtt_buffer_index, buffer, size);
    }

    virtual off_t seek(off_t offset, int whence = SEEK_SET)
    {
        /* Seeking is not support by this file handler */
        return -ESPIPE;
    }

    virtual off_t size()
    {
        /* Size is not defined for this file handle */
        return -EINVAL;
    }

    virtual int isatty()
    {
        /* File handle is used for terminal output */
        return true;
    }

    virtual int close()
    {
        return 0;
    }

protected: 
    unsigned _rtt_buffer_index;
};

#endif // _SEGGER_RTT_H_

