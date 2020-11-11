//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_TYPEDEFINE_HPP
#define MUTENET_TYPEDEFINE_HPP


#if defined(WIN32)
using socket_t = SOCKET;
#else
using socket_t = int;
#endif


#endif //MUTENET_TYPEDEFINE_HPP
