/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Socket.cpp
*/

#include "Wrapper/Socket.hpp"

#include <cstring>
#include <string>
#include <cerrno>

bool Wrapper::Socket::Initialize()
{
#ifdef _WIN32
    WSADATA data = {0};
    std::int32_t result = WSAStartup(MAKEWORD(2, 2), &data);

    if (result != 0) {
        return false;
    }
    return true;
#else
    return true;
#endif
}

void Wrapper::Socket::Cleanup()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

bool Wrapper::Socket::IsValid(const SocketType& sock)
{
#ifdef _WIN32
    return sock != INVALID_SOCKET;
#else
    return sock >= 0;
#endif
}

Wrapper::Socket::SocketType Wrapper::Socket::Create(const Protocol& protocol)
{
#ifdef _WIN32
    SocketType sock = INVALID_SOCKET;
#else
    SocketType sock = -1;
#endif

    if (protocol != Protocol::UDP) {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    } else {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    return sock;
}

bool Wrapper::Socket::Close(const SocketType& sock)
{
#ifdef _WIN32
    if (IsValid(sock)) {
        std::int32_t result = closesocket(sock);

        if (result != 0) {
            return false;
        }
        return true;
    }
    return false;
#else
    if (IsValid(sock)) {
        std::int32_t result = close(sock);

        if (result != 0) {
            return false;
        }
        return true;
    }
    return false;
#endif
}

bool Wrapper::Socket::Connect(const SocketType& sock, const std::string& address, std::uint16_t port)
{
    struct sockaddr_in addr = {};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

#ifdef _WIN32
    if (IsValid(sock)) {
        if (InetPtonA(AF_INET, address.c_str(), &addr.sin_addr) == 1) {
            std::int32_t result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
            return (result != SOCKET_ERROR);
        }
        return false;
    }
    return false;
#else
    if (IsValid(sock)) {
        if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
            return false;
        }

        std::int32_t result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        return result >= 0;
    }
    return false;
#endif
}

bool Wrapper::Socket::Bind(const SocketType& sock, std::uint16_t port, const std::string& address)
{
    struct sockaddr_in addr = {};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (address.empty()) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
#ifdef _WIN32
        if (InetPtonA(AF_INET, address.c_str(), &addr.sin_addr) != 1) {
            return false;
        }
#else
        if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
            return false;
        }
#endif
    }

#ifdef _WIN32
    if (IsValid(sock)) {
        std::int32_t result = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
        return result != SOCKET_ERROR;
    }
    return false;
#else
    if (IsValid(sock)) {
        std::int32_t result = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
        return result >= 0;
    }
    return false;

#endif
}

bool Wrapper::Socket::Listen(const SocketType& sock, std::int32_t backlog)
{
#ifdef _WIN32
    if (IsValid(sock)) {
        std::int32_t result = listen(sock, backlog);
        return result != SOCKET_ERROR;
    }
    return false;
#else
    if (IsValid(sock)) {
        std::int32_t result = listen(sock, backlog);
        return result >= 0;
    }
    return false;
#endif
}

Wrapper::Socket::SocketType Wrapper::Socket::Accept(const SocketType& sock, std::optional<std::reference_wrapper<std::pair<std::string, std::uint16_t>>> data)
{
    struct sockaddr_in addr = {};
    socklen_t len = sizeof(addr);

#ifdef _WIN32
    if (IsValid(sock)) {
        SocketType client = accept(sock, (struct sockaddr*)&addr, &len);

        if (IsValid(client) && data) {
            char str[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, &addr.sin_addr, str, INET_ADDRSTRLEN);

            std::pair<std::string, std::uint16_t>& ref = data->get();
            ref.first = str;
            ref.second = ntohs(addr.sin_port);
        }
        return client;
    }
    return INVALID_SOCKET;
#else
    if (IsValid(sock)) {
        SocketType client = accept(sock, (struct sockaddr*)&addr, &len);

        if (IsValid(client) && data) {
            char str[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, &addr.sin_addr, str, INET_ADDRSTRLEN);

            std::pair<std::string, std::uint16_t>& ref = data->get();
            ref.first = str;
            ref.second = ntohs(addr.sin_port);
        }
        return client;
    }
    return -1;
#endif
}

