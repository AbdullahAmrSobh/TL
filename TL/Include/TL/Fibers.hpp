#pragma once

#include <TL/Context.hpp>

#include <TL/Containers/Function.hpp>
#include <TL/Containers/Vector.hpp>

#include <atomic>

namespace TL
{
    struct TaskSchedulerConfig;
    struct TaskScheduler;
    struct Worker;
    struct TaskSet;
    struct Future;

    enum class TaskSetPriority
    {
        Low,
        Med,
        High,
        Count,
    };

    constexpr size_t   TaskSetPriorityCount = (size_t)TaskSetPriority::Count;

    TL_EXPORT uint32_t getHardwareWorkersCount();

    using TaskFunction = TL::Function<void(uint32_t start, uint32_t end, uint32_t threadnum)>;

    struct TaskSchedulerConfig
    {
        uint32_t workersCount = getHardwareWorkersCount() - 1;
    };

    struct TaskSet
    {
    };

    constexpr int sCacheLineSize = 64;

    // Producer   [ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 ]
    //                  h (consumers)   t (producer)
    template<typename Task>
    struct TaskQueue
    {
        alignas(sCacheLineSize) std::atomic_uint32_t m_head = 0;
        alignas(sCacheLineSize) std::atomic_uint32_t m_tail = 0;
        alignas(sCacheLineSize) TL::Vector<Task>     m_task = {};

        size_t capacity() const;
        size_t shirnk();


        bool push(Task task)
        {

        }

        void pop(Task* outTask)
        {

        }
    };

    struct TL_EXPORT TaskScheduler
    {
        void   init(const TaskSchedulerConfig& config);
        void   shutdown();

        // queues a task for execution, task is executed when a worker thread is empty
        Future go(TaskSet& task);
        Future go(TaskFunction& task);
        Future compute();

        // waits for task to finish
        void   wait(Future future);
        bool   ready(Future future);

    private:
        inline bool isRunning() const { return m_isRunning.load(std::memory_order_acquire); }

        void        workerNotifyReady(Worker* worker);
        bool        workerTryRunTask(Worker* worker);

        friend struct Worker;

        std::atomic_bool     m_isRunning;

        TL::Vector<Worker>   m_workers;

        std::atomic_uint16_t m_nextWorkerReady;
    };
} // namespace TL