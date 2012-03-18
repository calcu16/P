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
        typedef typename Helper<I-1,TS...>::value value;
        typedef typename Helper<I-1,TS...>::reference reference;
        typedef typename Helper<I-1,TS...>::const_reference const_reference;
        typedef typename Helper<I-1,TS...>::pointer pointer;
        typedef typename Helper<I-1,TS...>::const_pointer const_pointer;
        
    };
    
    template<typename T, typename... TS>
    struct type<0, T, TS...>
    {
        
        typedef T  value;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef T const * const_pointer;
    };
}
#endif
