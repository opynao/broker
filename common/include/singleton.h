#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <string>

namespace Common
{
    template <typename SingletonType>
    class Singleton
    {
    public:
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;
        Singleton(Singleton &&) = delete;
        Singleton &operator=(Singleton &&) = delete;

        static std::shared_ptr<SingletonType> GetInstance()
        {
            auto sp = m_instance.lock();
            if (!sp)
            {
                sp = std::make_shared<SingletonType>();
                m_instance = sp;
            }
            return sp;
        }

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    private:
        inline static std::weak_ptr<SingletonType> m_instance;
    };
} // !namespace Common
#endif