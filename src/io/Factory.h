//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_FACTORY_H
#define FEROXILLS_FACTORY_H

#include  "ISampleProvider.h"
#include  "SoftwareProvider.h"
#include  "SerialProvider.h"

class ProviderFactory {
    public:
       static ISampleProvider * createProvider(const ProviderType::Type type) {
           switch (type) {
               case ProviderType::SOFTWARE_SOURCE:
                   return  new SoftwareProvider();
               case ProviderType::USB_SOURCE:
                   break;
               case ProviderType::UART_SOURCE:
                   return new SerialProvider();
               case ProviderType::NO_SOURCE:
                   break;
               default:
                   break;
           }
           return  nullptr;
       }
    static std::unique_ptr<ProviderSettings> getDefaultProviderSettings(const ProviderType::Type type) {
           switch (type) {
               case ProviderType::SOFTWARE_SOURCE:
                   {
                       auto sett = std::make_unique<SoftwareProviderSettings>();
                       sett->set_voltage(4);
                       sett->set_frequency(150);
                       sett->set_interval_ms(10);
                       sett->set_type(SignalType::SINUS);
                       sett->set_phase(0);
                       return sett;
                   }
               case ProviderType::UART_SOURCE:
                   {
                       auto sett = std::make_unique<SerialSettings>();
                       sett->setBaudRate(QSerialPort::Baud9600);
                       return sett;
                   }
               default:
                   return std::make_unique<SoftwareProviderSettings>();
           }
       }
};

#endif //FEROXILLS_FACTORY_H