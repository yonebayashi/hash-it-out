# HW2: Hash it out

**Authors: David Tamas-Parris, Hien Nguyen**

#### Structure of this repo
`cache.hh`: Header file for Cache

`cache_lib.cc`: Implementation file for Cache

`test_cache_lib.cc`: Unit testing for all Cache functionality

`evictor.hh`: Header file for Evictor

`fifo_evictor.hh`: Header file for FIFO Evictor, derived class from Evictor.

`fifo_evictor.cc`: Implementation file for FIFO Evictor.

To compile:

```
make
```
Then run

```
./test_cache_lib
```

### Part 1: Basic cache operations


We implemented basic cache functionality including
- `get`: retrieves an existing key from the cache
- `set`: inserts or modifies existing keys
- `del` : removes an item from the cache
- `space_used`: returns the total memory used by all cache values (not keys)
- `reset`: deletes all data from the cache

std::unordered_map was used as the underlying hash table, hashing keys to items of type val_type. Care was taken to manage memory. `set` copies memory controlled by the user into a new address controlled by the cache. The memory is freed when the k-v pair is deleted or the value changes. 


### Part 2: Testing
We test all cache functionality for both when the Evictor is a `nullptr` and when the Evictor is a FIFO Evictor.

We test for the following cases:
- Insert a key-value pair into the cache when `maxmem` is not exceeded :white_check_mark:
- Insert a key-value pair into the cache when `maxmem` has been exceeded :white_check_mark:
- Getting a key-value pair that was inserted from the cache :white_check_mark:
- Getting a key-value pair that was not inserted from the cache :white_check_mark:
- Getting a key-value pair that was inserted and modified from the cache :white_check_mark:
- Getting a key-value pair that was inserted and deleted from the cache :white_check_mark:
- Attempting to access a k-v pair after it should be evicted in another key's insertion results in a nullptr. :white_check_mark:
- `memused` updates as expected after an insertion that should involve an eviction. :white_check_mark:
- Resetting the cache drops  `memused` to 0 and querying previously inserted keys returns a nullptr. :white_check_mark:

### Part 3: Performance
As mentioned previously, std::unordered_map is used as the underlying hash table. All operations do indeed run in asymptotic constant time. 

We implement [Rolling Hashing](https://en.wikipedia.org/wiki/Rolling_hash#Rabin-Karp_rolling_hash) for string hashing. We implement the algorithm as described here: https://cp-algorithms.com/string/string-hashing.html

### Part 4/5: Collision resolution & Dynamic Resizing
We let **C++ standard unordered_map** handle collisions. This was done for a few reasons. First, it meant one less thing to implement, and was sure to be optimized beyond anything we could practically accomplish. Second, its collision handling (collision chaining) is a perfectly acceptable policy for our purposes, especially since we require the ability to change k-v pairs (this takes an unacceptable performance toll with other collision-handling policies like quadratic/linear probing). 



We also allow std::unordered_map to handle resizing. **rehashes** into a table with more buckets are automatically performed by the container whenever its **load factor** is going to surpass its **max_load_factor** in an operation. The load factor is defined as *the ratio between the number of elements in the container (its size) and the number of buckets (bucket_count)*. We override the default max_load_factor (1.0) by unordered_map to the custom value 0.75. 



### Part 5: Eviction policy
We implemented First-In-First-Out (FIFO) eviction policy using std::queue. Touching a key adds a key to the queue, and evicting a key involves popping a key from the queue (the cache implementation then attempts to delete the corresponding k-v pair). 
