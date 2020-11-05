#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "Header.h"

class DataManipulation
{
private:
	typedef struct flightData
	{
		char* dateTime;
		double x;
		double y;
		double z;
		double weight;
		double alt;
		double pitch;
		double bank;
	}FlightData;

public:

	DataPacket ParseFromInput(char* fileName)
	{
		DataPacket retData;



		return retData;
	}





};