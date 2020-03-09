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


### Part 2: Testing
(list a bunch of tests that we did)

### Part 3: Performance
We implement [Rolling Hashing](https://en.wikipedia.org/wiki/Rolling_hash#Rabin-Karp_rolling_hash) for string hashing. We implement the algorithm as described here: https://cp-algorithms.com/string/string-hashing.html


### Part 4: Collision resolution
We let **C++ standard unordered_map** handle collisions: **rehashes** are automatically performed by the container whenever its **load factor** is going to surpass its **max_load_factor** in an operation. The load factor is defined as *the ratio between the number of elements in the container (its size) and the number of buckets (bucket_count)*. We override the default max_load_factor (1.0) by unordered_map to the custom value 0.75. The container uses the value of max_load_factor as the threshold that forces an increase in the number of buckets (and thus causing a rehash).


### Part 5: Eviction policy
We implemented First-In-First-Out (FIFO) eviction policy using the C++ standard queue.
