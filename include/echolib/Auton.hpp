#pragma once
#include <functional>
#include <iostream>

class Auton {
    public:
        Auton(std::function<void()> iAutonomousCallback,const char* iAutonomousName, const char* iAutonomousDesc);

        const char* autonomousName;
        const char* autonomousDesc;
        std::function<void()> autonomousCallback;
    private:
};