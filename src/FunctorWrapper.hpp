#include <functional>

template<typename... ARGS>
class FunctorWrapper
{
public:
    FunctorWrapper( std::function<void(ARGS...)> f ) : m_f(f) {}
    void operator()(ARGS... args) const { m_f(args...); }

private:
    std::function<void(ARGS...)> m_f;
};

template<typename... ARGS>
FunctorWrapper<ARGS...> MakeFunctorWrapper(void(*f)(ARGS...)) { return FunctorWrapper<ARGS...>(f); }
