#include <iostream>
#include <cassert>
#include <typeinfo>

#include "evictor.hh"
#include "cache_lib.cc"

using namespace std;


void testCacheBasic() {
  Cache basic_cache(8);

  const Cache::val_type val1 = "1";
  const Cache::val_type val2 = "2";
  const Cache::val_type val3 = "16";

  assert(basic_cache.space_used() == 0);  // cache is initialliy empty
  cout << "Test empty cache passed" << endl;

  // Insert items in the cache
  cout << "Inserting key 1 of size " << strlen(val1)+1 << " bytes" << endl;
  basic_cache.set("k1", val1, strlen(val1)+1);

  cout << "Inserting key 2 of size " << strlen(val2)+1 << " bytes" << endl;
  basic_cache.set("k2", val2, strlen(val2)+1);

  assert(basic_cache.space_used() == 4);
  cout << "Test insert 2 items passed" << endl;

  cout << "Inserting key 3 of size " << strlen(val3)+1 << " bytes" << endl;
  basic_cache.set("k3", val3, strlen(val3)+1);

  assert(basic_cache.space_used() == 7);
  cout << "Test insert 3 items passed" << endl;

  // TODO: Test should evict the first item to make space for the new one

  // TODO: Test should stop accepting new values when maxmem is exceeded

  // TODO: Test get function
  Cache::size_type size;

  cout << "Retrieving key 1... " << basic_cache.get("k1", size) << endl;

  assert(basic_cache.get("k1", size) == "1");
  cout << "Test get key 1 passed" << endl;
}

int main(int argc, char const *argv[]) {
  testCacheBasic();
  return 0;
}
