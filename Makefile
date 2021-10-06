runsim:
	g++ runsim.cpp license.cpp -o runsim

testChild:
	g++ testChild.cpp license.cpp -o testChild

clean:
	rm runsim testChild
