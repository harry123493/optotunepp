#ifndef OPTOTUNEPP_HPP
#define OPTOTUNEPP_HPP

#include <iostream>
#include <stdlib.h>
#include <string>

#if defined(__GNUC__) && __linux__
#    include <fcntl.h>
#    include <termios.h>
#    include <unistd.h>
#elif defined(_MSC_VER) && _WIN32
#    include <windows.h>
#    include <strsafe.h>
#else
#    error "Unsupported platform"
#endif

#define OPTOTUNEPP_OK 0
#define OPTOTUNEPP_FAILED -1

namespace opp
{
class _SerialPort
{
#if defined(__GNUC__) && __linux__
  private:
    int     m_fd{-1};
    termios m_tty;

#elif defined(_MSC_VER) && _WIN32
  private:
    HANDLE handle;
    DCB    dcb;
#endif

  private:
    char m_buffer[128];
    char m_err[128];
    bool printTerm{false};

  public:
    _SerialPort(void);
    ~_SerialPort();
    _SerialPort(const _SerialPort& other) = delete;
    _SerialPort(_SerialPort&& other)      = delete;
    _SerialPort
    operator=(const _SerialPort& other) = delete;
    _SerialPort&
    operator=(_SerialPort&& other) = delete;
    /**
     * @brief Connect to serial port
     * 
     * @param port portname, e.g. "\\\\.\\COM3" for windows, "/dev/ttyACM0" for linux
     * @return int Return 0 if successful, return -1 if failed
     */
    int
    connect(const char* port);
    void
    disconnect(void);
    int
    send(const char* data, size_t size);
    int
    recv(size_t size);
    const char*
    buffer(void) const noexcept;
    const char*
    lastError(void) const noexcept;
    void
    setPrintTerm(bool enable) noexcept;
};

}  // namespace opp

#endif  // OPTOTUNEPP_HPP