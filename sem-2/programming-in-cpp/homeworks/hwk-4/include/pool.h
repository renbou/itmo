#pragma once

#include <memory>

/// hide implementation details, exposing only pimpl interface
class PoolAllocator
{
public:
    PoolAllocator(unsigned /*min_p*/, unsigned /*max_p*/);
    ~PoolAllocator();

    void * allocate(std::size_t /*n*/);

    void deallocate(const void * /*ptr*/);

private:
    class Pool;
    std::unique_ptr<Pool> pool;
};
