#ifndef SINGLETON_BASE_H
#define SINGLETON_BASE_H

#define INST(CLASS_NAME) CLASS_NAME::getInstance()

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

#endif  // SINGLETON_BASE_H
