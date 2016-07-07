/*
 * Copyright 2015 Gianluca Tiepolo <tiepolo.gian@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * CountSketch.h
 *
 *  Created on: Jul 7, 2016
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef COUNTSKETCH_H_
#define COUNTSKETCH_H_

#include "Murmurhash.h"
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <functional>
#include <algorithm>

/*
 * Implementation of the Count Sketch algorithm
 * based on <http://dimacs.rutgers.edu/~graham/pubs/papers/freqvldbj.pdf>
 */
class CountSketch {
	typedef std::vector<int> Matrice;
public:
	/*
	 * Constuctor
	 * params are epsilon (between 0.01 and 1) and gamma (between 0.1 and 1)
	 */
	CountSketch(double epsilon, double gamma) {
		// calculate depth and width based on epsilon and gamma
		d = ceil(log(4 / gamma));
		w = ceil(1 / pow(epsilon, 2));

		// create Matrix
		srand(time(NULL)); // seed time to rand so we get some random numbers
		for (unsigned int i = 0; i < d; ++i) {
			C.push_back(Matrice(w)); // create 'w' columns for each 'd' row
			seeds.push_back(rand()); // add random number to first seed function
			sign_seeds.push_back(rand()); // add random number to second seed function
		}
	}

	/*
	 * Add a string to the counter
	 */
	void addString(std::string s) {
		// use dafult C++ hasher to convert string to int
		int item = hasher(s);
		for (unsigned int i = 0; i < d; ++i) {
			// use value from seeds vector to seed the hashing function and create hash
			int p = murmurhash(&item, seeds[i]) % w;
			// use value from second seed vector (-1/+1)
			int sign = murmurhash(&item, sign_seeds[i]) % 2;
			// C = C + cg - update value
			C[i][p] += (sign * 2 - 1) * 1;
		}
	}

	/*
	 * Add an int to the counter
	 */
	void addInt(int item) {
		for (unsigned int i = 0; i < d; ++i) {
			// use value from seeds vector to seed the hashing function and create hash
			int p = murmurhash(&item, seeds[i]) % w;
			// use value from second seed vector (-1/+1)
			int sign = murmurhash(&item, sign_seeds[i]) % 2;
			// C = C + cg - update value
			C[i][p] += (sign * 2 - 1) * 1;
		}
	}

	/*
	 * Get the frequency of a string
	 */
	double getStringFrequency(std::string s) {
		int item = hasher(s);
		double values[d];
		for (unsigned int i = 0; i < d; ++i) {
			int p = murmurhash(&item, seeds[i]) % w;
			int sign = murmurhash(&item, sign_seeds[i]) % 2;
			values[i] = (sign * 2 - 1) * C[i][p];
		}
		// return the median (4.3.2 The median trick, "ESTIMATING THE NUMBER OF DISTINCT ELEMENTS", page 18)
		std::nth_element(values, values + d / 2, values + d);
		return values[d / 2];
	}

	/*
	 * Get the frequency of an int
	 */
	double getIntFrequency(int item) {
		double values[d];
		for (unsigned int i = 0; i < d; ++i) {
			int p = murmurhash(&item, seeds[i]) % w;
			int sign = murmurhash(&item, sign_seeds[i]) % 2;
			values[i] = (sign * 2 - 1) * C[i][p];
		}
		// return the median (4.3.2 The median trick, "ESTIMATING THE NUMBER OF DISTINCT ELEMENTS", page 18)
		std::nth_element(values, values + d / 2, values + d);
		return values[d / 2];
	}

	/*
	 * Virtual destructor
	 */
	virtual ~CountSketch(){};
private:
	// depth and width of the matrix
	unsigned int d, w;
	// matrix
	std::vector<Matrice> C;
	// vector of seeds
	std::vector<int> seeds;
	// 2nd vector of seeds (-1/+1)
	std::vector<int> sign_seeds;
	// string to int hasher
	std::hash<std::string> hasher;
};

#endif /* COUNTSKETCH_H_ */
