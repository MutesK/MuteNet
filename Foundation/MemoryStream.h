#pragma once


namespace Util
{
    using Byte = char;

    class MemoryStream
    {
        static const int64_t BUFFER_LENGTH = 20000;
    protected:
        Byte *_Buffer;

        int64_t _Head = 0;
        int64_t _Tail = 0;
        int64_t _Capacity = BUFFER_LENGTH;
    public:
        MemoryStream();

        virtual ~MemoryStream();

        virtual void Serialize(void *Data, uint32_t ByteCount) = 0;

        const char *GetBufferPtr() const;
    };


    inline const char *MemoryStream::GetBufferPtr() const
    {
        return _Buffer;
    }
}
