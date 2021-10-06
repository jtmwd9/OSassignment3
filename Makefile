runsim:
	g++ runsim.cpp license.cpp -o runsim

testsim:
	g++ testsim.cpp license.cpp -o testsim

clean:
	rm runsim testsim
