// Unit tests for the LRU Cache core logic (see include/cache.hpp, src/cache.cpp,
// src/storage.cpp). Built and run with GoogleTest — see the root CMakeLists.txt
// for how the dependency is fetched, and `make test` for how to run these.
//
// Only the public API of Cache is exercised here (putValue, getValue, contains,
// store_cache_data, load_from_file, clear_cache) since the Node/DLL internals
// are private implementation details.

#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "cache.hpp"

namespace {
const std::string kCacheFile = "assets/cache_data.txt";

// Reads the whole cache_data.txt file into a string (empty if missing).
std::string readCacheFile() {
    std::ifstream in(kCacheFile);
    std::ostringstream contents;
    contents << in.rdbuf();
    return contents.str();
}
}  // namespace

// ---------------------------------------------------------------------------
// put/get basics
// ---------------------------------------------------------------------------

TEST(PutGet, ReturnsTheValueThatWasStored) {
    Cache cache(3);
    cache.putValue("name", "velocache");

    EXPECT_EQ(cache.getValue("name"), "velocache");
}

TEST(PutGet, GettingAMissingKeyReportsNotFound) {
    Cache cache(3);

    EXPECT_EQ(cache.getValue("missing"), "Value NOT found!\n");
}

TEST(PutGet, PuttingAnExistingKeyOverwritesItsValue) {
    Cache cache(3);
    cache.putValue("key", "old-value");
    cache.putValue("key", "new-value");

    EXPECT_EQ(cache.getValue("key"), "new-value");
}

TEST(PutGet, ContainsReflectsWhatWasActuallyStored) {
    Cache cache(3);
    cache.putValue("key", "value");

    EXPECT_TRUE(cache.contains("key"));
    EXPECT_FALSE(cache.contains("missing"));
}

// ---------------------------------------------------------------------------
// LRU eviction policy
// ---------------------------------------------------------------------------

TEST(Eviction, DropsTheLeastRecentlyUsedKeyWhenCapacityIsExceeded) {
    Cache cache(2);
    cache.putValue("a", "1");
    cache.putValue("b", "2");
    cache.putValue("c", "3");  // capacity is 2, so "a" (oldest) should be evicted

    EXPECT_FALSE(cache.contains("a"));
    EXPECT_TRUE(cache.contains("b"));
    EXPECT_TRUE(cache.contains("c"));
}

TEST(Eviction, ReadingAKeyRefreshesItsRecencySoItSurvivesEviction) {
    Cache cache(2);
    cache.putValue("a", "1");
    cache.putValue("b", "2");
    cache.getValue("a");       // "a" is now the most recently used
    cache.putValue("c", "3");  // "b" is now the least recently used, not "a"

    EXPECT_TRUE(cache.contains("a"));
    EXPECT_FALSE(cache.contains("b"));
    EXPECT_TRUE(cache.contains("c"));
}

TEST(Eviction, UpdatingAnExistingKeyAlsoRefreshesItsRecency) {
    Cache cache(2);
    cache.putValue("a", "1");
    cache.putValue("b", "2");
    cache.putValue("a", "1-updated");  // re-putting "a" should count as a recent use
    cache.putValue("c", "3");

    EXPECT_TRUE(cache.contains("a"));
    EXPECT_FALSE(cache.contains("b"));
}

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------

TEST(EdgeCases, ZeroCapacityCacheStoresNothingAndDoesNotCrash) {
    Cache cache(0);
    cache.putValue("x", "1");  // regression test: this used to segfault

    EXPECT_FALSE(cache.contains("x"));
    EXPECT_EQ(cache.getValue("x"), "Value NOT found!\n");
}

// ---------------------------------------------------------------------------
// File I/O: store_cache_data() / load_from_file() / clear_cache()
// ---------------------------------------------------------------------------

TEST(FileIO, StoreThenLoadRestoresAllEntries) {
    {
        Cache writer(3);
        writer.putValue("a", "1");
        writer.putValue("b", "2");
        writer.store_cache_data();
    }

    Cache reader(3);
    reader.load_from_file();

    EXPECT_EQ(reader.getValue("a"), "1");
    EXPECT_EQ(reader.getValue("b"), "2");
}

TEST(FileIO, ClearCacheEmptiesBothMemoryAndTheFile) {
    Cache cache(3);
    cache.putValue("a", "1");
    cache.store_cache_data();

    cache.clear_cache();

    EXPECT_FALSE(cache.contains("a"));
    EXPECT_EQ(readCacheFile(), "");
}

// ---------------------------------------------------------------------------
// Custom entry point: back up and restore the developer's real
// assets/cache_data.txt so running the test suite doesn't clobber their
// locally persisted cache (the FileIO tests above write to that same path).
// ---------------------------------------------------------------------------

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    bool hadExistingFile = static_cast<bool>(std::ifstream(kCacheFile));
    std::string backup = readCacheFile();

    int result = RUN_ALL_TESTS();

    if (hadExistingFile) {
        std::ofstream out(kCacheFile, std::ios::trunc);
        out << backup;
    }

    return result;
}
