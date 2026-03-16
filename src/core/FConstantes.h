//
// Created by ferdinand on 15/03/2026.
//

#ifndef FEROXILLS_FCONSTANTES_H
#define FEROXILLS_FCONSTANTES_H

namespace Feroxills::Constants {
    static constexpr unsigned int HORIZONTAL_DIVISIONS = 10;
    static constexpr unsigned int VERTICAL_DIVISIONS = 8;
    static constexpr float VERTICAL_SCALES[] = {
        0.01f,0.02f,0.05f,
        0.1f,0.2f,0.5f,
        1.0f,2.0f,5.0f,
        10.0f,20.0f,50.0f
    };
    static constexpr float HORIZONTAL_SCALES[] = {
        1e-9f, 2e-9f, 5e-9f,
        1e-8f, 2e-8f, 5e-8f,
        1e-7f, 2e-7f, 5e-7f,
        1e-6f, 2e-6f, 5e-6f,
        1e-5f, 2e-5f, 5e-5f,
        1e-4f, 2e-4f, 5e-4f,
        1e-3f, 2e-3f, 5e-3f,
        1e-2f, 2e-2f, 5e-2f,
        1e-1f, 2e-1f, 5e-1f,
        1.0f,  2.0f,  5.0f
    };
    static constexpr unsigned long SAMPLING_FREQUENCY = 44100;
    static constexpr unsigned int NUMBER_OF_PERIOD_PRINT_IN_AUTO = 2;
    static constexpr double SCALE_HYSTERESIS = 0.2; // 20%
}

#endif //FEROXILLS_FCONSTANTES_H