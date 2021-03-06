// tema1AG.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <time.h> 
#include <cstdlib>
#include <fstream>
#include <ctime>
#include<climits>

/////////////////////
//#define nrOfV 10
#define MaxIterations 100
#define SetPrecison 3
#define PI 3.14159265359
#define NormalAmountOfNumbers 30


using namespace std;

vector<double> hcfirst;
vector<double> hcbest;
vector<double> annealing;


vector<double>interval(2);
bool minOrMax;/* 0-min 1-max*/
int  function;
vector<double> maxFunction(3), minFunction(3);
unsigned int nrOfV;
int vectrCount;


double randDoubleVal(double rM, double lM) {
	double range = lM - rM;
	double randVal = (double)rand() / (double)RAND_MAX;
	return range + randVal + rM;
}


void initMaxs(vector<double> &maxFunction, vector<double> &minFunction) {
	for (int i = 0; i < 3; i++) {
		maxFunction[i] = LLONG_MIN;
		minFunction[i] = LLONG_MAX;
	}
}


vector<int> decodeBits1(vector<bool>solution1) {
	vector<int> decodedBits;
	int q=0;
	for (int i = 0; i <solution1.size(); i++) {
		q += pow(2, i)*solution1[i];
	}
	decodedBits.push_back(q);
	return decodedBits;
}


long f(vector<int>a) {
	return pow(a[0], 3) - 60 * pow(a[0], 2) + 900 * a[0] + 100;
}





void genRandBitString(vector<bool>&randbitstring) {
	for (auto i = 0; i <randbitstring.size(); i++) {
		randbitstring[i] = rand() % 2;
		//cout << best[i] << " ";
	}
}

double dejong(vector<double>vctr) {
	double fctRes = 0;
	for (int i = 0; i < vctr.size(); i++) {
		fctRes += vctr[i] * vctr[i];
	}
	return fctRes;
}




int getFunctions() {
	cout << "1.Six Hump Camel Back \n2.De Jong  \n3.Rastrigin \n4. Schwefel \nIntroduceti indicele functiei: ";
	int index_fct;

	cin >> index_fct;
	switch (index_fct)
	{
	case 1: {/*Six Hump*/
		interval[0] = -2;
		interval[1] = 2;
		nrOfV = 2;
		break;
	}
	case 2: {/*De J*/
		interval[0] = -5.12;
		interval[1] = 5.12;
		nrOfV = NormalAmountOfNumbers;
		break;
	}
	case 3: {/*Rastrigin*/
		interval[0] = -5.12;
		interval[1] = 5.12;
		nrOfV = NormalAmountOfNumbers;
		break;
	}
	case 4: {/*Schwefel*/
		interval[0] = -500;
		interval[1] = 500;
		nrOfV = NormalAmountOfNumbers;
		break;
	}
	default:
	{
		cout << "BAD ENTRY\nPROGRAM TERMINATED";
		exit(-1);
	}
	}
	return index_fct;
}


void maxInFct(double value) {
	if (value > maxFunction[vectrCount])maxFunction[vectrCount] = value;
	if (value < minFunction[vectrCount])minFunction[vectrCount] = value;
}

unsigned int getFirstNeighbor(vector<bool>currbitString) {
	double best;
	double curr;
		best = f(decodeBits1(currbitString));
		for (unsigned int bit = 0; bit < currbitString.size(); bit++)
		{
			currbitString[bit] = !currbitString[bit];
			curr = f(decodeBits1(currbitString));
			currbitString[bit] = !currbitString[bit];
			if (minOrMax == 0)
			{
				if (curr < best)return bit;
			}
			else
			{
				if (curr > best)return bit;
			}
		}

		return UINT_MAX;
}




unsigned int getBestNeighbor(vector<bool>currbitString) {
	double best;
	double curr;
	unsigned int bestBit =UINT_MAX;


		best = f(decodeBits1(currbitString));
		for (unsigned int bit = 0; bit < currbitString.size(); bit++)
		{
			currbitString[bit] = !currbitString[bit];
			curr = f(decodeBits1(currbitString));
			currbitString[bit] = !currbitString[bit];

			if (minOrMax == 0)
			{
				if (curr < best) {
					best = curr;
					bestBit = bit;
				}
			}
			else
			{
				if (curr > best) {
					best = curr;
					bestBit = bit;
				}
			}


		}
	

	return bestBit;
}





