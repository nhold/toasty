#pragma once

template <typename T>
class Vector2
{
    public:
        Vector2();
        Vector2(T x, T y);

        T x;
        T y;
};

#include <Vector2.inl>

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2ui;
typedef Vector2<float> Vector2f;