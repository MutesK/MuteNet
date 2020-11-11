//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_EVENTBASECOMPONENT_HPP
#define MUTENET_EVENTBASECOMPONENT_HPP

namespace EventLoop
{
    class IOContextImpl;

    using IOContextImplPtr = std::shared_ptr<IOContextImpl>;

    class IEventBaseComponent
    {
    protected:
        IOContextImplPtr _ContextPtr;

        IEventBaseComponent(const IOContextImplPtr &Ptr);
    };

}


#endif //MUTENET_EVENTBASECOMPONENT_HPP
