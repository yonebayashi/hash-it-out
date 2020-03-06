#include <queue>

#include "evictor.hh"


class FifoEvictor : class Evictor {
private:
  std::queue<key_type> key_list;

public:
  FifoEvictor(std::queue<key_type>& key_list);
  ~FifoEvictor() = default;

  // Disallow copies
  FifoEvictor(const FifoEvictor&) = delete;
  FifoEvictor& operator=(const FifoEvictor&) = delete;

  void touch_key(const key_type&);
  const key_type evict();
  };
}
