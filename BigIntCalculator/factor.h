#pragma once

/*
This file is part of Alpertron Calculators.
Copyright 2015 Dario Alejandro Alpern
Alpertron Calculators is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
Alpertron Calculators is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with Alpertron Calculators.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mpir.h"
#include "boost/multiprecision/gmp.hpp" 
#define ZT(a) a.backend().data()
#define ZisEven(a) (mpz_even_p(ZT(a)) != 0)  /* true iff a is even (works for -ve a as well) */
typedef boost::multiprecision::mpz_int Znum;

#include "showtime.h"

//#ifdef __EMSCRIPTEN__
//void getCunn(char *url, char *factorsFromServer);
//#endif

typedef boost::multiprecision::mpz_int Znum;

class zFactors {
public:
	Znum Factor;
	int exponent;
	int upperBound;
};


void showECMStatus(void);
bool ecm(Znum &Nz, long long maxdivisor);
extern int lang;
extern Znum Zfactor;

/* access underlying mpz_t inside an bigint */
#define ZT(a) a.backend().data()

bool factorise(Znum numberZ, std::vector <zFactors> &factorlist,
	Znum Quad[]);
void BigIntPowerIntExp(const Znum &Base, int exponent, Znum &Power);
long long MulPrToLong(const Znum &x);
long long JacobiSymbol(long long upper, long long lower);
long long PowerCheck(const Znum &BigInt, Znum &Base, long long upperBound);
double logBigNbr(const Znum &BigInt);
void DivideBigNbrByMaxPowerOf2(int &ShRight, Znum &number);
int PrimalityTest(const Znum &Value, long long upperBound);
