//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Abstractions/MemoryPool.hpp>
#include <cassert>

MemoryPool::MemoryPool(std::size_t l_block_size, int l_pre_alloc, int l_max_alloc):
        _block_size(l_block_size),
        _max_alloc(l_max_alloc),
        _allocated(l_pre_alloc)
{
    assert(l_max_alloc == 0 || l_max_alloc >= l_pre_alloc);
    assert(l_pre_alloc >= 0 && l_max_alloc >= 0);

    int r = BLOCK_RESERVE;

    if (l_pre_alloc > r)
        r = l_pre_alloc;

    if (l_max_alloc > 0 && l_max_alloc < r)
        r = l_max_alloc;

    _blocks.reserve(static_cast<unsigned long>(r));

    try
    {
        for (int i = 0; i < l_pre_alloc; ++i) {
            _blocks.push_back(new char[_block_size]);
        }
    }
    catch (...)
    {
        Clear();
        throw;
    }

}

MemoryPool::~MemoryPool()
{
    Clear();
}

void *MemoryPool::GetMemory()
{
    std::lock_guard<std::mutex> lock{_mutex};

    if (_blocks.empty())
    {
        if (_max_alloc == 0 || _allocated < _max_alloc)
        {
            ++_allocated;
            return new char[_block_size];
        }
        else
            throw std::runtime_error("MemoryPool exhausted.");
    }
    else
    {
        auto ptr = _blocks.back();
        _blocks.pop_back();
        return ptr;
    }
}

void MemoryPool::Release(void *l_ptr)
{
    std::lock_guard<std::mutex> lock{_mutex};

    try
    {
        _blocks.push_back(reinterpret_cast<char*>(l_ptr));
    }
    catch (...)
    {
        delete [] reinterpret_cast<char*>(l_ptr);
    }
}

std::size_t MemoryPool::BlockSize() const
{
    return _block_size;
}

int MemoryPool::Allocated() const
{
    return _allocated;
}

int MemoryPool::Available() const
{
    return static_cast<int>(_blocks.size());
}

void MemoryPool::Clear()
{
    for (auto &_block : _blocks) {
        delete [] _block;
    }
    _blocks.clear();
}