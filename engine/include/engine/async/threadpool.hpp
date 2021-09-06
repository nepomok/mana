/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef ENGINE_THREADPOOL_HPP
#define ENGINE_THREADPOOL_HPP

#include <memory>
#include <vector>
#include <thread>
#include <functional>
#include <map>
#include <cassert>

#include "engine/async/task.hpp"

namespace engine {
    class ThreadPool {
    public:
        static ThreadPool pool;

        explicit ThreadPool(int numberOfThreads = 4) : isShutdown(false) {
            assert(numberOfThreads > 0);
            for (int i = 0; i < numberOfThreads; i++) {
                threads.emplace_back(std::thread([this]() { pollTasks(); }));
            }
        }

        ~ThreadPool() {
            shutdown();
            for (auto &thread : threads) {
                thread.join();
            }
        }

        std::shared_ptr<Task> addTask(const std::function<void()> &work) {
            if (isShutdown)
                throw std::runtime_error("Thread pool was shut down");

            std::shared_ptr<Task> ret;
            {
                const std::lock_guard<std::mutex> l(taskMutex);
                int index;
                if (taskCache.empty()) {
                    if (taskIndex == std::numeric_limits<int>::max())
                        throw std::runtime_error("Maximum concurrent tasks reached");
                    index = taskIndex++;
                } else {
                    index = *taskCache.begin();
                    taskCache.erase(taskCache.begin());
                }
                ret = std::make_shared<Task>(work);
                tasks[index] = ret;
            }

            taskVar.notify_all();

            return ret;
        }

        void shutdown() {
            isShutdown = true;
            taskVar.notify_all();
        }

    private:
        std::mutex taskMutex;

        int taskIndex = 0;
        std::vector<int> taskCache;
        std::map<int, std::shared_ptr<Task>> tasks;
        std::condition_variable taskVar;

        std::vector<std::thread> threads;

        std::atomic<bool> isShutdown;

        void pollTasks() {
            std::unique_lock<std::mutex> taskLock(taskMutex);
            while (!isShutdown) {
                if (!tasks.empty()) {
                    auto task = tasks.begin()->second;
                    taskCache.emplace_back(tasks.begin()->first);

                    tasks.erase(tasks.begin());

                    taskLock.unlock();
                    task->start();
                    taskLock.lock();
                }
                taskVar.wait(taskLock, [this] {
                    if (isShutdown)
                        return true;
                    return !tasks.empty();
                });
            }
        }
    };
}

#endif //ENGINE_THREADPOOL_HPP
