//
// Created by junmkim on 2020-11-19.
//

#ifndef MUTENET_SOCKETDESCRIPTORHELPER_HPP
#define MUTENET_SOCKETDESCRIPTORHELPER_HPP

namespace EventLoop
{
    class IDescriptor;
}

namespace MuteNet
{
    class SocketDescriptorHelper
    {
    public:
        static void InetPton(int af, const void *source, char* dest, size_t len);

        static int SetSocketNonblock(descriptor_t fd);

        static int SetListenSocketReuseable(descriptor_t listen);

        static int Connect(EventLoop::IDescriptor* DescriptorPtr,
                           const sockaddr* Ip,
                           socklen_t size);

        static std::string ErrorString(int64_t ErrorCode);

        static int64_t GetLastError();
    };
}


#endif //MUTENET_SOCKETDESCRIPTORHELPER_HPP
