#include <unordered_map>

#include "cache.hh"
#include "evictor.hh"

using namespace std;

/*
// Implement Universal Hashing: https://en.wikipedia.org/wiki/Universal_hashing
size_type std::hash<key_type>(key_type key, size_type a, size_type p)
{
  size_type hash = 0;
  for (size_type i=0; i< key.length(); ++i) {
    hash = ((h*a) + key[i]) % p;
  }
  return hash;
}
*/

class Cache::Impl {
  public:
    size_type maxmem;
    float max_load_factor;
    Evictor* evictor;
    hash_func hasher;

    unordered_map<key_type, val_type, hash_func> m_cache;
    byte_type memused;

    Impl(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher) : maxmem(maxmem), max_load_factor(max_load_factor), evictor(evictor), hasher(hasher),
                            memused(0), m_cache(0, hasher)
        {}

    ~Impl() = default;

    void set(key_type key, val_type val, size_type size)
    {
      if (memused + size > maxmem) {
        while (memused + size > maxmem) {
          auto item = m_cache.begin();
          del(item->first);
          memused -= sizeof(item->second);
        }
      }
      m_cache[key] = val;
      memused += size;
      return;
    }

    val_type get(key_type key, size_type& val_size) const {};

    bool del(key_type key) {};

    size_type space_used() const {};

    void reset() {};
};

Cache::Cache(size_type maxmem,
      float max_load_factor,
      Evictor* evictor,
      hash_func hasher) : pImpl_(new Impl(maxmem, max_load_factor, evictor, hasher)) {}
Cache::~Cache() {}

void Cache::set(key_type key, val_type val, size_type size)
{
  return pImpl_-> set(key, val, size);
}

Cache::val_type Cache::get(key_type key, size_type& val_size) const
{
  return pImpl_->get(key, val_size);
}

bool Cache::del(key_type key)
{
  return pImpl_->del(key);
}

Cache::size_type Cache::space_used() const
{
  return pImpl_->space_used();
}

void Cache::reset()
{
  return pImpl_->reset();
}