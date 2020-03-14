// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <iostream>
#include <cassert>
#include <typeinfo>
#include <cstring>

#include "evictor.hh"
#include "cache.hh"
#include "fifo_evictor.hh"

#include "cache_lib.cc"   // for testing purpose

using namespace std;



TEST_CASE ("Cache with nullptr Evictor", "[cache]") {
  Cache cache(8, 0.75, nullptr);
  REQUIRE(cache.space_used() == 0);

  const Cache::val_type val1 = "1";
  const Cache::val_type val2 = "2";
  const Cache::val_type val3 = "16";
  const Cache::val_type val4 = "17";
  const Cache::val_type val5 = "18";

  Cache::size_type size;

  SECTION( "should not get a key that wasn't inserted" ) {
    REQUIRE(cache.get("k1", size) == NULL);
    REQUIRE(size == 0);
  }

  SECTION( "should get a key that was inserted" ) {
    cache.set("k1", val1, strlen(val1)+1);

    REQUIRE(strcmp(cache.get("k1", size), val1) == 0);   // string comparision
    REQUIRE(size == strlen(val1)+1);

    // inserting extra keys should increment memory used correctly
    cache.set("k2", val2, strlen(val2)+1);

    REQUIRE(strcmp(cache.get("k2", size), val2) == 0);
    REQUIRE(size == strlen(val2)+1);
    REQUIRE(cache.space_used() == 4);

    // should not insert any more key when maxmem has been exceeded
    cache.set("k3", val3, strlen(val3)+1);

    REQUIRE(strcmp(cache.get("k3", size), val3) == 0);
    REQUIRE(size == strlen(val3)+1);
    REQUIRE(cache.space_used() == 7);

    cache.set("k4", val4, strlen(val4)+1);

    REQUIRE(cache.get("k4", size) == NULL);
    REQUIRE(size == 0);
    REQUIRE(cache.space_used() == 7);
  }

  SECTION( "should get a key that was inserted and modified" ) {
    cache.set("k3", val3, strlen(val3)+1);

    REQUIRE(strcmp(cache.get("k3", size), val3) == 0);
    REQUIRE(size == strlen(val3)+1);
    REQUIRE(cache.space_used() == size);

    // modify value of key 3
    cache.set("k3", val4, strlen(val4)+1);

    REQUIRE(strcmp(cache.get("k3", size), val4) == 0);
    REQUIRE(size == strlen(val4)+1);
    REQUIRE(cache.space_used() == strlen(val4)+1);
  }

  SECTION( "should not get a key that was inserted and deleted" ) {
    cache.set("k1", val1, strlen(val1)+1);

    REQUIRE(strcmp(cache.get("k1", size), val1) == 0);   // string comparision
    REQUIRE(size == strlen(val1)+1);
    REQUIRE(cache.space_used() == 2);

    cache.del("k1");
    REQUIRE(cache.get("k4", size) == NULL);
    REQUIRE(size == 0);
    REQUIRE(cache.space_used() == 0);
  }

  SECTION( "resetting should return empty cache" ) {
    cache.set("k1", val1, strlen(val1)+1);
    cache.set("k2", val2, strlen(val2)+1);

    cache.reset();
    REQUIRE(cache.space_used() == 0);
  }
}

