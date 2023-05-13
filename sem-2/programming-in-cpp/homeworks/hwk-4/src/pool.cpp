#include "pool.h"

#include <cstddef>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using byte = std::byte;

bool is_power_of_2(std::uint64_t x)
{
    return (x & (x - 1)) == 0;
}

/// Log for max size
std::uint64_t lg2(std::uint64_t x)
{
    unsigned int k = 0;
    while (x > 1) {
        x /= 2;
        k++;
    }
    return k;
}

struct List
{
    List *next, *prev;
};

class PoolAllocator::Pool
{
public:
    Pool(const unsigned int min_p, const unsigned int max_p)
        : max_p(max_p)
        , max_block_size(std::size_t(1) << max_p)
        , min_p(min_p)
        , min_block_size(std::size_t(1) << min_p)
        , scaled(max_p - min_p)
        , memory(std::size_t(1) << max_p)
        , block_meta(std::size_t(1) << scaled, byte(0b10000000u))
        , buckets(scaled + 1)
        , list_items(std::size_t(1) << scaled)
    {
        block_meta[0] = encode_meta(max_p, FREE);
        buckets[0] = &list_items[0];
    }

    byte * allocate(std::size_t size)
    {
        if (size > max_block_size) {
            throw std::bad_alloc{};
        }
        if (!is_power_of_2(size)) {
            size = std::size_t(1ull) << (lg2(size) + 1u);
        }
        if (size < min_block_size) {
            size = min_block_size;
        }

        const byte * block = find_free_block(size);
        if (block == nullptr) {
            throw std::bad_alloc{};
        }
        return const_cast<byte *>(split_block(block, size));
    }

    void deallocate(const byte * block)
    {
        if (block < &memory[0] ||
            block > &memory[memory.size() - 1]) {
            throw std::domain_error("Unable to deallocate memory not from this pool.");
        }
        const unsigned int index = block_index(block);
        if (block_status(index) == FREE) {
            throw std::invalid_argument("Unable to deallocate a free block");
        }
        std::size_t size = block_size(index);
        if (size < min_p) {
            throw std::runtime_error("Block meta information is corrupted - stored size is smaller than min block size");
        }
        if ((block - &memory[0]) % size != 0) {
            throw std::invalid_argument("Invalid block cannot be deallocated (not on pool block boundary)");
        }
        merge(block);
    }

private:
    enum Status : unsigned char
    {
        FREE = 0,
        USED = 1
    };

    byte encode_meta(unsigned char log_size, unsigned char used)
    {
        return static_cast<byte>(log_size | (used << 7u));
    }

    /// Block index if the block is of the smallest size
    unsigned int block_index(const byte * block) const
    {
        return (block - &memory[0]) / min_block_size;
    }

    unsigned int relative_block_index(const byte * block, std::size_t size)
    {
        return (block - &memory[0]) / size;
    }

    std::size_t block_size(const unsigned int index)
    {
        return std::size_t(1) << (static_cast<unsigned int>(block_meta[index]) & 127u);
    }

    Status block_status(const unsigned int index)
    {
        return Status(static_cast<unsigned int>(block_meta[index]) & 128u);
    }

    unsigned int bucket_node_index(const List * ptr)
    {
        return ptr - &list_items[0];
    }

    const byte * bucket_node_to_memory(const List * ptr)
    {
        return &memory[bucket_node_index(ptr) * min_block_size];
    }

    List * memory_to_bucket_node(const byte * ptr)
    {
        return &list_items[block_index(ptr)];
    }

    std::pair<unsigned int, unsigned int> buddy(const byte * block, std::size_t size)
    {
        const unsigned int index = relative_block_index(block, size);
        return {block_index(&memory[(index ^ 1u) * size]), (index ^ 1u)};
    }

    void add_to_bucket(const unsigned int bucket, List * node)
    {
        if (buckets[bucket] == nullptr) {
            buckets[bucket] = node;
        }
        else {
            buckets[bucket]->prev = node;
            node->next = buckets[bucket];
            buckets[bucket] = node;
        }
    }

