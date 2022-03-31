#include <optotunepp.hpp>

namespace opp
{
#if defined(__GNUC__) && __linux__

#elif defined(_MSC_VER) && _WIN32

#    define GET_LAST_ERRORMSG(buffer)                                          \
        do                                                                     \
        {                                                                      \
            DWORD dw = GetLastError();                                         \
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |                     \
                              FORMAT_MESSAGE_FROM_SYSTEM |                     \
                              FORMAT_MESSAGE_IGNORE_INSERTS,                   \
                          NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
                          buffer, 0, NULL);                                    \
        } while (0)

_SerialPort::_SerialPort(void) {}

_SerialPort::~_SerialPort()
{
    disconnect();
}

int
_SerialPort::connect(const char* port)
{
    handle = CreateFile(port,                          // port name
                        GENERIC_READ | GENERIC_WRITE,  // R/W access
                        0,  // (share) 0: cannot share COM port
                        0,  // security 0: none
                        OPEN_EXISTING, 0, 0);
    COMMTIMEOUTS timeout;
    // unit: ms
    // The maximum time allowed to elapse before the arrival of the next byte
    timeout.ReadIntervalTimeout = 10;
    /**
     * The multiplier used to calculate the total time-out period for read operations, in milliseconds.
     * For each read operation, this value is multiplied by the requested number of bytes to be read.
    */
    timeout.ReadTotalTimeoutMultiplier = 10;
    /**
     * A constant used to calculate the total time-out period for read operations, in milliseconds.
     * For each read operation, this value is added to the product of the ReadTotalTimeoutMultiplier member and 
     * the requested number of bytes.
    */
    timeout.ReadTotalTimeoutConstant    = 1000;
    timeout.WriteTotalTimeoutMultiplier = 10;
    timeout.WriteTotalTimeoutConstant   = 1000;

    if ((!SetCommTimeouts(handle, &timeout)) || (!GetCommState(handle, &dcb)))
        return OPTOTUNEPP_FAILED;

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    return OPTOTUNEPP_OK;
}

void
_SerialPort::disconnect(void)
{
    CloseHandle(handle);
}

int
_SerialPort::send(const char* data, size_t size)
{
    if (size == 0)
        return OPTOTUNEPP_OK;

    DWORD b_sent{0};
    if (!WriteFile(handle, data, (DWORD)size, &b_sent, NULL))
    {
        GET_LAST_ERRORMSG(m_err);

        if (printTerm)
        {
            std::cerr << m_err << "\n";
        }

        return OPTOTUNEPP_FAILED;
    }

    return OPTOTUNEPP_OK;
}

int
_SerialPort::recv(size_t size)
{
    DWORD b_recv{0};
    if (!ReadFile(handle, m_buffer, (DWORD)size, &b_recv, NULL))
    {
        GET_LAST_ERRORMSG(m_err);

        if (printTerm)
        {
            std::cerr << m_err << "\n";
        }

        return OPTOTUNEPP_FAILED;
    }

    return OPTOTUNEPP_OK;
}

const char*
_SerialPort::buffer(void) const noexcept
{
    return m_buffer;
}

const char*
_SerialPort::lastError(void) const noexcept
{
    return m_err;
}

void
_SerialPort::setPrintTerm(bool enable) noexcept
{
    printTerm = enable;
}

#endif

}  // namespace opp
