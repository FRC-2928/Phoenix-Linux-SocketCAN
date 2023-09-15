#define Phoenix_No_WPI // remove WPI dependencies
#include "ctre/Phoenix.h"
#include "ctre/phoenix/platform/Platform.hpp"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
// #include <SDL2/SDL.h>

using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;

/* make some talons for drive train */
std::string interface = "can0";
TalonSRX talLeft(1, interface); //Use the specified interface
TalonSRX talRght(0); //Use the default interface (can0)

void initDrive()
{
	/* both talons should blink green when driving forward */
	talRght.SetInverted(true);
}

void drive(double fwd, double turn)
{
    // if (fwd != 0 || turn != 0) 
        printf("%f fwd\n"
                "%f turn\n",
                fwd,
                turn);
	double left = fwd - turn;
	double rght = fwd + turn; /* positive turn means turn robot LEFT */

	talLeft.Set(ControlMode::PercentOutput, left);
	talRght.Set(ControlMode::PercentOutput, rght);
}
/** simple wrapper for code cleanup */
void sleepApp(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {	
	// Comment out the call if you would rather use the automatically running diag-server, note this requires uninstalling diagnostics from Tuner. 
	// c_SetPhoenixDiagnosticsStartTime(-1); // disable diag server, instead we will use the diag server stand alone application that Tuner installs

	/* setup drive */
	initDrive();
    printf("Drive Initiated\n");

	while (true) {
		/* we are looking for gamepad (first time or after disconnect),
			neutral drive until gamepad (re)connected. */
		drive(0.5, 0);

        /* loop yield for a bit */
        sleepApp(10000);

		/* we've left the loop, likely due to gamepad disconnect */
		drive(0, 0);
	
	}
	return 0;
}
