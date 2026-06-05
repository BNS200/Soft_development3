#ifndef IOCTASK_H
#define IOCTASK_H
#include <iostream>
#include <string>
#include "ioccontainer.h"

enum ProcessorType
{
    x86,
    x86_64
};

class IProcessor
{
public:
    virtual ~IProcessor() = default;
    virtual std::string GetProcessorInfo() const = 0;
    virtual double getSpeed() const = 0;
    virtual ProcessorType getType() const = 0;
    virtual std::string getVersion() const = 0;
};

class IntelProcessor : public IProcessor
{
private:
    std::string m_version;
    ProcessorType m_type;
    double m_speed;

public:
    IntelProcessor(double speed, ProcessorType type, std::string version) : m_speed(speed), m_type(type), m_version(version){}

    std::string GetProcessorInfo() const override
    {
        std::string typeStr = (m_type == x86) ? "x86" : "x86_64";
        return "Intel Processor: " + m_version + ", Speed: " + to_string(m_speed) + ", Type: " + typeStr;
    }

    double getSpeed() const override {
        return m_speed;
    }
    ProcessorType getType() const override {
        return m_type;
    }
    std::string getVersion() const override {
        return m_version;
    }
};

class AMDProcessor : public IProcessor
{
private:
    std::string m_version;
    ProcessorType m_type;
    double m_speed;

public:
    AMDProcessor(double speed, ProcessorType type, std::string version) : m_speed(speed), m_type(type), m_version(version){}

    std::string GetProcessorInfo() const override
    {
        std::string typeStr = (m_type == x86) ? "x86" : "x86_64";
        return "AMD Processor: " + m_version + ", Speed: " + to_string(m_speed) + ", Type: " + typeStr;
    }

    double getSpeed() const override {
        return m_speed;
    }
    ProcessorType getType() const override {
        return m_type;
    }
    std::string getVersion() const override {
        return m_version;
    }
};

class Computer
{
private:
    std::shared_ptr<IProcessor> m_processor;

public:
    Computer(std::shared_ptr<IProcessor> processor) : m_processor(processor)
    {}

    void Configure()
    {
        std::cout << "Computer configured with: " << m_processor->GetProcessorInfo() << std::endl;

    }

    void SetProcessor(std::shared_ptr<IProcessor> processor)
    {
        m_processor = processor;
    }

    std::shared_ptr<IProcessor> GetProcessor() const
    {
        return m_processor;
    }
};


IOCContainer gContainer;

std::shared_ptr<IProcessor> CreateIntelProcessor()
{
    return make_shared<IntelProcessor>(3.4, x86_64, "Core i7-12700K");
}

std::shared_ptr<IProcessor> CreateAMDProcessor()
{
    return make_shared<AMDProcessor>(4.0, x86_64, "Ryzen 7 5800X");
}

void RegisterIntel()
{
    gContainer.RegisterFunctor<IProcessor>(CreateIntelProcessor);
}

void RegisterAMD()
{
    gContainer.RegisterFunctor<IProcessor>(CreateAMDProcessor);
}

std::shared_ptr<Computer> CreateComputer() {
    return make_shared<Computer>(gContainer.GetObject<IProcessor>());
}
#endif // IOCTASK_H
