#pragma once

#include <functional>

#define DECLARE_MULTICAST_DELEGATE(DelegateName) \
    class DelegateName : public MulticastDelegate<> {}

#define DECLARE_MULTICAST_DELEGATE_OneParam(DelegateName, Param1) \
    class DelegateName : public MulticastDelegate<Param1> {}

#define DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateName, Param1, Param2) \
    class DelegateName : public MulticastDelegate<Param1, Param2> {}

#define DECLARE_MULTICAST_DELEGATE_ThreeParams(DelegateName, Param1, Param2, Param3) \
    class DelegateName : public MulticastDelegate<Param1, Param2, Param3> {}

template<typename...Args>
class MulticastDelegate
{
public:
    MulticastDelegate()
    {
        _trashes.reserve(10);
        _delegates.reserve(10);
    }

    virtual ~MulticastDelegate()
    {
        _delegates.clear();
    }

    using Callback = std::function<void(Args...)>;

    uint32 Bind(const Callback& callback)
    {
        uint32 id = ++_callbackId;
        _delegates[id] = callback;
        return id;
    }

    template<typename T>
    uint32 BindObject(shared_ptr<T> obj, void((T::* func)(Args...)))
    {
        weak_ptr<T> weak = obj;
        uint32 id = ++_callbackId;

        _delegates[id] = [this, weak, func, id](Args...args)
            {
                if (shared_ptr<T> shared = weak.lock())
                {
                    ((*shared).*func)(args...);
                }
                else
                {
                    this->AddTrash(id);
                }
            };
        return id;
    }

    void AddTrash(uint32 callbackId)
    {
        _trashes.push_back(callbackId);
    }

    void RemoveDelegate(uint32 callbackId)
    {
        if (_delegates.contains(callbackId))
            _delegates.erase(callbackId);
    }

    void Broadcast(Args...args)
    {
        for (auto& [id, callback] : _delegates)
            callback(args...);

        for (uint32 id : _trashes)
        {
            RemoveDelegate(id);
        }
        _trashes.clear();
    }

protected:
    
    unordered_map<uint32, Callback> _delegates;
    vector<uint32> _trashes;
    uint32 _callbackId = 0;
};

