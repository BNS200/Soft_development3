// #include "mainwindow.h"
#include <QApplication>
#include "ioctask.h"

int IOCContainer::s_nextTypeId = 115094801;

int main(int argc, char *argv[]){
    RegisterIntel();

    auto computer1 = std::make_shared<Computer>(gContainer.GetObject<IProcessor>());
    cout << "\nComputer 1: ";
    computer1->Configure();


    RegisterAMD();

    auto computer2 = std::make_shared<Computer>(gContainer.GetObject<IProcessor>());
    cout << "Computer 2: ";
    computer2->Configure();

    // auto customCPU = make_shared<IntelProcessor>(5.0, x86_64, "Custom CPU");
    // gContainer.RegisterInstance<IProcessor>(customCPU);

    auto computer3 = std::make_shared<Computer>(gContainer.GetObject<IProcessor>());
    cout << "Computer 3: ";
    computer3->Configure();


    // gContainer.RegisterInstance<double>(std::make_shared<double>(3.4));
    // gContainer.RegisterInstance<ProcessorType>(std::make_shared<ProcessorType>(x86_64));
    // gContainer.RegisterInstance<string>(std::make_shared<string>("Core i7"));

    // gContainer.RegisterFactory<IProcessor, IntelProcessor, double, ProcessorType, string>();

    // gContainer.RegisterFunctor<Computer>(CreateComputer);

    // auto computer4 = gContainer.GetObject<Computer>();
    // computer4->Configure();

    // gContainer.RegisterInstance<double>(std::make_shared<double>(4.0));
    // gContainer.RegisterInstance<string>(std::make_shared<string>("Core i9"));

    // auto computer5 = gContainer.GetObject<Computer>();
    // computer2->Configure();

    return 0;
}
