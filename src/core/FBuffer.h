#pragma once

#include <cstddef>

class FBuffer
{
public:
    FBuffer() = default;
    virtual ~FBuffer() = default;

    virtual void Clear() = 0;
    virtual size_t Size() const = 0;
};