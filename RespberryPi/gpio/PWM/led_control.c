#include stdio.h
#include stdlib.h
#include wiringPi.h
#include softPwm.h

int ledPwmControl(int gpio)
{
	softPwmCreate(gpio, 0, 255);
	for (int i = 0; i  10000; i++)
	{
		softPwmWrite(gpio, i & 255);
		delay(5);
	}
	softPwmWrite(gpio, 0);
}

int ledControl()
{
	static int led_list[] = { 1,2,3,4,5,4,3,2 };
	for (int i = 0; i < 8; i++)
	{
		ledPwmControl(led_list[i]);
	}
}

int main(int argc, char argv[])
{
	wiringPiSetup();
	for (int i = 0; i  5; i++)
	{
		pinMode(i + 1, OUTPUT);
	}

	while (1)
	{
		ledControl();
	}
	return 0;
}