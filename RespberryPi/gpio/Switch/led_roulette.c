#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

int isPress(int sw)
{
	return digitalRead(sw) == LOW;
}

void waitForPress(int sw)
{
	while (1)
	{
		if (isPress(sw))
			return;
	}
}

void ledControl(int sw)
{
	static int led_list[] = { 1,2,3,4,5,4,3,2 };
	for (int i = 0; i < 8; i++)
	{
		digitalWrite(led_list[i], HIGH);

		if (isPress(sw))
		{
			waitForPress(sw);
		}
		delay(300);

		digitalWrite(led_list[i], LOW);
		delay(300);
	}
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
		ledControl(5);
	}
	return 0;
}