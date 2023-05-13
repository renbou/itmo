#pragma once

#include <algorithm>
#include <cstddef>
#include <list>
#include <new>
#include <ostream>
#include <stdexcept>

template <class Key, class KeyProvider, class Allocator>
class Cache
{
public:
    template <class... AllocArgs>
    Cache(const std::size_t cache_size, AllocArgs &&... alloc_args)
        : m_max_top_size(cache_size)
        , m_max_low_size(cache_size)
        , m_alloc(std::forward<AllocArgs>(alloc_args)...)
    {
        if (cache_size == 0) {
            throw std::invalid_argument("Cannot initialize cache of size 0");
        }
    }

    std::size_t size() const
    {
        return lru.size() + low_prior_queue.size();
    }

    bool empty() const
    {
        return lru.empty() && low_prior_queue.empty();
    }

    template <class T>
    T & get(const Key & key);

    std::ostream & print(std::ostream & strm) const;

    friend std::ostream & operator<<(std::ostream & strm, const Cache & cache)
    {
        return cache.print(strm);
    }

private:
    const std::size_t m_max_top_size;
    const std::size_t m_max_low_size;
    std::list<KeyProvider *> lru;
    std::list<KeyProvider *> low_prior_queue;
    Allocator m_alloc;

    // make el the low prior queue's new front element
    void low_prior_store(KeyProvider * el);

    // make el the lru's new front element
    void lru_store(KeyProvider * el);
};

template <class Key, class KeyProvider, class Allocator>
inline void Cache<Key, KeyProvider, Allocator>::low_prior_store(KeyProvider * el)
{
    if (low_prior_queue.size() == m_max_low_size) {
        m_alloc.template destroy<KeyProvider>(static_cast<void *>(low_prior_queue.back()));
        low_prior_queue.pop_back();
    }
    low_prior_queue.push_front(el);
}

template <class Key, class KeyProvider, class Allocator>
inline void Cache<Key, KeyProvider, Allocator>::lru_store(KeyProvider * el)
{
    if (lru.size() == m_max_top_size) {
        low_prior_store(lru.back());
        lru.pop_back();
    }
    lru.push_front(el);
}

template <class Key, class KeyProvider, class Allocator>
template <class T>
inline T & Cache<Key, KeyProvider, Allocator>::get(const Key & key)
{
    auto search = [&key, this](std::list<KeyProvider *> & list) {
        typename std::list<KeyProvider *>::iterator it = std::find_if(list.begin(), list.end(), [&key](KeyProvider * el) {
            return *el == key;
        });
        if (it != list.end()) {
            KeyProvider * save = *it;
            list.erase(it); // will not call the destructor
            lru_store(save);
            return true;
        }
        return false;
    };

    if (search(lru) || search(low_prior_queue)) {
        return dynamic_cast<T &>(*lru.front());
    }
    low_prior_store(m_alloc.template create<T>(key));
    return dynamic_cast<T &>(*low_prior_queue.front());
}

template <class Key, class KeyProvider, class Allocator>
inline std::ostream & Cache<Key, KeyProvider, Allocator>::print(std::ostream & strm) const
{
    auto helper = [&strm](const std::list<KeyProvider *> & list) {
        bool first = true;
        for (const auto x : list) {
            if (!first) {
                strm << ' ';
            }
            else {
                first = false;
            }
            strm << *x;
        }
    };
    strm << "Priority: ";
    helper(lru);
    strm << "\nRegular: ";
    helper(low_prior_queue);
    strm << std::endl;
    return strm;
}
