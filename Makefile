all: webclient

webclient: webclient.cpp
	g++ -o webclient webclient.cpp

clean:
	rm -f webclient

submit:
	~schwesin/bin/submit cpsc328 project3

.PHONY: all clean submit

