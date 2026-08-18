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
};

#endif //FEROXILLS_FACTORY_H