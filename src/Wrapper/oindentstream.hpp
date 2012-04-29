#ifndef WRAPPER_OINDENTSTREAM
#define WRAPPER_OINDENTSTREAM
#include <string>
#include <iostream>
namespace wrapper
{
    struct Endl {} endl;
    
    class oIndentStream
    {
        size_t indents_;
        size_t precedent_;
        std::string holding_;
        std::ostream* wrapped_;
        bool newline_;
    public:
        oIndentStream(std::ostream& wrapped)
            : indents_(0), precedent_(0),
                wrapped_(&wrapped), newline_(true) {}
        
        oIndentStream& operator++()
        {
            ++indents_;
            return *this;
        }
        
        oIndentStream& operator--()
        {
            if(indents_ != 0)
                --indents_;
            return *this;
        }
        
        size_t& operator*() { return precedent_; }
        const size_t& operator*() const { return precedent_; }
        
        oIndentStream& operator<<(const Endl&)
        {
            newline_ = true;
            *wrapped_ << std::endl;
            return *this;
        }
        
        oIndentStream& operator<<(const std::string& s)
        {
            if(newline_)
                for(size_t i = 0; i < indents_; ++i)
                    *wrapped_ << "    ";
            newline_ = false;
            *wrapped_ << s;
            return *this;
        }
        
        oIndentStream& operator<<(const unsigned long long int& ui)
        {
            if(newline_)
                for(size_t i = 0; i < indents_; ++i)
                    *wrapped_ << "    ";
            newline_ = false;
            *wrapped_ << ui;
            return *this;
        }
    };
}
#endif
