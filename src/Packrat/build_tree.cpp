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
#include "build_tree.hpp"
#include <cassert>
#include <string>
#include <iostream>
using namespace std;
using namespace packrat;

template<>
BuildTree<bool>::BuildTree(const AST& tree)
{
    if(tree["value"].size() == 0)
        t_ = new bool(tree["value"].length() > 0);
    else if(*tree["value"]["type"] == "bool")
        t_ = new bool(*tree["value"]["value"] == "true");
    else
        t_ = NULL;
}

template<>
BuildTree<int>::BuildTree(const AST& tree)
{
    if(*tree["value"]["type"] == "int")
    {
        t_ = new int();
        std::istringstream(*tree["value"]["value"]) >> *t_;
    }
    else
        t_ = NULL;
}

template<>
BuildTree<unsigned long long int>::BuildTree(const AST& tree)
{
    if(*tree["value"]["type"] == "int")
    {
        t_ = new unsigned long long int();
        std::istringstream(*tree["value"]["value"]) >> *t_;
    }
    else
        t_ = NULL;
}

template<>
BuildTree<string>::BuildTree(const AST& tree)
{
    if(*tree["value"]["type"] == "StringConst")
    {
        // FIXME
        t_ = NULL;
    }
    else if(*tree["value"]["type"] == "Ident")
    {
        t_ = new string(*tree["value"]["value"]);
    }
    else
    {
        t_ = new string(*tree["value"]);
    }
}
