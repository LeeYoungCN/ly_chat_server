#ifndef SINGLETON_H
#define SINGLETON_H

namespace common::base {
template <typename T>
class SingletonBase {
public:
    static T& getInstance()
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
#endif  // SINGLETON_H
