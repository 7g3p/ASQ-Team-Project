#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;


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

	FlightData ParseData(char* data);

public:

	int static ParseFromInput(char* fileName);
};
