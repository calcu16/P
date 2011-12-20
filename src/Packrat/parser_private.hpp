namespace packrat
{
    template<typename... Args>
    Parser& Parser::add(const std::string& name, const Symbol& value, Args... args)
    {
        return add(name, value).add(args...);
    }
    template<typename... Args>
    Parser& Parser::add(const std::string& name, const std::string& value, Args... args)
    {
        return add(name, value).add(args...);
    }
    template<typename... Args>
    Parser::Parser(Args... args)
    {
        add(args...);
    }
}
