// Copied from stackoverflow: How To Easily Indent Output To Ofstream
// http://stackoverflow.com/questions/1391746/how-to-easily-indent-
//        output-to-ofstream

#include <locale>
#include <algorithm>
#include <iostream>
#include <fstream>

class IndentFacet: public std::codecvt<char,char,std::mbstate_t>
{
public:
    explicit IndentFacet(size_t ref = 0): std::codecvt<char,char,std::mbstate_t>(ref)    {}
    
    typedef std::codecvt_base::result               result;
    typedef std::codecvt<char,char,std::mbstate_t>  parent;
    typedef parent::intern_type                     intern_type;
    typedef parent::extern_type                     extern_type;
    typedef parent::state_type                      state_type;
    
    int&    state(state_type& s) const          {return *reinterpret_cast<int*>(&s);}
protected:
    virtual result do_out(state_type& tabNeeded,
                          const intern_type* rStart, const intern_type*  rEnd, const intern_type*&   rNewStart,
                          extern_type*       wStart, extern_type*        wEnd, extern_type*&         wNewStart) const
    {
        result  res = std::codecvt_base::noconv;
        
        for(;(rStart < rEnd) && (wStart < wEnd);++rStart,++wStart)
        {
            // 0 indicates that the last character seen was a newline.
            // thus we will print a tab before it. Ignore it the next
            // character is also a newline
            if ((state(tabNeeded) == 0) && (*rStart != '\n'))
            {
                res                 = std::codecvt_base::ok;
                state(tabNeeded)    = 1;
                *wStart             = '\t';
                ++wStart;
                if (wStart == wEnd)
                {
                    res     = std::codecvt_base::partial;
                    break;
                }
            }
            // Copy the next character.
            *wStart         = *rStart;
            
            // If the character copied was a '\n' mark that state
            if (*rStart == '\n')
            {
                state(tabNeeded)    = 0;
            }
        }
        
        if (rStart != rEnd)
        {
            res = std::codecvt_base::partial;
        }
        rNewStart   = rStart;
        wNewStart   = wStart;
        
        return res;
    }
    
    // Override so the do_out() virtual function is called.
    virtual bool do_always_noconv() const throw()
    {
        return false;   // Sometime we add extra tabs
    }
    
};
