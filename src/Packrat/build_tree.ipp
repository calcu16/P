template<typename T>
typename BuildTree<T>::return_t buildTree(const AST& tree)
{
    BuildTree<T> build(tree);
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

template<typename... TS>
BuildTree<std::tuple<TS...> >::~BuildTree()
{
    delete t_;
}

template<typename S, typename R, typename SEP>
BuildTree<fold_left_t<S, R, SEP> >::~BuildTree()
{
    delete r_;
}

template<typename T>
void BuildTree<T>::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

template<typename L>
void BuildTree<std::list<L> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

template<typename... US>
void BuildTree<wrapper::Union<US...> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

template<typename... TS>
void BuildTree<std::tuple<TS...> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

template<typename S, typename R, typename SEP>
void BuildTree<fold_left_t<S, R, SEP> >::swap(BuildTree<T>& other)
{
    std::swap(r_, other.r_);
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
BuildTree<wrapper::Union<US...> >&
    BuildTree<wrapper::Union<US...> >::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}

template<typename... TS>
BuildTree<std::tuple<TS...> >&
    BuildTree<std::tuple<TS...> >::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}
template<typename S, typename R, typename SEP>
BuildTree<fold_left_t<S, R, SEP> >&
    BuildTree<fold_left_t<S, R, SEP> >::operator=(BuildTree<T> rhs)
{
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

template<typename... TS>
std::tuple<TS...>& BuildTree<std::tuple<TS...> >::operator*()
{
    return *t_;
}

template<typename S, typename R, typename SEP>
std::tuple<R, SEP, R>& BuildTree<fold_left_t<S, R, SEP> >::operator*()
{
    return *r_;
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
const wrapper::Union<US...>&
    BuildTree<wrapper::Union<US...> >::operator*() const
{
    return *t_;
}

template<typename... TS>
const std::tuple<TS...>& BuildTree<std::tuple<TS...> >::operator*() const
{
    return *t_;
}

template<typename S, typename R, typename SEP>
const std::tuple<R, SEP, R>& BuildTree<fold_left_t<S, R, SEP> >::operator*() const
{
    return *r_;
}

template<typename T>
BuildTree<T>::BuildTree(const AST& tree)
    : t_(new T())
{
    t_->value_ = BuildTree<typename T::type>::template makeTree<T::names_l>(tree, T::names);
}

template<>
BuildTree<int>::BuildTree(const AST& tree);

template<>
BuildTree<std::string>::BuildTree(const AST& tree);

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
    (const AST& tree, name_t names, T& result)
{
    typedef typename wrapper::type<I-1, US...>::value type;
    if(*tree["type"] == names[I-1])
        result.template set<I-1>(buildTree<type>(tree["value"]));
    else
        BuildUnion<I-1,US...>(tree, names, result);
}

template<typename... TS>
template<typename... US>
BuildTree<wrapper::Union<TS...> >::BuildUnion<0, US...>::BuildUnion
    (const AST&, name_t, T&)
{
}

template<typename... TS>
BuildTree<std::tuple<TS...> >::BuildTree(const AST& tree, name_t names)
{
    t_ = new std::tuple<TS...>();
    BuildTuple<size, TS...>(tree, names, *t_);
}

template<typename... TS>
template<size_t I, typename... US>
BuildTree<std::tuple<TS...> >::BuildTuple<I, US...>::BuildTuple
    (const AST& tree, name_t names, T& result)
{
    typedef typename wrapper::type<I-1, US...>::value type;
    result.template get<I-1,US...>() = buildTree<type>(tree["value"]);
    BuildTuple<I-1,US...>(tree, names, result);
}

template<typename... TS>
template<typename... US>
BuildTree<std::tuple<TS...> >::BuildTuple<0, US...>::BuildTuple
    (const AST&, name_t, T&)
{
}

template<typename S, typename R, typename SEP>
BuildTree<fold_left_t<S, R, SEP> >::BuildTree(const AST& tree, name_t names)
    : r_(NULL)
{
    AST::const_iterator i = tree.begin();
    std::string value = names[0], sep = names[1];
    
    R left = buildTree<R>((*i)[value]);
    for(++i; i != tree.end(); ++i)
    {
        r_ = new std::tuple<R,SEP,R>(left, buildTree<SEP>((*i)[sep]), buildTree<R>((*i)[value]));
        S holder;
        holder.value_ = *r_;
        left = (R)holder;
    }
}

template<typename T>
template<size_t I>
T BuildTree<T>::makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

template<typename... US>
template<size_t I>
wrapper::Union<US...> BuildTree<wrapper::Union<US...> >::makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

template<typename... TS>
template<size_t I>
std::tuple<TS...> BuildTree<std::tuple<TS...> >::makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

template<typename S, typename R, typename SEP>
template<size_t I>
std::tuple<R, SEP, R> BuildTree<fold_left_t<S, R, SEP> >::makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

template<>
template<size_t I>
int BuildTree<int>::makeTree(const AST& tree, typename table_t<I>::type names)
{
    size_t i;
    for(i = 0; i < I; ++i)
        if(*tree["value"] == names[i])
            break;
    return i;
}
