#include "TL/Fibers.hpp"

#include <thread>

namespace TL
{
    uint32_t getHardwareWorkersCount()
    {
        return std::thread::hardware_concurrency();
    }

    struct Worker
    {
        uint32_t                  m_index;
        std::thread               m_thread;
        static int workerMain(TaskScheduler* ts, uint32_t index)
        {
            while (ts->isRunning())
            {
                for (uint32_t priority = TaskSetPriorityCount - 1; priority > (int)TaskSetPriority::Low; priority--)
                {
                    auto& queue = m_perWorkerQueue[priority];
                }
            }

            return 0;
        }
    };


    void TaskScheduler::init(const TaskSchedulerConfig& config)
    {
        for (uint32_t worker = 0; worker < config.workersCount; worker++)
        {
            m_workers[worker].m_thread = std::thread(Worker::workerMain, this, worker);
        }
    }

    void TaskScheduler::shutdown()
    {
    }

} // namespace TL