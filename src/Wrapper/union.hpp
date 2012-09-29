/*
Copyright (c) 2012, Andrew Carter, Dietrich Langenbach, Xanda Schofield
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/
#ifndef WRAPPER_UNION_HPP
#define WRAPPER_UNION_HPP
#include "type.hpp"
namespace wrapper
{
    // Empty unions do not make any sense,
    //  so we require at least 1 element
    //  (prolly should be 2).
    template<typename... TS>
    class Union {};
    
    template<typename T, typename... TS>
    class Union<T, TS...>
    {
    private:
        template<int, typename...>
        class Helper {};
        
        Helper<0, T, TS...> helper_;
        int active_;
        void* value_;
    public:
        static const size_t size = type<0, T, TS...>::size;
        
        Union();
        Union(const Union<T,TS...>&);
        ~Union();
        
        void swap(Union<T,TS...>&);
        Union<T,TS...>& operator=(Union<T,TS...>);
        
        template<int I>
        typename type<I,T,TS...>::const_reference get() const;
        template<int I>
        void set(typename type<I,T,TS...>::const_reference);
        operator int() const;
    private:
        void* copyActive() const;
        void destroyActive();
        
        template<int I>
        struct Helper<I>
        {
            inline void* copy(int i, void*) const;
            inline void destroy(int i, void*) const;
        };
        
        template<int I, typename U, typename... US>
        struct Helper<I, U, US...>
        {
            Helper<I+1,US...> next_;
            inline void* copy(int i, void*) const;
            inline void destroy(int i, void*) const;
        };
    };
}
#include "union.ipp"
#endif