bool Wrapper::Socket::SetReuse(const SocketType& sock, bool reuse)
{
    std::int32_t opt = 0;

    if (reuse) {
        opt = 1;
    }
#ifdef _WIN32
    if (IsValid(sock)) {
        std::int32_t result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), static_cast<int>(sizeof(opt)));
        return result != SOCKET_ERROR;
    }
    return false;
#else
    if (IsValid(sock)) {
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            return false;
        }
#ifdef SO_REUSEPORT
        setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
#endif
        return true;
    }
    return false;
#endif
}

Wrapper::Socket::TransmitType Wrapper::Socket::Send(const SocketType& sock, const std::vector<std::uint8_t>& raw)
{
    if (raw.empty()) {
        return 0;
    }

#ifdef _WIN32
    if (IsValid(sock)) {
        return send(sock, reinterpret_cast<const char*>(raw.data()), static_cast<std::int32_t>(raw.size()), 0);
    }
    return -1;
#else
    if (IsValid(sock)) {
        return ::send(sock, raw.data(), raw.size(), 0);
    }
    return -1;
#endif
}

std::vector<std::uint8_t> Wrapper::Socket::Receive(const SocketType& sock, std::size_t size)
{
    if (size > 0 && size <= MAX_TCP_MESSAGE_SIZE) {
        std::vector<std::uint8_t> raw(size);
        TransmitType bytes = -1;

#ifdef _WIN32
        bytes = recv(sock, reinterpret_cast<char*>(raw.data()), static_cast<std::int32_t>(size), 0);
#else
        bytes = ::recv(sock, raw.data(), size, 0);
#endif

        if (bytes <= 0) {
            return {};
        }

        raw.resize(bytes);
        return raw;
    }
    return {};
}

std::int32_t Wrapper::Socket::Poll(std::vector<PollType>& fds, std::int32_t timeout)
{
    if (fds.empty()) {
        return 0;
    }

#ifdef _WIN32
    return WSAPoll(fds.data(), static_cast<ULONG>(fds.size()), timeout);
#else
    return ::poll(fds.data(), fds.size(), timeout);
#endif
}

std::vector<std::uint8_t> Wrapper::Socket::ReceiveFrom(const SocketType& sock, std::string& address, std::uint16_t& port)
{
    std::vector<std::uint8_t> buffer(UDP_PACKET_SIZE);
    struct sockaddr_in addr = {};
    socklen_t addrlen = sizeof(addr);

#ifdef _WIN32
    int32_t bytes = recvfrom(sock, reinterpret_cast<char*>(buffer.data()), static_cast<std::int32_t>(buffer.size()), 0, (struct sockaddr*)&addr, &addrlen);
#else
    ssize_t bytes = recvfrom(sock, buffer.data(), buffer.size(), 0, (struct sockaddr*)&addr, &addrlen);
#endif

    if (bytes <= 0) {
        return {};
    }
    char ip[INET_ADDRSTRLEN] = {0};

    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);

    address = ip;
    port = ntohs(addr.sin_port);

    buffer.resize(bytes);
    return buffer;
}

Wrapper::Socket::TransmitType Wrapper::Socket::SendTo(const SocketType& sock, const std::vector<std::uint8_t>& data, const std::string& address, std::uint16_t port)
{
    if (data.empty()) {
        return -1;
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

#ifdef _WIN32
    if (InetPtonA(AF_INET, address.c_str(), &addr.sin_addr) != 1) {
        return -1;
    }
    return sendto(sock, reinterpret_cast<const char*>(data.data()), static_cast<std::int32_t>(data.size()), 0, (struct sockaddr*)&addr, sizeof(addr));
#else
    if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
        return -1;
    }
    return sendto(sock, data.data(), data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
#endif
}

std::string Wrapper::Socket::GetLastError()
{
#ifdef _WIN32
    std::int32_t code = WSAGetLastError();
    char* msg = nullptr;

    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, 0, (LPSTR)&msg, 0, nullptr);

    if (msg != nullptr) {
        std::string err(msg);

        LocalFree(msg);
        return err;
    } else {
        return "Unknown error";
    }
#else
    return std::string(std::strerror(errno));
#endif
}
