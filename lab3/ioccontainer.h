#ifndef IOCCONTAINER_H
#define IOCCONTAINER_H
#include <functional>
#include <memory>
#include <map>

using namespace std;

class IOCContainer
{
    static int s_nextTypeId;
    template<typename T>
    static int GetTypeID() {
        static int typeId = s_nextTypeId++;
        return typeId;
    }

public:
    class FactoryRoot
    {
    public:
        virtual ~FactoryRoot() {}
    };


    std::map<int, std::shared_ptr<FactoryRoot>> m_factories;

    template<typename T>
    class CFactory : public FactoryRoot
    {
        std::function<std::shared_ptr<T>()> m_functor;
    public:
        ~CFactory() {}
        CFactory(std::function<std::shared_ptr<T>()> functor) : m_functor(functor){}
        std::shared_ptr<T> GetObject() {
            return m_functor();
        }
    };

    template<typename T>
    std::shared_ptr<T> GetObject() {
        auto typeId = GetTypeID<T>();
        auto factoryBase = m_factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        return factory->GetObject();
    }

    template<typename TInterface, typename... TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>... ts)> functor) {
        m_factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([ = ] { return functor(GetObject<TS>()...); });
    }

    template<typename TInterface, typename... TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS>... ts)) {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>... ts)>(functor));
    }

    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> t) {
        m_factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([ = ] { return t; });
    }

    template<typename TInterface>
    void RegisterInstance(TInterface t) {
        m_factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>(
            [ = ] { return t; }
            );
    }

    template<typename TInterface, typename TConcrete, typename... TArguments>
    void RegisterFactory() {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments>... ts)>(
            [](std::shared_ptr<TArguments>... arguments) -> std::shared_ptr<TInterface> {
                return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
            }));
    }

    template<typename TInterface, typename TConcrete, typename... TArguments>
    void RegisterInstance() {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }
};

#endif // IOCCONTAINER_H
