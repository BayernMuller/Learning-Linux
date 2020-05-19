#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

int ledControl()
{
	for (int i = 0; i < 5; i++)
	{
		digitalWrite(i + 1, HIGH);
		delay(1000);

		digitalWrite(i + 1, LOW);
		delay(1000);
	}
	for (int i = 4; i > 1; i--)
	{
		digitalWrite(i, HIGH);
		delay(1000);

		digitalWrite(i, LOW);
		delay(1000);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	wiringPiSetup();
	for (int i = 0; i < 5; i++)
	{
		pinMode(i + 1, OUTPUT);
	}

	while (1)
	{
		ledControl();
	}
	return 0;
}