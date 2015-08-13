OBJS = test.o SensorNetwork.o
CC = g++ -std=c++11
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

Test : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o Test

test.o : test.cpp SensorNetwork.h
	$(CC) $(CFLAGS) test.cpp

SensorNetwork.o : SensorNetwork.h SensorNetwork.cpp Target.h Sensor.h
	$(CC) $(CFLAGS) SensorNetwork.cpp

Sensor.o : Sensor.h Sensor.cpp Policy.h
	$(CC) $(CFLAGS) Sensor.cpp
