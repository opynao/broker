#pragma once
#include "message.h"
#include "utils.h"

#include <iostream>
#include <vector>
#include <set>

template <typename Type>
class Observer
{
public:
    virtual ~Observer() = default;
    virtual void OnMessageReceived(const Type &) = 0;
};

template <typename Type>
class Observable
{
public:
    void Subscribe(const std::shared_ptr<Observer<Type>> &observer)
    {
        observers.insert(observer);
    }
    void Unsubscribe(const std::shared_ptr<Observer<Type>> &observer)
    {
        observers.erase(observers.find(observer));
    }

protected:
    void Notify(const Type &msg)
    {
        for (auto obs : observers)
            if (obs)
                obs->OnMessageReceived(msg);
    }

private:
    std::set<std::shared_ptr<Observer<Type>>> observers;
};