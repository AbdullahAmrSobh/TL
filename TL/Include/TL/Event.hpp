#pragma once

#include "TL/Containers/Map.hpp"
#include "TL/Containers/Vector.hpp"
#include "TL/Containers/Function.hpp"

namespace TL
{
    template<typename Event>
    class EventQueue
    {
    public:
        struct SubID
        {
            size_t id;
        };

        using Handler = TL::Function<bool(const Event&)>;

        EventQueue() = default;
        EventQueue(IAllocator* allocator);
        EventQueue(const EventQueue&) = delete;

        SubID subscribe(Handler&& handler);

        void  unsubscribe(SubID);

        void  broadcast(const Event& event);

        void  poll();

    protected:
        void onBroadcastImpl(Event event);

    private:
        // todo: order based on (when handler returns true blocks subsequent subs from listening to this event)
        TL::Map<size_t, Handler> m_subscribers;
    };

    template<typename Event>
    EventQueue<Event>::EventQueue(IAllocator* allocator)
        : m_subscribers(allocator)
    {
    }

    template<typename Event>
    typename EventQueue<Event>::SubID EventQueue<Event>::subscribe(Handler&& handler)
    {
        static size_t nextId = 1;
        size_t        id     = nextId++;
        m_subscribers[id]    = handler;
        return SubID{id};
    }

    template<typename Event>
    void EventQueue<Event>::unsubscribe(SubID subId)
    {
        m_subscribers.erase(subId.id);
    }

    template<typename Event>
    void EventQueue<Event>::broadcast(const Event& event)
    {
        for (auto& [id, handler] : m_subscribers)
        {
            if (handler(event) == true)
            {
                break;
            }
        }
        // m_queue.push_back(event);
    }

    template<typename Event>
    void EventQueue<Event>::poll()
    {
        // for (const auto& event : m_queue)
        // {
        //     for (auto& [id, handler] : m_subscribers)
        //     {
        //         handler(event);
        //     }
        // }
        // m_queue.clear();
    }
} // namespace TL