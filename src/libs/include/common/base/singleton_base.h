#ifndef SINGLETON_BASE_H
#define SINGLETON_BASE_H

#include <memory>
#include <mutex>

#define INST(CLASS_NAME) CLASS_NAME.getInstance()

template <typename T>
class SingletonBase {
public:
    static T& getInstance()
    {
        std::once_flag onceFlag;
        std::call_once(onceFlag, []() { _instance = std::make_unique<T>(); });
        return *_instance;
    };

    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

protected:
    SingletonBase() = default;
    virtual ~SingletonBase() = default;

private:
    static std::unique_ptr<T> _instance;
};

template <typename T>
std::unique_ptr<T> SingletonBase<T>::_instance = nullptr;

#endif  // SINGLETON_BASE_H
