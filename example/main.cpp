/*
 * Example usage
 */

#include "CountSketch.h"

int main() {
	// create counter setting epsilon=0.01 and gamma=0.1
	// (lower values give better precision but need more space)
	CountSketch counter(0.01, 0.1);

	// add a few strings
	counter.addString("Gianluca");
	counter.addString("Marco");
	counter.addString("Luca");
	counter.addString("Gianluca");
	counter.addString("Anna");

	// add a few numbers
	counter.addInt(26);
	counter.addInt(131);
	counter.addInt(742);
	counter.addInt(26);
	counter.addInt(26);
	counter.addInt(12);

	// print frequency
	std::cout << "The frequency of 'Gianluca' is " << counter.getStringFrequency("Gianluca") << std::endl;
	std::cout << "The frequency of '26' is " << counter.getIntFrequency(26) << std::endl;

	return 0;
}
