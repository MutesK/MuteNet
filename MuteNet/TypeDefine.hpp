//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_TYPEDEFINE_HPP
#define MUTENET_TYPEDEFINE_HPP


#if defined(WIN32)
using socket_t = SOCKET;
using socklen_t = int;
#else
using socket_t = int;

const int8_t SOCKET_ERROR  = -1;
const int8_t INVALID_SOCKET =  (socket_t)(~0);

#endif

using descriptor_t = socket_t;


template<class Type>
struct LockObject : public Type
{
	std::shared_mutex _mutex;
};

#endif //MUTENET_TYPEDEFINE_HPP
