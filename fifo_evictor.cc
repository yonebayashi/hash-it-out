#include <queue>

#include "evictor.hh"
#include "fifo_evictor.hh"


FifoEvictor::FifoEvictor(std::queue<key_type>& key_list) : key_list(key_list) {}

void FifoEvictor::touch_key(const key_type&) {}

const key_type FifoEvictor::evict()  {
  if (key_list.empty()) {
    return "";
  }
  key_type evicted = key_list.front();
  key_list.pop();
  return evicted->first;
}
