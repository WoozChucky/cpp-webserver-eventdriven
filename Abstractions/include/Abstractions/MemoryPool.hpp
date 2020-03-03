//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef MEMORYPOOL_HPP
#define MEMORYPOOL_HPP

#include <vector>
#include <mutex>
#include "Logger.hpp"

/**
 * @brief A simple pool for fixed-size memory blocks.
 *
 * The main purpose of this class is to speed-up
 * memory allocations, as well as to reduce memory
 * fragmentation in situations where same blocks
 * are allocated all over again, such as in server
 * applications.
 *
 * All allocated blocks are retained for future use.
 * A limit on the number of blocks can be specified.
 * Blocks can be pre-allocated.
 */
class MemoryPool {

public:

    /**
     * @brief Creates a MemoryPool for blocks with given l_block_size.
     * The number of blocks given in l_pre_alloc are pre-allocated.
     * @param l_block_size The size of a block.
     * @param l_pre_alloc  The number of blocks to preallocate.
     * @param l_max_alloc  The max number of blocks that can be pre-allocated.
     */
    explicit MemoryPool(std::size_t l_block_size, int l_pre_alloc = 0, int l_max_alloc = 0);
    MemoryPool() = delete;

    ~MemoryPool();

    /**
     * @brief Returns a memory block.
     * If there are no more blocks in the pool, a new block will be allocated.
     *
     * If maxAlloc blocks are already allocated, an
     * OutOfMemory exception is thrown.
     *
     */
    void* GetMemory();

    template <typename T>
    inline T* Allocate() {
        if (sizeof(T) > BlockSize()) {
            TRACE("%s", "Not enough memory in a block.");
            return nullptr;
        } else {
            return reinterpret_cast<T*>(GetMemory());
        }
    }

    /**
     * @brief Releases a memory block and returns it to the pool
     * @param l_ptr The memory block
     */
    void Release(void* l_ptr);

    /**
     * @brief Returns the size of a block.
     *
     */
    [[nodiscard]] std::size_t BlockSize() const;

    /**
     * @brief Returns the number of allocated blocks.
     *
     */
    [[nodiscard]] int Allocated() const;

    /**
     * @brief Returns the number of available blocks in the pool.
     *
     */
    [[nodiscard]] int Available() const;

private:

    MemoryPool(const MemoryPool&);
    MemoryPool& operator= (const MemoryPool&);

    void Clear();

    enum
    {
        BLOCK_RESERVE = 128
    };

    typedef std::vector<char*> BlockVec;

    std::size_t _block_size;
    int 		_max_alloc;
    int 		_allocated;
    BlockVec	_blocks;
    std::mutex	_mutex;

};

#endif //MEMORYPOOL_HPP
