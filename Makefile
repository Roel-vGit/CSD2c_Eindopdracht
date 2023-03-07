CXX= g++
CXXFLAGS = -I/usr/local/include -Wall -std=c++1z -g
LDFLAGS= -ljack
SOURCE= main.o effect.o
TARGET= app

$(TARGET) : $(SOURCE)
	$(CXX) -o $@ $(SOURCE) $(LDFLAGS)

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm -f $(TARGET) *.o *.csv


