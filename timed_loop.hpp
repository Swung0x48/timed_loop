#ifndef TIMED_EVENTLOOP_TIMED_EVENTLOOP_H
#define TIMED_EVENTLOOP_TIMED_EVENTLOOP_H

#include <thread>
#include <utility>

class timed_loop {
private:
    std::thread thread_;
    std::chrono::time_point<std::chrono::steady_clock> start_time_;
    std::chrono::time_point<std::chrono::steady_clock> end_time_;
    std::chrono::nanoseconds delta_;
    std::function<void()> callback_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool running_ = false;
    bool finished_ = false;
public:
    void run() {
        running_ = true;
        cv_.notify_one();
    }

    void stop() {
        finished_ = true;
        if (thread_.joinable())
            thread_.join();
    }

    template <typename Rep, typename Period>
    timed_loop(std::chrono::duration<Rep, Period> duration, std::function<void()> callback, bool start_immediately = false):
        delta_(duration),
        callback_(std::move(callback)) {

        thread_ = std::thread([this]() {
            while (!finished_) {
                std::unique_lock<std::mutex> lk(mtx_);
                while (!running_)
                    cv_.wait(lk);

                start_time_ = std::chrono::steady_clock::now();
                end_time_ = start_time_ + delta_;

                callback_();

                std::this_thread::sleep_until(end_time_);
            }
        });

        if (start_immediately)
            run();
    }

    timed_loop(const timed_loop&) = delete;
};


#endif //TIMED_EVENTLOOP_TIMED_EVENTLOOP_H
