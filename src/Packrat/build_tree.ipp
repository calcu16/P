template<typename T>
T buildTree(const AST& tree)
{
    BuildTree<T> build(tree);
    return *build;
}

template<size_t I, typename T>
T buildTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

template<typename T>
BuildTree<T>::~BuildTree()
{
    delete t_;
}

template<typename L>
BuildTree<std::list<L> >::~BuildTree()
{
    delete t_;
}

template<typename... US>
BuildTree<wrapper::Union<US...> >::~BuildTree()
{
    delete t_;
}

template<typename T>
void BuildTree<T>::swap(BuildTree<T>& other)
{
    std::swap(t_, other._);
}

template<typename L>
void BuildTree<std::list<L> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other._);
}

template<typename... US>
void BuildTree<wrapper::Union<US...> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other._);
}

template<typename T>
BuildTree<T>& BuildTree<T>::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}

template<typename L>
BuildTree<std::list<L> >& BuildTree<std::list<L> >::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}

template<typename... US>
BuildTree<wrapper::Union<US...> >& BuildTree<wrapper::Union<US...> >::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}
template<typename T>
T& BuildTree<T>::operator*()
{
    return *t_;
}

template<typename L>
std::list<L>& BuildTree<std::list<L> >::operator*()
{
    return *t_;
}

template<typename... US>
wrapper::Union<US...>& BuildTree<wrapper::Union<US...> >::operator*()
{
    return *t_;
}

template<typename T>
const T& BuildTree<T>::operator*() const
{
    return *t_;
}

template<typename L>
const std::list<L>& BuildTree<std::list<L> >::operator*() const
{
    return *t_;
}

template<typename... US>
const wrapper::Union<US...>& BuildTree<wrapper::Union<US...> >::operator*() const
{
    return *t_;
}

template<typename T>
BuildTree<T>::BuildTree(const AST& tree)
    : t_(new T())
{
    t_->value_ = buildTree<T::count, typename T::type>(tree, T::names);
}

template<>
BuildTree<int>::BuildTree(const AST& tree)
{
    if(*tree["type"] == "int")
    {
        t_ = new int();
        std::istringstream(*tree["value"]) >> *t_;
    }
    else
        t_ = NULL;
}

template<>
BuildTree<std::string>::BuildTree(const AST& tree)
{
    if(*tree["type"] == "string")
    {
        // FIXME
        t_ = NULL;
    }
    else
        t_ = new std::string(*tree);
}

template<typename L>
BuildTree<std::list<L> >::BuildTree(const AST& tree)
{
    t_ = new std::list<L>();
    for(AST::const_iterator i = tree.begin(); i != tree.end(); ++i)
        t_->push_back(buildTree<L>(*i));
}

template<typename... US>
BuildTree<wrapper::Union<US...> >::BuildTree(const AST& tree, name_t names)
{
    t_ = new wrapper::Union<US...>();
    BuildUnion<size, US...>(tree, names, *t_);
}

template<typename... TS>
template<size_t I, typename... US>
BuildTree<wrapper::Union<TS...> >::BuildUnion<I, US...>::BuildUnion
    (const AST& tree, name_t names, wrapper::Union<US...>& result)
{
    typedef typename wrapper::type<I-1, US...>::value type;
    if(*tree["type"] == names[I-1])
        result.template set<I-1>(buildTree<type>(tree["value"]));
    else
        BuildUnion<I-1,US...>(tree, names, result);
}


