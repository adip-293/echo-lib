#include "echolib/Auton.hpp"

Auton::Auton(std::function<void()> iAutonomousCallback,const char* iAutonomousName, const char* iAutonomousDesc){
    autonomousCallback = iAutonomousCallback;
    autonomousName = iAutonomousName;
    autonomousDesc = iAutonomousDesc;
}