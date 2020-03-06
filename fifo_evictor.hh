#include <queue>

#include "evictor.hh"


class FifoEvictor : public Evictor {
private:
  std::queue<key_type> key_queue;

public:
  FifoEvictor(std::queue<key_type>& key_queue);
  ~FifoEvictor() = default;

  // Disallow copies
  FifoEvictor(const FifoEvictor&) = delete;
  FifoEvictor& operator=(const FifoEvictor&) = delete;

  void touch_key(const key_type& key); //Is it kosher to pass an argument here?
  const key_type evict();
};