void Annealing() {
	srand(time(NULL));
	double currBestValInString;
	double alteredBestValInString;
	bool local;
	vector<bool> Vc(5);
	genRandBitString(Vc);
	unsigned int stuckCounter;
	unsigned int randBit;
	int tests = 0;
	unsigned int temp = 10000;
	do {
		stuckCounter = 0;
		genRandBitString(Vc);
		do {
			local = false;
			vector<bool>Vn;
			stuckCounter++;
			currBestValInString = f(decodeBits1(Vc));
			randBit = rand() % Vc.size();
			alteredBestValInString = f(decodeBits1(Vc));
			
			if (minOrMax == 0) {
				if (alteredBestValInString > currBestValInString) {
					Vc[randBit] = !Vc[randBit];
				}
				else {
					if (randDoubleVal(0.0, 1.0) > exp(-abs(alteredBestValInString - currBestValInString) / temp))
						Vc[randBit] = !Vc[randBit];
					else
						local = true;
				}
			}
			else
			{
				if (alteredBestValInString < currBestValInString) {
					Vc[randBit] = !Vc[randBit];
				}
				else {
					if (randDoubleVal(0.0, 1.0) > exp(-abs(alteredBestValInString - currBestValInString) / temp))
						Vc[randBit] = !Vc[randBit];
					else
						local = true;
				}
			}
			annealing.push_back(currBestValInString);
		} while (local&&stuckCounter <= 30);

		currBestValInString = f(decodeBits1(Vc));
		maxInFct(currBestValInString);

		--temp;
		tests++;
	} while (tests < MaxIterations);
	//	cout << (minOrMax == 1 ? maxFunction[2] : minFunction[2]) << endl;
}




void hcBest() {
	srand(time(NULL));
	vector<bool> Vc(5);
	unsigned short count = 1;
	unsigned short Bit;
	double currBestValInString;
	double alteredBestValInString;
	bool local;
	int tests = 0;
	do {


		genRandBitString(Vc);
		do {
			local = false;
			vector<bool>Vn;
			unsigned int testBit = getBestNeighbor(Vc);
			if (testBit != UINT_MAX) {
				currBestValInString = f(decodeBits1(Vc));
				Vc[testBit] = !Vc[testBit];
				alteredBestValInString = f(decodeBits1(Vc));
				if (minOrMax == 0)
				{
					if (alteredBestValInString > currBestValInString) {
						Vc[testBit] = !Vc[testBit];
						local = true;
					}
				}
				else
				{
					if (alteredBestValInString < currBestValInString) {
						Vc[testBit] = !Vc[testBit];
						local = true;
					}
				}
			}
			hcbest.push_back(currBestValInString);
		} while (local);

		tests++;
		currBestValInString = f(decodeBits1(Vc));
		maxInFct(currBestValInString);
		
		//cout << currBestValInString << " ";
		//hcbest.push_back(currBestValInString);
	} while (tests < MaxIterations);
	//	return (minOrMax == 1 ? maxFunction[1] : minFunction[1]);
}


void hcFirst() {
	double currBestValInString;
	double alteredBestValInString;
	bool local;
	vector<bool> Vc(5);
	unsigned int t = 0;
	do {


		genRandBitString(Vc);
		do {
			local = false;
			vector<bool>Vn;
			unsigned int testBit = getFirstNeighbor(Vc);
			if (testBit != UINT_MAX) {
				currBestValInString = f(decodeBits1(Vc));
				Vc[testBit] = !Vc[testBit];
				alteredBestValInString = f(decodeBits1(Vc));

				if (minOrMax == 0)
				{
					if (alteredBestValInString > currBestValInString) {
						Vc[testBit] = !Vc[testBit];
						local = true;
					}
				}
				else
				{
					if (alteredBestValInString < currBestValInString)
					{
						Vc[testBit] = !Vc[testBit];
						local = true;
					}
				}

				hcfirst.push_back(currBestValInString);
			}
		} while (local);
		t++;
		currBestValInString = f(decodeBits1(Vc));
		maxInFct(currBestValInString);
		//hcfirst.push_back(currBestValInString);
	} while (t < MaxIterations);
	//cout << currBestValInString << " ";
	//cout << (minOrMax == 1 ? maxFunction[0] : minFunction[0]) << endl;
}
//


int main() {
	srand(time(NULL));

	initMaxs(maxFunction, minFunction);
	minOrMax = 1;
	hcFirst();
	vectrCount++;
	//cout << "Max:" << maxFunction[0] << "\tMin:" << minFunction[0] << "\n";
	hcBest();
	vectrCount++;
	Annealing();
	for (int i = 0; i < 3; i++) {
		cout << "Max:" << maxFunction[i] << "\tMin:" << minFunction[i] << "\n";
	}
	//long max = 0;
	//int max_i = 0;
	//for (int i = 1; i < 32; i++) {
	//	vector<int>a;
	//	a.push_back(i);
	//	cout << f(a)<<" ";
	//	if (max < f(a)) {
	//		max = f(a);
	//		max_i = i;
	//	}
	//}
	//cout << "\nMAX:" << max << " MAX_i" << max_i;
	return 0;
}