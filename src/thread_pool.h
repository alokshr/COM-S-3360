#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <atomic>

/**
 * A class for handling parallel threads and tasks
 */
class thread_pool {
    public:
        /**
         * Creates a thread pool
         * @param num_threads number of threads to use
         */
        thread_pool(size_t num_threads = std::thread::hardware_concurrency()) : stop(false), total_tasks(0), completed_tasks(0) {
            // Create threads
            for (size_t i = 0; i < num_threads; i++) {
                threads.emplace_back([this] {
                    while (true) {
                        // Grabbing a task
                        std::function<void()> task;
                        {
                            // Lock the task queue to safely grab data
                            std::unique_lock<std::mutex> lock(mutex);

                            // Wait till a new task is available or the thread pool is stopped
                            condition_variable.wait(
                                lock,
                                [this]{ return stop || !tasks.empty(); }
                            );
                            
                            // Exit thread if thread pool is stopped and no more tasks are available
                            if (stop && tasks.empty()) return;

                            // Grab next task
                            task = std::move(tasks.front());
                            this->tasks.pop();
                        }

                        // Perform task
                        task();
                        completed_tasks++;
                    }
                });
            }
        }

        /**
         * Destroys and cleans up this thread pool
         */
        ~thread_pool() {
            {
                // Lock the task queue to update the stop flag safely
                std::unique_lock<std::mutex> lock(mutex);
                stop = true;
            }

            // Notify all threads of an update to the thread pool
            condition_variable.notify_all();

            // Join threads
            for (auto& thread : threads) {
                thread.join();
            }
        }

        /**
         * Adds a task to this thread pool
         * @param task function/task to run 
         */
        void enqueue(std::function<void()> task) {
            {
                std::unique_lock<std::mutex> lock(mutex);
                tasks.emplace(move(task));
                total_tasks++;
            }
            condition_variable.notify_one();
        }

        /**
         * Returns the percentage of tasks completed
         * @return percentage of tasks completed
         */
        double get_progress_percent() {
            return (total_tasks == 0) ? 1 : static_cast<double>(completed_tasks)/total_tasks;
        }

    private:
        mutable std::mutex mutex;
        std::condition_variable condition_variable;
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> tasks;
        bool stop;
        int total_tasks;
        std::atomic<int> completed_tasks;
};
#endif