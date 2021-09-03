#ifndef Src_NONCOPYABLE_H_
#define Src_NONCOPYABLE_H_


//  Private copy constructor and copy assignment ensure classes derived from
//  class noncopyable cannot be copied.

namespace SeasSnowFlake {
    namespace Src {

        // protection from unintended ADL(Argument Dependent Lookup)
        namespace Noncopyable_ {

            class Noncopyable
            {
            protected:
                Noncopyable() = default;
                ~Noncopyable() = default;
                
                Noncopyable(const Noncopyable&) = delete;
                Noncopyable(const Noncopyable&&) = delete;
                Noncopyable& operator=(const Noncopyable&) = delete;
                Noncopyable& operator=(const Noncopyable&&) = delete;
            };
        }

        typedef Noncopyable_::Noncopyable Noncopyable;
    }
}

#endif // _JW_CORE_NONCOPYABLE_H_