// void testCacheBasic_wo_Evictor() {
//   cout<< endl<<"Beginning test for cache with no evictor..."<<endl;
//   Cache cache(0, 0.75, nullptr);
//   const Cache::val_type val1 = "1";
//   Cache::size_type size;
//   cout << "Inserting key 1 of size " << strlen(val1)+1 << " bytes" << endl;
//   cache.set("k1", val1, strlen(val1)+1);
//   cout << "Attempting to retrieve key 1 (should not have been inserted)... " << endl;
//   assert(cache.get("k1", size) == NULL);
//   assert(size == 0);
//   cout << "Test passed for no-evictor memory overflow" << endl;
// }
//
// void testCacheBasic_w_Evictor() {
//   FifoEvictor* evictor= new FifoEvictor();
//   Cache basic_cache(8, 0.75, evictor);
//
//   const Cache::val_type val1 = "1";
//   const Cache::val_type val2 = "2";
//   const Cache::val_type val3 = "16";
//   const Cache::val_type val4 = "17";
//   const Cache::val_type val5 = "18";
//
//   assert(basic_cache.space_used() == 0);  // cache is initialliy empty
//   cout << "Test empty cache passed" << endl;
//
//   // Insert items in the cache
//   cout << "Inserting key 1 of size " << strlen(val1)+1 << " bytes" << endl;
//   basic_cache.set("k1", val1, strlen(val1)+1);
//
//   cout << "Inserting key 2 of size " << strlen(val2)+1 << " bytes" << endl;
//   basic_cache.set("k2", val2, strlen(val2)+1);
//
//   assert(basic_cache.space_used() == 4);
//   cout << "Test insert 2 items passed" << endl;
//
//   cout << "Inserting key 3 of size " << strlen(val3)+1 << " bytes" << endl;
//   basic_cache.set("k3", val3, strlen(val3)+1);
//
//   assert(basic_cache.space_used() == 7);
//   cout << "Test insert 3 items passed" << endl;
//
//   /* Test get function */
//   Cache::size_type size;
//
//   // Getting a key that was inserted
//   cout << "Retrieving key 1... " << endl;
//   assert(strcmp(basic_cache.get("k1", size), val1) == 0);
//   cout << "Test get key 1 passed" << endl;
//
//
//   cout << "Attempting to retrieve key 4 (not yet in cache)... " << endl;
//   assert(basic_cache.get("k4", size) == NULL);
//   assert(size == 0);
//   cout << "Test retrieve item not in cache passed" << endl;
//
//   // Getting a key that was inserted and modified
//   cout << "Inserting value of size " << strlen(val3)+1 << " bytes at key 3" << endl;
//   basic_cache.set("k3", val4, strlen(val4)+1);
//   cout<< "Space used is: " << basic_cache.space_used() <<endl;
//   assert(basic_cache.space_used() == 7);
//
//   cout << "Retrieving key 3... " << endl;
//   assert(strcmp(basic_cache.get("k3", size), val4) == 0);
//   cout << "Test get key after its value has been changed passed" << endl;
//
//   //Testing eviction after memory overflow AND
//   //querying for both evicted value and newly added value:
//
//   cout<<"Adding k-v pair key 5..." <<endl;
//   basic_cache.set("k5", val5, strlen(val5)+1);
//   cout << "Attempting to retrieve key 1 (evicted from cache)... " << endl;
//   assert(basic_cache.get("k1", size) == nullptr);
//   assert(basic_cache.space_used() == 8);
//   cout << "Test passed for evicting item from cache" << endl;
//
//   cout<<"Attempting to retrieve key 5... " <<endl;
//   assert(strcmp(basic_cache.get("k5", size), val5) == 0);
//   cout<< "Test passed for an insertion that uses eviction. "<<endl;
//
//
//   //Testing Deletion, and querying after deletion:
//   cout<<"Deleting key 4... "<<endl;
//   basic_cache.del("k4");
//   cout << "Attempting to retrieve key 4 (deleted from cache)... "  << endl;
//   assert(basic_cache.get("k4", size) == NULL);
//   assert(size == 0);
//   cout <<"Test passed for querying after deletion" << endl;
//
//   // Testing reset function
//   cout<<"Reseting the Cache..."<<endl;
//   basic_cache.reset();
//   assert(basic_cache.space_used()== 0);
//   assert(basic_cache.get("k4",size)== NULL);
//   cout<<"Test passed for reseting the cache."<<endl;
// }

// int main() {
//   testCacheBasic_w_Evictor();
//   testCacheBasic_wo_Evictor();
//   return 0;
// }
