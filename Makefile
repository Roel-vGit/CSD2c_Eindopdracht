CXX= g++
CXXFLAGS = -I/usr/local/include -Wall -g -std=c++1z -g
LDFLAGS= -ljack
SOURCE= main.o effect.o CircularBuffer.o oscillator.o waveshaper.o
TARGET= app

$(TARGET) : $(SOURCE)
	$(CXX) -o $@ $(SOURCE) $(LDFLAGS)

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm -f $(TARGET) *.o *.csv


