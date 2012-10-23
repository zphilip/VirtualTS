#include "CARDeskTop.h"


int main(int argc, char** argv)
{
	CARDeskTop* arDeskTop = new CARDeskTop();
	arDeskTop->Run(argc, argv, CARDeskTop::kAR, "Data/settings_a");

	return (0);
}
