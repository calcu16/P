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
