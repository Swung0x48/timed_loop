#include <iostream>
#include "timed_loop.hpp"
int main() {
    timed_loop loop(std::chrono::milliseconds(1000),
                    []() { std::cout << std::chrono::system_clock::now().time_since_epoch().count() << std::endl; },
                    true);
    //loop.run();
    int a;
    std::cin >> a;
    loop.stop();
    std::cin >> a;
    return 0;
}
