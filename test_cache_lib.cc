#include <iostream>
#include <cassert>
#include <typeinfo>

#include "evictor.hh"
#include "cache_lib.cc"
#include "fifo_evictor.cc"

using namespace std;


void testCacheBasic() {
  FifoEvictor* evictor= new FifoEvictor();
  Cache basic_cache(8, 0.75, evictor);

  const Cache::val_type val1 = "a";
  const Cache::val_type val2 = "2";
  const Cache::val_type val3 = "16";
  const Cache::val_type val4 = "17";
  const Cache::val_type val5 = "18";

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

  // cout << "Retrieving key 1... " << basic_cache.get("k1", size) << endl;

  // assert(basic_cache.get("k1", size) == "1");

  // cout << "Test get key 1 passed" << endl;

  cout << "Attempting to retrieve key 4 (not yet in cache)... " << endl;
  basic_cache.get("k4", size);
  assert(size == 0);
  cout << "Test retrieve item not in cache passed" << endl;

  cout << "Test for retreiving unentered key passed" << endl;
  //Change a key already in cache
  cout << "Inserting val4  of size " << strlen(val3)+1 << " bytes at key 3" << endl;
  basic_cache.set("k3", val4, strlen(val4)+1);
  assert(basic_cache.space_used() == 7);

  //Show the value changed
  cout << "Retrieving key 3... " << basic_cache.get("k3", size) << endl;

  assert(basic_cache.get("k3", size) == "17");

  cout << "Test get key 1 passed" << endl;

  //TODO: show the memory at old address is now deleted?

  // Does this do it? cout<< "Viewing memory at val4... "<< '"'<<val4<<'"'<<endl;
  // Corresponding assertion? : assert(val4=="");


  //Testing Deletion and querying after deletion:
  cout<<"Deleting key 4... "<<endl;
  basic_cache.del("k4");
  cout << "Attempting to retreive key 4 (deleted from cache)... " << '"'<<basic_cache.get("k4", size) <<'"'<< endl;
  assert(basic_cache.get("k4", size) == "");
  cout <<"Test passed for querying after deletion"<<endl;


  //TODO:
  //Testing eviction after memory overflow AND
  //querying for both evicted value and newly added value:







}

int main(int argc, char const *argv[]) {
  testCacheBasic();
  return 0;
}
