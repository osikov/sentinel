
class ProcNetTcpRecord
{
public:
    const char *sl;
    const char *local_address;
    const char *rem_address;
    const char *st;
    const char *tx_queue;
    const char *rx_queue;
    const char *tr;
    const char *tm_when;
    const char *retrnsmt;
    const char *uid;
    const char *timeout;
    const char *inode;

    ProcNetTcpRecord(const char*b);
private:
    char buffer[1024];
};
