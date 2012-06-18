/*
Copyright (c) 2012, Andrew Carter, Dietrich Lagenbach, Xanda Schofield
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
#ifndef WRAPPER_TYPE_HPP
#define WRAPPER_TYPE_HPP
namespace wrapper
{
    template<size_t I, typename... TS>
    struct type
    {
    private:
        template<size_t J, typename... US>
        struct Helper;
        
        template<size_t J, typename U, typename... US>
        struct Helper<J,U,US...>
        {
            typedef typename type<J,US...>::value value;
            typedef typename type<J,US...>::reference reference;
            typedef typename type<J,US...>::const_reference const_reference;
            typedef typename type<J,US...>::pointer pointer;
            typedef typename type<J,US...>::const_pointer const_pointer;
        };
    public:
        static const size_t size = 1 + type<0, TS...>::size;
        typedef typename Helper<I-1,TS...>::value value;
        typedef typename Helper<I-1,TS...>::reference reference;
        typedef typename Helper<I-1,TS...>::const_reference const_reference;
        typedef typename Helper<I-1,TS...>::pointer pointer;
        typedef typename Helper<I-1,TS...>::const_pointer const_pointer;
        
    };
    
    template<typename T, typename... TS>
    struct type<0, T, TS...>
    {
        static const size_t size = 1 + type<0, TS...>::size;
        typedef T  value;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef T const * const_pointer;
    };
    
    template<size_t I>
    struct type<I>
    {
        static const size_t size = 0;
        typedef void  value;
        typedef void  reference;
        typedef void  const_refernce;
        typedef void* pointer;
        typedef void* const_pointer;
    };
}
#endif
