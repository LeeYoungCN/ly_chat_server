#ifndef COMMON_BASE_SINGLETON_H
#define COMMON_BASE_SINGLETON_H

namespace common::base {
template <typename T>
class SingletonBase {
public:
    static T& instance()
    {
        static T instance;
        return instance;
    };

    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

protected:
    SingletonBase() = default;
    virtual ~SingletonBase() = default;
};
}  // namespace common::base
#endif  // COMMON_BASE_SINGLETON_H
