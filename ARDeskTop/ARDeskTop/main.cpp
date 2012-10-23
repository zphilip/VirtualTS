#include "CARDeskTop.h"


int main(int argc, char** argv)
{
	CARDeskTop* arDeskTop = new CARDeskTop();
	arDeskTop->Run(argc, argv, CARDeskTop::kVR, "Data/settings_v");

	return (0);
}
//kCalibration
