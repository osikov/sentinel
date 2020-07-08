#include "ProcNetTcpRecord.hpp"
#include <string.h>


ProcNetTcpRecord::ProcNetTcpRecord(const char *b)
{
    strncpy(buffer, b, 1024);
    const char *delim = " ";
    sl = strtok(buffer, " ");
    local_address = strtok(0, delim);
    rem_address = strtok(0, delim);
    st = strtok(0, delim);
    tx_queue = strtok(0, delim);
    rx_queue = strtok(0, delim);
    tr = strtok(0, delim);
    tm_when = strtok(0, delim);
    retrnsmt = strtok(0, delim);
    uid = strtok(0, delim);
    timeout = strtok(0, delim);
    inode = strtok(0, delim);
}