    void remove_from_bucket(const unsigned int bucket, List * node)
    {
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
        if (buckets[bucket] == node) {
            buckets[bucket] = node->next;
        }
        else {
            node->prev->next = node->next;
        }
        node->next = node->prev = nullptr;
    }

    /// Given index of found free block, split it until the required size and return pointer to "allocated" block
    /// Set the blocks which are split to used so that we don't accidentally coalesce them
    const byte * split_block(const byte * block, const std::size_t required_size)
    {
        std::size_t current_size = block_size(block_index(block));
        unsigned char lg2_current_size = lg2(current_size);

        while (current_size != required_size) {
            block_meta[block_index(block)] = encode_meta(0, USED);
            /// Save second block as the free block for next size
            const byte * second_half = static_cast<const byte *>(block + current_size / 2);
            block_meta[block_index(second_half)] = encode_meta(lg2_current_size - 1, FREE);
            add_to_bucket(max_p - (lg2_current_size - 1), memory_to_bucket_node(second_half));
            current_size /= 2;
            lg2_current_size--;
        }

        // set resulting block to used as well
        block_meta[block_index(block)] = encode_meta(lg2_current_size, USED);
        return block;
    }

    const byte * get_block(unsigned int bucket)
    {
        while (buckets[bucket] != nullptr) {
            List * current = buckets[bucket];
            remove_from_bucket(bucket, current);
            if (block_status(bucket_node_index(current)) == FREE) {
                return bucket_node_to_memory(current);
            }
        }
        return nullptr;
    }

    const byte * find_free_block(const std::size_t size)
    {
        const byte * free_block = nullptr;
        for (int i = max_p - lg2(size); i >= 0 && free_block == nullptr; i--) {
            free_block = get_block(i);
        }
        return free_block;
    }

    void merge(const byte * block)
    {
        std::size_t current_size = (block_size(block_index(block)));
        unsigned char lg2_current_size = lg2(current_size);
        for (auto [buddy_index, relative_index] = buddy(block, current_size);
             current_size < memory.size() &&
             block_size(buddy_index) == current_size &&
             block_status(buddy_index) == FREE;
             std::tie(buddy_index, relative_index) = buddy(block, current_size)) {
            block_meta[block_index(block)] = encode_meta(0, USED);
            block_meta[buddy_index] = encode_meta(0, USED);
            /// Remove the buddy from the free bucket
            remove_from_bucket(max_p - lg2_current_size,
                               memory_to_bucket_node(&memory[relative_index * current_size]));
            /// Parent block
            block = &memory[(relative_index / 2) * current_size * 2];
            current_size *= 2;
            lg2_current_size++;
        }

        // set resulting block to free
        block_meta[block_index(block)] = encode_meta(lg2_current_size, FREE);
        add_to_bucket(max_p - lg2_current_size, memory_to_bucket_node(block));
    }

    const unsigned int max_p;
    const std::size_t max_block_size;
    const unsigned int min_p;
    const std::size_t min_block_size;
    const unsigned int scaled;
    /// Make it const locally so that we don't accidentally change it, do const_cast only in allocate
    const std::vector<byte> memory;

    /// block of size n=2^max_p has index 0, blocks of size n/2 have indices {1, 2}, blocks of size n/2/2 have indices {3, 4, 5, 6}, ...
    std::vector<byte> block_meta;
    /// Buckets of linked lists
    std::vector<List *> buckets;
    std::vector<List> list_items;
};

PoolAllocator::PoolAllocator(const unsigned int min_p, const unsigned int max_p)
    : pool(std::make_unique<Pool>(min_p, max_p)) {}

PoolAllocator::~PoolAllocator() = default;

void * PoolAllocator::allocate(const std::size_t size)
{
    return pool->allocate(size);
}

void PoolAllocator::deallocate(const void * ptr)
{
    pool->deallocate(static_cast<const byte *>(ptr));
}
