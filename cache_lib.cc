#include "cache.hh"
#include "evictor.hh"

using namespace std;


class Impl {

  size_type maxmem;
  float max_load_factor;
  Evictor* evictor;
  hash_func hasher;

  Impl(size_type maxmem,
      float max_load_factor,
      Evictor* evictor,
      hash_func hasher) maxmem(maxmem), max_load_factor(max_load_factor), evictor(evictor), hasher(hasher)
      {}

  ~Impl() = default;

  void set(key_type key, val_type val, size_type size) {}

  val_type get(key_type key, size_type& val_size) const;

  bool del(key_type key);

  size_type space_used() const;

  void reset();
}

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
