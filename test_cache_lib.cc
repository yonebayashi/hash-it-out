#include <iostream>
#include <cassert>
#include <typeinfo>
#include <cstring>

#include "evictor.hh"
#include "cache.hh"
#include "fifo_evictor.hh"
//#include "cache_lib.cc"
//#include "fifo_evictor.cc"

using namespace std;

void testCacheBasic_wo_Evictor() {
  Cache cache(0, 0.75, nullptr);
  const Cache::val_type val1 = "1";
  Cache::size_type size;
  cout << "Inserting key 1 of size " << strlen(val1)+1 << " bytes" << endl;
  cache.set("k1", val1, strlen(val1)+1);
  cout << "Attempting to retrieve key 1 (should not have been inserted)... " << endl;
  assert(cache.get("k1", size) == NULL);
  assert(size == 0);
  cout << "Test passed for no-evictor memory overflow" << endl;



}

void testCacheBasic_w_Evictor() {
  FifoEvictor* evictor= new FifoEvictor();
  Cache basic_cache(8, 0.75, evictor);

  const Cache::val_type val1 = "1";
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

  // TODO: Test should stop accepting new values when maxmem is exceeded

  // TODO: Test get function
  Cache::size_type size;

  cout << "Retrieving key 1... " << endl;
  assert(strcmp(basic_cache.get("k1", size), val1) == 0);
  cout << "Test get key 1 passed" << endl;


  cout << "Attempting to retrieve key 4 (not yet in cache)... " << endl;
  assert(basic_cache.get("k4", size) == NULL);
  assert(size == 0);
  cout << "Test retrieve item not in cache passed" << endl;

  //Change a key already in cache
  cout << "Inserting value of size " << strlen(val3)+1 << " bytes at key 3" << endl;
  basic_cache.set("k3", val4, strlen(val4)+1);
  cout<<"Space used is: "<<basic_cache.space_used()<<endl;
  assert(basic_cache.space_used() == 7);
  cout <<"Test for changing an existing k-v pair passed"<<endl;

  //Show the value changed
  cout << "Retrieving key 3... " << endl;
  assert(strcmp(basic_cache.get("k3", size), val4) == 0);
  cout << "Test get key after its value has been changed passed" << endl;

  //TODO:
  //Testing eviction after memory overflow AND
  //querying for both evicted value and newly added value:

  cout<<"Adding k-v pair k5: '18'... "<<endl;
  basic_cache.set("k5", val5, 3);
  cout << "Attempting to retrieve key 1 (evicted from cache)... " << endl;
  assert(basic_cache.get("k1", size) == nullptr);
  assert(basic_cache.space_used() == 8);
  cout << "Test passed for evicting item from cache" << endl;

  cout<<"Attempting to retreive k5... "<<  basic_cache.get("k5", size)<<endl;
  //assert(basic_cache.get("k5", size) == "5");
  cout<< "Test passed for an insertion that uses eviction. "<<endl;

  //TODO: show the memory at old address is now deleted?

  // Does this do it? cout<< "Viewing memory at val4... "<< '"'<<val4<<'"'<<endl;
  // Corresponding assertion? : assert(val4=="");


  //Testing Deletion, and querying after deletion:
  cout<<"Deleting key 4... "<<endl;
  basic_cache.del("k4");
  cout << "Attempting to retrieve key 4 (deleted from cache)... "  << endl;
  assert(basic_cache.get("k4", size) == NULL);
  assert(size == 0);
  cout <<"Test passed for querying after deletion" << endl;







}

int main() {
  testCacheBasic_w_Evictor();
  testCacheBasic_wo_Evictor();
  return 0;
}
