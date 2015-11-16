//
// Created by gxdw68 on 2015/11/13.
//

#include "gtest/gtest.h"
#include "FunctorWrapper.hpp"

void simpleFreeFuncton()
{
}

TEST(FunctorWrapper, wrapperSimpleFreeFunction)
{
    auto fw1 = FunctorWrapper<>(simpleFreeFuncton);
    fw1();
    auto fw2 = MakeFunctorWrapper(simpleFreeFuncton);
    fw2();
}