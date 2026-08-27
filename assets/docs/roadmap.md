## Roadmap & Future Upgrades

- ✓ **Current Implementation:** 

  - C++ core with modular architecture   
  - O(1) LRU cache using hash map and doubly linked list
  - Filesystem-based persistent storage (*tail-to-head* serialization)
  - LRU cache invalidation strategy with configurable capacity
  - Linux system support
  - Makefile build system with modular compilation
  - Comprehensive benchmark suite with CSV logging for performance tracking:
    - `p50`, `p95`, `p99` latency percentiles
    - Throughput measurements (ops/sec)
    - Automated results persistence to `benchmarks_log.csv`
    - Historical trend analysis capabilities
  - Sample GIF examples for the benchmark and server programs
  - Duplicate key detection function with overwrite/skip option
  - `Clear Cache` function to clear the entire cache
  - File corruption handling while loading data into the cache
  - Runtime Cache capacity flag 
  - Docker build support

- ◴ **In Progress:** 
  - Robustness improvements and edge-case handling
  - Performance optimization opportunities
  
- ◌ **Planned:**
  - Network server capabilities (client-server architecture)
  - Cross-platform support (macOS, Windows)
  - Remote access and distributed caching
  - System monitoring stats (CPU, memory, network)
  - Plugin architecture support
  - Additional storage backends evaluation
  - RAII/smart pointer implementation
