#pragma once

namespace MuteNet
{
    class Acceptor;

    typedef std::shared_ptr <Acceptor> AcceptorPtr;

    class Acceptor
    {
        friend class ServiceListener;

        friend class ASyncAcceptRequest;

    private:
        ServiceListener *_Port;
        SOCKET _Listen;
        SOCKADDR_IN *_Address;
        int _Backlog;
        ListenerCallback _Callback;
        ListenerErrorCallback _ErrorCallback;
        void *_Key;
        AcceptorPtr _Self;

        bool _StopTrigger = false;
    public:
        static AcceptorPtr Listen(ServiceListener *Port,
                                  ListenerCallback Callback, ListenerErrorCallback ErrorCallback,
                                  SOCKADDR_IN *Ip, void *AdditionalKey, int Backlog = 0);

        void Start();

        void Stop();

    private:
        Acceptor(ServiceListener *Port, ListenerCallback &Callback, ListenerErrorCallback &ErrorCallback,
                 SOCKADDR_IN *Ip, void *key, int Backlog);

        void InitializeListenSocket();

        void StartAccept() const;
    };

}

