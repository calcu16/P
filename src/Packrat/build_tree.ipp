#include <tuple>
template<typename T>
T packrat::buildTree(const AST& tree)
{
    return *BuildTree<T>(tree);
}
