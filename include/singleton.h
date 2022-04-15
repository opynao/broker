#pragma once

template <typename T>
class Singleton
{
public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator=(Singleton &&) = delete;

    static T &GetInstance()
    {
        if (!m_instance)
            m_instance = new T();

        return *m_instance;
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    inline static T *m_instance{nullptr};
};
