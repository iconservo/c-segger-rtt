#ifndef _SEGGER_RTT_H_
#define _SEGGER_RTT_H_

#include <SEGGER_RTT_Conf.h>
#include <SEGGER_RTT.h>

class SeggerRTT : public FileHandle {
   public:
    SeggerRTT(unsigned rtt_buffer_index = 0) : _rtt_buffer_index(rtt_buffer_index), _is_atty(0) {
        /* Initialize SEGGER RTT using init function. */
        SEGGER_RTT_Init();
        SEGGER_RTT_Write(_rtt_buffer_index, "Try to start RTT.\r\n", 20);
        int diff = _SEGGER_RTT.aUp[_rtt_buffer_index].WrOff - _SEGGER_RTT.aUp[_rtt_buffer_index].RdOff;
        int n = 1000;
        while ((n > 0) && (diff > 0)) {
            n--;
            diff = _SEGGER_RTT.aUp[_rtt_buffer_index].WrOff - _SEGGER_RTT.aUp[_rtt_buffer_index].RdOff;
            wait_ms(1);
        }
        if (diff == 0) {
            // RTT interface exists
            _is_atty = 1;
            SEGGER_RTT_SetFlagsUpBuffer(_rtt_buffer_index, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
            SEGGER_RTT_Write(_rtt_buffer_index, "RTT is in working state!\r\n", 27);
        } else {
            _is_atty = -21;
            SEGGER_RTT_Write(_rtt_buffer_index, "RTT is not present!\r\n", 22);
        }
    }

    virtual ssize_t write(const void* buffer, size_t size) { return SEGGER_RTT_Write(_rtt_buffer_index, buffer, size); }

    virtual ssize_t read(void* buffer, size_t size) { return SEGGER_RTT_Read(_rtt_buffer_index, buffer, size); }

    virtual off_t seek(off_t offset, int whence = SEEK_SET) {
        /* Seeking is not support by this file handler */
        return -ESPIPE;
    }

    virtual off_t size() {
        /* Size is not defined for this file handle */
        return -EINVAL;
    }

    virtual int isatty() {
        /* File handle is used for terminal output */
        return _is_atty;
    }

    virtual int close() { return 0; }

   protected:
    unsigned _rtt_buffer_index;
    int _is_atty;
};

#endif  // _SEGGER_RTT_H_
