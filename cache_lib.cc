#include <unordered_map>

#include "cache.hh"
#include "evictor.hh"
#include <cmath>
using namespace std;


// Implement Rolling Hash: https://en.wikipedia.org/wiki/Rolling_hash#Rabin-Karp_rolling_hash
size_type std::hash<key_type>(key_type key, size_type p = 53, size_type m)
{
  size_type key_as_unsigned= 0;
  for (key_type::size_type i = 0; i < key.size(); i++) {
    key_as_unsigned+=(((unsigned long long int )key[i])*std::pow(p, i)) %m;
    key_as_unsigned= key_as_unsigned % m;
  }

  return std::floor(m* (k*A- std::floor(k*A) ) );
}


class Cache::Impl {
  public:
    size_type maxmem;
    float max_load_factor;
    Evictor* evictor;
    hash_func hasher;

    unordered_map<key_type, val_type, hash_func> m_cache;
    size_type memused;

    Impl(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher) : maxmem(maxmem), max_load_factor(max_load_factor), evictor(evictor), hasher(hasher),
                            memused(0), m_cache(0, hasher)
      {
        m_cache.max_load_factor(max_load_factor);
      }

    ~Impl() = default;

    void set(key_type key, val_type val, size_type size)
    {

      while (memused + size > maxmem) {
        //TO-DO: Body of this loop needs to change and be replaced with Evictor functionality
        auto item = m_cache.begin();
        if (del(item->first)) {
          memused -= strlen(item->second)+1;  // evict old values in cache to make enough space for new ones

        } else {

          break;  // no more cache items to evict; stop accepting new values
        };
      }
      m_cache[key] = val;
      memused += size;
      return;
    }

    val_type get(key_type key, size_type& val_size) const {
      auto item = m_cache.find(key);
      if (item == m_cache.end()) {
        std::cout << "Item not found" << std::endl;
        val_size = 1;
        return nullptr;
      }
      val_size = strlen(item->second)+1;
      return item->second;
    };

    bool del(key_type key) {
      return m_cache.erase(key) == 1 ? true : false;
    };

    size_type space_used() const {
      return memused;
    };

    void reset() {
      m_cache.clear();
      memused = 0;
    };
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
