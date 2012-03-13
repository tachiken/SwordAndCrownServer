TARGET:= server
SOURCES:= src/SwordAndCrownServer.cpp src/session.cpp
.PHONY: clean

$(TARGET): $(SOURCES)
	g++ -g -lpthread $(SOURCES) -o $(TARGET)

clean:
	$(RM) -f $(TARGET)
