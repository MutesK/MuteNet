#pragma once

#include "MemoryStream.h"

namespace Util
{
    class InputMemoryStream : public MemoryStream
    {
    public:
        InputMemoryStream() = delete;

        virtual ~InputMemoryStream() = default;

        virtual int64_t GetRemainingDataSize() const;

        virtual void Read(void *outData, uint32_t inByteCount);

        template<typename Type>
        void Read(Type &outData);

        template<typename Type>
        void Read(std::vector<Type> &vector);

        virtual void Serialize(void *outData, uint32_t inByteCount) override;
    };

    inline int64_t InputMemoryStream::GetRemainingDataSize() const
    {
        return _Tail - _Head;
    }

    template<typename Type>
    void InputMemoryStream::Read(Type &outData)
    {
        Read(&outData, sizeof(Type));
    }

    template<typename Type>
    void InputMemoryStream::Read(std::vector<Type> &vector)
    {
        size_t elemCount = 0;
        Read(elemCount);
        vector.resize(elemCount);

        for (Type &elem : vector)
        {
            Read(elem);
        }
    }

}