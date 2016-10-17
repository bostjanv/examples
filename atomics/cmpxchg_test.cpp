#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>
#include <cstdio>

inline uint8_t get_next_cnt(std::atomic<uint8_t>& cnt)
{
    uint8_t expected = cnt.load();
    uint8_t desired;

    do {
        if (expected == 7) {
            desired = 1;
        }
        else {
            desired = expected + 1;
        }
    }
    while (!cnt.compare_exchange_weak(expected, desired));

    return desired;
}

/*
static void foo(std::atomic<uint8_t>& cnt)
{
    for (;;) {
        uint8_t c = get_next_cnt(cnt);
        assert(c >= 1 && c <= 7);
        std::cout << (int)c << std::endl;
    }
}
*/

#define N 4
#define M 10000000

int main() {
    std::atomic<uint8_t> cnt = 0;
    assert(cnt.load() == 0);

    std::thread t[N];

    for (int i = 0; i < N; i++) {
        t[i] = std::thread([&cnt]() {
            for (int i = 0; i < M; i++) {
                uint8_t c = get_next_cnt(cnt);
                if (c == 0 || c > 7) abort();
                assert(c >= 1 && c <= 7);
                //std::cout << (int)c << std::endl;
                //printf("%d\n", c);
            }
        });
    }

#if 0
    for (;;) {
        uint8_t c = get_next_cnt(cnt);
        if (c == 0 || c > 7) abort();
        assert(c >= 1 && c <= 7);
        //std::cout << (int)c << std::endl;
        //printf("%d\n", c);
    }
#endif

    for (int i = 0; i < N; i++) {
        t[i].join();
    }

    //std::thread t(foo, std::ref(cnt));
    //t.join();
}
