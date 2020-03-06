#include <unordered_map>

#include "cache.hh"
#include "evictor.hh"
#include <cmath>
#include <iostream>
#include <cstring>


// Implement Rolling Hash: https://en.wikipedia.org/wiki/Rolling_hash#Rabin-Karp_rolling_hash
/*Cache::size_type default_hash(const key_type &key, const Cache::size_type &m, const Cache::size_type p = 53)
{
  Cache::size_type key_as_unsigned= 0;
  for (Cache::size_type i = 0; i < key.size(); i++) {
    key_as_unsigned+=(((unsigned long int )key[i])*std::pow(p, i)) %m;
    key_as_unsigned= key_as_unsigned % m;
  }

  return std::floor(m* (k*A- std::floor(k*A) ) );
}
*/

class Cache::Impl {
  public:
    size_type maxmem;
    float max_load_factor;
    Evictor* evictor;
    hash_func hasher;

    std::unordered_map<key_type, val_type, hash_func> m_cache;
    size_type memused;

    Impl(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher = std::hash<key_type>()) :
                            maxmem(maxmem), max_load_factor(max_load_factor), evictor(evictor), hasher(hasher),
                            memused(0), m_cache(0, hasher)
      {
        m_cache.max_load_factor(max_load_factor);
      }

    ~Impl() {
      for (auto it=m_cache.begin(); it!= m_cache.end(); ++it) {
        del(m_cache[it->first]);
      }
      if (evictor!= nullptr) {
        delete evictor;
      }

    }

    void set(key_type key, val_type val, size_type size)
    {
      if (size>maxmem) {
        return;
      }
      if (memused + size >maxmem and evictor==nullptr) {
        return;
      }
      while (memused + size > maxmem) {
        auto key_to_evict = evictor->evict();
        del(key);
      }
      if (m_cache.find(key)!= m_cache.end()) {
        del(key);
      }

      val_type new_val;
      std::memcpy(new_val, val, size+1);
      m_cache[key] = new_val;
      memused += size+1;
      evictor->touch_key(key);
      return;
    }

    val_type get(key_type key, size_type& val_size) const {
      auto item = m_cache.find(key);
      if (item == m_cache.end()) {
        std::cout << "Item not found" << std::endl;
        val_size = 0;
        return nullptr;
      }
      val_size = strlen(item->second);
      evictor->touch_key(key);
      return item->second;
    };

    bool del(key_type key) {
      size_type size =0;
      get(key, size);
      if (size>0) {
        delete[] m_cache[key];
        m_cache.erase(key);
        memused-= size+1;
        return true;
      } else {
        return false;
      }
    };

    size_type space_used() const {
      return memused;
    };

    void reset() {
      m_cache.clear();
      memused = 0;
    };
    /*
    key_type key_to_evict() {
      if (evictor!= nullptr) {
        return evictor->evict();
      } else {
        return "";
      }
    }
    */
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
