//
// Created by Mute on 2020-11-18.
//

#ifndef MUTENET_TCPLINK_HPP
#define MUTENET_TCPLINK_HPP

namespace MuteNet
{
    class TCPLink
    {
        friend class Network;
    public:
        class Callbacks
        {
        public:
            virtual ~Callbacks() {}

            virtual void OnCreated(TCPLink* Link) = 0;

            virtual void OnReceivedData(const char* data, size_t Length) = 0;

            virtual void OnRemoteClosed(void) = 0;

            virtual void OnTLSHandShakeCompleted() = 0;

            virtual void OnError(int ErrorCode, const std::string& ErrorMsg) = 0;
        };

        typedef std::shared_ptr<Callbacks> CallbacksPtr;
    protected:
        CallbacksPtr _Callback;

        TCPLink(CallbacksPtr callbacks)
        :_Callback(callbacks)
                {
                }

    public:
        virtual ~TCPLink()
        {
        }

        virtual bool Send(const void* Data, size_t Length) = 0;

        virtual std::string GetLocalIP(void) const = 0;

        virtual uint16_t GetLocalPort() const = 0;

        virtual std::string GetRemoteIP() const = 0;

        virtual uint16_t GetRemotePort() const = 0;

        virtual void Shutdown() = 0;
        virtual void Close() = 0;
    };
}

#endif //MUTENET_TCPLINK_HPP
