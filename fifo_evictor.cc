#include <queue>

#include "evictor.hh"
#include "fifo_evictor.hh"


FifoEvictor::FifoEvictor(std::queue<key_type>& key_queue) : key_queue(key_queue) {}

void FifoEvictor::touch_key(const key_type& key) {
	key_queue.push(key);
	return;
}

const key_type FifoEvictor::evict()  {
  if (key_queue.empty()) {
    return "";
  }
  key_type evicted = key_queue.front();
  key_queue.pop();
  return evicted->first;
}


