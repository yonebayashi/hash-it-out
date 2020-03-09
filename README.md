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
g++ -std=c++17 test_cache_lib.cc
```
Then run

```
./a.out
```

### Part 1: Basic cache operations

We implement basic cache functionality including
- `get`: retrieves an existing key from the cache
- `set`: inserts or modifies existing keys
- `del` : removes an item from the cache
- `space_used`: returns the total memory used by all cache values (not keys)
- `reset`: deletes all data from the cache


### Part 2: Testing
We test all cache functionality for both when the Evictor is a `nullptr` and when the Evictor is a FIFO Evictor.

We test for the following cases:
- Insert a key-value pair into the cache when `maxmem` is not exceeded :white_check_mark:
- Insert a key-value pair into the cache when `maxmem` has been exceeded :white_check_mark:
- Getting a key-value pair that was inserted from the cache :white_check_mark:
- Getting a key-value pair that was not inserted from the cache :white_check_mark:
- Getting a key-value pair that was inserted and modified from the cache :white_check_mark:
- Getting a key-value pair that was inserted and deleted from the cache :white_check_mark:

### Part 3: Performance
We implement [Rolling Hashing](https://en.wikipedia.org/wiki/Rolling_hash#Rabin-Karp_rolling_hash) for string hashing. We implement the algorithm as described here: https://cp-algorithms.com/string/string-hashing.html


### Part 4: Collision resolution
We let **C++ standard unordered_map** handle collisions: **rehashes** are automatically performed by the container whenever its **load factor** is going to surpass its **max_load_factor** in an operation. The load factor is defined as *the ratio between the number of elements in the container (its size) and the number of buckets (bucket_count)*. We override the default max_load_factor (1.0) by unordered_map to the custom value 0.75. The container uses the value of max_load_factor as the threshold that forces an increase in the number of buckets (and thus causing a rehash).


### Part 5: Eviction policy
We implemented First-In-First-Out (FIFO) eviction policy using the C++ standard queue.
