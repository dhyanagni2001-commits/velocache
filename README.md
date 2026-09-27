
<h1 style="font-size: 60px; font-family: roboto;" align= "center">velocache</h1>

<div align = "center">
<a href = "LICENSE.md"><img src = "https://img.shields.io/github/license/tecnolgd/velocache?color=1a1a1a&style=flat-square" alt = "MIT License"></a>
<a href="https://ftp.gnu.org/gnu/gcc/"><img src="https://img.shields.io/badge/g++-13.3.0%2B-1a1a1a?style=flat-square" alt="g++ 13.3.0+"></a>
<img src = "https://img.shields.io/badge/storage-filesystem-1a1a1a?style=flat-square" alt = "Storage: Filesystem">
<a href="https://github.com/tecnolgd/velocache/releases"><img src="https://img.shields.io/github/v/release/tecnolgd/velocache?include_prereleases&color=1a1a1a&style=flat-square?" alt="Release">
<a href="https://github.com/tecnolgd/velocache/graphs/contributors"><img src="https://img.shields.io/github/contributors/tecnolgd/velocache?color=1a1a1a&style=flat-square" alt="Contributors Badge">
</a>
<a href = "#documentation"><img src = "https://img.shields.io/badge/docs-available-1a1a1a?style=flat-square" alt = "Docs"></a>
</div>
<hr>

> velocache is a high-performance, in-memory LRU (Least Recently Used) cache engine designed for $O(1)$ operations and reliable state persistence.
---


## Sample Menu

 ```txt
velocache >>>>
Cache capacity: 3
LOG: Cache loaded from the file

Operations supported
1. Data storage
2. Data retrieval
3. Cache display
4. Save Cache
5. Clear Cache
6. Exit
Select an option (1-6): 
 ```

## Key Features:

- **Constant Time Complexity:** $O(1)$ for both `get` and `put` operations via Hash-Map and Doubly Linked List integration.       
- **Smart Hydration:** Tail-to-Head file serialization ensures cache priority is preserved across system restarts.    
- **LRU Cache Invalidation:** Automatic memory management with Least Recently Used (LRU) eviction policy when capacity is reached.    
- **Persistent Storage:** Filesystem-based cache persistence for recovery after restarts, explicit termination, and supported interruption signals.
- **Runtime Configurable Capacity:** The cache max size can be set at launch with `--capacity` or `-c`.
- **Minimal Footprint:** Zero external dependencies; optimized for embedded or sidecar use cases.

## Performance data

| Operation | Throughput (ops/sec) | Latency (p50) | Latency (p95) | Latency (p99) | Latency (Avg) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **PUT** | 1.00 M  | 0.26 μs | 1.37 μs | 6.28 μs | 0.99 μs |
| **GET** | 2.53 M | 0.23 μs | 0.76 μs | 1.066 μs | 0.40 μs |

- Results from the integrated **Benchmark Suite** [**10,000 operations** for both `getValue()` and `putValue()`]
  - Comprehensive latency percentile analysis (p50, p95, p99)
  - Throughput measurements in operations per second
  - Automated CSV logging to `assets/benchmarks_log.csv` for historical tracking and trend analysis
    
    > Check the sample [CSV logging](assets/docs/usage.md#csv-logging-sample) sample for further details.

## Documentation
- [Architecture](assets/docs/architecture.md)
- [Roadmap](assets/docs/roadmap.md)
- [Tool usage](assets/docs/usage.md)

## Installation & Build

### Build using Docker

velocache is available as a lightweight pre-built container on Docker Hub.

1. Pull the image from Docker Hub    
    ```bash
    docker pull tecnolgd/velocache:latest
    ```
2. Run with Persistent Storage
    ```bash
    docker run -it --rm -v $(pwd)/assets:/app/assets tecnolgd/velocache:latest
    ```
3. Build Locally(Optional) 
    ```bash
    docker build -t velocache:local .
    ```

> [!NOTE]       
> 1. The image is built using Multi-stage compilation(zero build tools inside runtime container) using an alpine base with an image size of ~4.3 Mb.
> The *Run with persistent Storage* is done to make sure the cache states(contents in `assets/cache_data.txt`) are preserved across container restarts.
> Currently, the docker build has no option of `Runtime Configuration of Capacity` and hence it defaults to **3**.
> There is NO command for running the benchmark script as of now.

### Build from source

> [!IMPORTANT]    
> Ensure you have `g++` installed( `make` recommended). 
```bash
#clone the repo
git clone https://github.com/tecnolgd/velocache/
```
```bash
#navigate to the project directory
cd velocache
```

From the project directory, run the following commands:    

- Using make (recommended)        
    
    - Build **both** the server and the benchmark tool
        ```bash
        make
        ```
        or
        ```bash
        make all
        ```

    - Build and run individualy      
        - **The Server**:     
            ```bash
            make v_server
            ```       
            - Run with a custom cache size:
                ```bash
                ./build/v_server --capacity 10 # '-c' can also be used in place of '--capacity
                ```
            or
            - Run directly
                ```bash
                ./build/v_server # Capacity defaults to 3
                ```
    
        - **The Benchmark Tool**:
            ```bash
            make v_bench
            ```
    - Clean object files:
        ```bash
        make clean
        ```

    - Run the unit tests (requires `cmake` — `brew install cmake` on macOS):
        ```bash
        make test
        ```
        > GoogleTest is fetched automatically on first run (into `cmake-build/`, gitignored) — no manual/system install needed. See [tests/test_cache.cpp](tests/test_cache.cpp).

- Manual Build and Run         

    - The Server     
        - Build:      
            ```bash
            g++ src/cache.cpp src/storage.cpp apps/server.cpp utils/input_validation.cpp -I./include -Wall -o build/v_server  
            ```      
        - Run:         
            ```bash   
            ./build/v_server
            ```      

    - The Benchmark Tool     
        - Build:       
            ```bash
            g++ src/cache.cpp utils/metrics.cpp tests/benchmark.cpp -I./include -Wall -o build/v_bench
            ```     
        - Run:     
            ```bash
            ./build/v_bench
            ```
            >For more details, check [Benchmark usage](assets/docs/usage.md#the-benchmark-tool)


## [Contributing](CONTRIBUTING.md#contributing-to-velocache)

## Contributors
A huge thanks to the developers contributing to velocache.     

- [nvphungdev](https://github.com/nvphungdev)

## Author & License

- **Author:** tecnolgd   
- **License:** [MIT License](LICENSE.md)
