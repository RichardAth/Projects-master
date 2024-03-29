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
#include <string>
#include <cmath>
#include "bignbr.h"

#define MAX_LIMBS_SIQS 15

/* nbr = - nbr */
void ChSignBigNbr(int nbr[], int length)
{
	int carry = 0;
	int ctr;
	for (ctr = 0; ctr < length; ctr++)
	{
		carry -= nbr[ctr];
		nbr[ctr] = carry & MAX_INT_NBR;
		carry >>= BITS_PER_INT_GROUP;
	}
}

/* nbr = - nbr */
void ChSignBigNbrB(int nbr[], int length)
{
	int carry = 0;
	int ctr;
	for (ctr = 0; ctr < length - 1; ctr++)
	{
		carry -= nbr[ctr];
		nbr[ctr] = carry & MAX_INT_NBR;
		carry >>= BITS_PER_INT_GROUP;
	}
	nbr[ctr] = carry - nbr[ctr];  /* last word does not have most significant bit masked off */
}
int BigNbrLen(const int Nbr[], int nbrLen) {
	int ix;
	for (ix = nbrLen; ix > 0; ix--) {
		if (Nbr[ix - 1] != 0)
			break;
	}
	return ix;
}
int BigNbrLen(const limb Nbr[], int nbrLen) {
	int ix;
	for (ix = nbrLen; ix > 0; ix--) {
		if (Nbr[ix - 1].x != 0)
			break;
	}
	return ix;
}

/* Sum = Nbr1+Nbr2  */
void AddBigNbr(const int Nbr1[], const int Nbr2[], int Sum[], int nbrLen)
{
	unsigned int carry = 0;
	int i;
	for (i = 0; i < nbrLen; i++)
	{
		carry = (carry >> BITS_PER_INT_GROUP) + (unsigned int)Nbr1[i] + (unsigned int)Nbr2[i];
		Sum[i] = (int)(carry & MAX_INT_NBR);
	}
}

/* Diff = Nbr1-Nbr2 */
void SubtractBigNbr(const int Nbr1[], const int Nbr2[], int Diff[], int nbrLen)
{
	int borrow = 0;
	int i;
	for (i = 0; i < nbrLen; i++)
	{
		borrow = (borrow >> BITS_PER_INT_GROUP) + Nbr1[i] - Nbr2[i];
		Diff[i] = borrow & MAX_INT_NBR;
	}
}

/* Sum = Nbr1+Nbr2  */
void AddBigNbrB(const int Nbr1[], const int Nbr2[], int Sum[], int nbrLen)
{
	unsigned int carry = 0;
	int i;
	for (i = 0; i < nbrLen - 1; i++)
	{
		carry = (carry >> BITS_PER_INT_GROUP) + (unsigned int)Nbr1[i] + (unsigned int)Nbr2[i];
		Sum[i] = (int)(carry & MAX_INT_NBR);
	}
	carry = (carry >> BITS_PER_INT_GROUP) + (unsigned int)Nbr1[i] + (unsigned int)Nbr2[i];
	Sum[i] = (int)carry;   // last 'digit' is not masked with MAX_INT_NBR
}

/* Diff = Nbr1-Nbr2 */
void SubtractBigNbrB(const int Nbr1[], const int Nbr2[], int Diff[], int nbrLen)
{
	int borrow = 0;
	int i;
	for (i = 0; i < nbrLen - 1; i++)
	{
		borrow = (borrow >> BITS_PER_INT_GROUP) + Nbr1[i] - Nbr2[i];
		Diff[i] = borrow & MAX_INT_NBR;
	}
	borrow = (borrow >> BITS_PER_INT_GROUP) + Nbr1[i] - Nbr2[i];
	Diff[i] = borrow;  /* B version differs from SubtractBigNbr only in that the most significant
					   word does not have the most significant bit masked off*/
}

/* Sum = Nbr1+Nbr2 (mod Mod) */
void AddBigNbrModN(const int Nbr1[], const int Nbr2[], int Sum[], const int Mod[], int nbrLen)
{
	int borrow = 0;
	unsigned int carry = 0;
	int i;
	for (i = 0; i < nbrLen; i++)
	{
		carry = (carry >> BITS_PER_INT_GROUP) + 
			(unsigned int)Nbr1[i] + (unsigned int)Nbr2[i];
		Sum[i] = (int)(carry & MAX_INT_NBR);
	}
	carry >>= BITS_PER_INT_GROUP;
	//pSum -= nbrLen;
	for (i = 0; i < nbrLen; i++)
	{
		borrow = (borrow >> BITS_PER_INT_GROUP) + Sum[i] - Mod[i];
		Sum[i] = borrow & MAX_INT_NBR;
	}
	borrow >>= BITS_PER_INT_GROUP;
	if (borrow + (int)carry != 0)
	{    // Sum is less than zero. Add Mod again.
		//pSum -= nbrLen;
		//pMod -= nbrLen;
		carry = 0;
		for (i = 0; i < nbrLen; i++)
		{
			carry = (carry >> BITS_PER_INT_GROUP) + (unsigned int)Sum[i] + (unsigned int)Mod[i];
			Sum[i] = (int)(carry & MAX_INT_NBR);
		}
	}
}

/* Diff = Nbr1-Nbr2 (mod Mod)*/
void SubtractBigNbrModN(const int Nbr1[], const int Nbr2[], int Diff[], const int Mod[], int nbrLen)
{
	int borrow = 0;
	int i;
	for (i = 0; i < nbrLen; i++)
	{
		borrow = (borrow >> BITS_PER_INT_GROUP) + Nbr1[i] - Nbr2[i];
		Diff[i] = borrow & MAX_INT_NBR;
	}
	if (borrow != 0)
	{
		unsigned int carry = 0;
		//pDiff -= nbrLen;
		for (i = 0; i < nbrLen; i++)
		{
			carry = (carry >> BITS_PER_INT_GROUP) + (unsigned int)Diff[i] + (unsigned int)Mod[i];
			Diff[i] = (int)(carry & MAX_INT_NBR);
		}
	}
}

/* bigProd = bigFactor*factor */
void MultBigNbrByInt(const int bigFactor[], int factor, int bigProd[], int nbrLen)
{
	int *bigProduct = bigProd;
	double dFactor;
	double dVal = 1 / (double)(1U << BITS_PER_INT_GROUP);
	bool factorPositive = true;
	int ctr, carry;
	if (factor < 0)
	{     // If factor is negative, indicate it and compute its absolute value.
		factorPositive = false;
		factor = -factor;
	}
	dFactor = (double)factor;
	carry = 0;
	for (ctr = 0; ctr < nbrLen; ctr++)
	{
		int low = (bigFactor[ctr] * factor + carry) & MAX_INT_NBR;
		// Subtract or add 0x20000000 so the multiplication by dVal is not nearly an integer.
		// In that case, there would be an error of +/- 1.
		if (low < HALF_INT_RANGE)
		{
			carry = (int)floor(((double)bigFactor[ctr] * dFactor + (double)carry + HALF_INT_RANGE / 2)*dVal);
		}
		else
		{
			carry = (int)floor(((double)bigFactor[ctr] * dFactor + (double)carry - HALF_INT_RANGE / 2)*dVal);
		}
		bigProduct[ctr] = low;
		//bigFactor++;
	}
	if (!factorPositive)
	{         // If factor is negative, change sign of product.
		ChSignBigNbr(bigProd, nbrLen);
	}
}

/* bigProd = bigFactor*factor */
void MultBigNbrByIntB(const int bigFactor[], int factor, int bigProd[], int nbrLen)
{
	int *bigProduct = bigProd;
	int low;
	double dFactor;
	double dVal = 1 / (double)(1U << BITS_PER_INT_GROUP);
	bool factorPositive = true;
	int ctr, carry;
	if (factor < 0)
	{     // If factor is negative, indicate it and compute its absolute value.
		factorPositive = false;
		factor = -factor;
	}
	dFactor = (double)factor;
	carry = 0;
	for (ctr = 0; ctr < nbrLen - 1; ctr++)
	{
		low = (bigFactor[ctr] * factor + carry) & MAX_INT_NBR;
		// Subtract or add 0x20000000 so the multiplication by dVal is not nearly an integer.
		// In that case, there would be an error of +/- 1.
		if (low < HALF_INT_RANGE)
		{
			carry = (int)floor(((double)bigFactor[ctr] * dFactor + (double)carry + HALF_INT_RANGE / 2)*dVal);
		}
		else
		{
			carry = (int)floor(((double)bigFactor[ctr] * dFactor + (double)carry - HALF_INT_RANGE / 2)*dVal);
		}
		bigProduct[ctr] = low;
		//bigFactor++;
	}
	low = bigFactor[ctr] * factor + carry;
	if ((low & MAX_INT_NBR) < HALF_INT_RANGE)
	{
		carry = (int)floor(((double)bigFactor[ctr] * dFactor + (double)carry + HALF_INT_RANGE / 2)*dVal);
	}
	else
	{
		carry = (int)floor(((double)bigFactor[ctr] * dFactor + (double)carry - HALF_INT_RANGE / 2)*dVal);
	}
	bigProduct[ctr] = low;
	if (!factorPositive)
	{         // If factor is negative, change sign of product.
		ChSignBigNbrB(bigProd, nbrLen);
	}
}

/* Quotient = Dividend/divisor */
void DivBigNbrByInt(const int Dividend[], int divisor, int Quotient[], int nbrLen)
{
	int ctr;
	int remainder = 0;
	double dDivisor = (double)divisor;  // assume divisor > 0
	double dLimb = 0x80000000;
	for (ctr = nbrLen - 1; ctr >= 0; ctr--)
	{
		double dDividend, dQuotient;
		int quotient, dividend;
		dividend = (remainder << BITS_PER_INT_GROUP) + Dividend[ctr];
		dDividend = (double)remainder * dLimb + Dividend[ctr];
		dQuotient = floor(dDividend / dDivisor + 0.5);
		quotient = (unsigned int)dQuotient;   // quotient has correct value or 1 more.
		remainder = dividend - quotient * divisor;
		if ((unsigned int)remainder >= (unsigned int)divisor)
		{     // remainder not in range 0 <= remainder < divisor. Adjust.
			quotient--;
			remainder += divisor;
		}
		Quotient[ctr] = quotient;
	}
}

/* calculate dividend%divisor */
int RemDivBigNbrByInt(const int Dividend[], int divisor, int nbrLen)
{
	int ctr;
	int remainder = 0;
	double dDivisor = (double)divisor;
	double dLimb = 0x80000000;
	//pDividend += nbrLen - 1;
	for (ctr = nbrLen - 1; ctr >= 0; ctr--)
	{
		unsigned int quotient, dividend;
		double dQuotient, dDividend;
		dividend = (remainder << BITS_PER_INT_GROUP) + Dividend[ctr];
		dDividend = (double)remainder * dLimb + Dividend[ctr];
		dQuotient = floor(dDividend / dDivisor + 0.5);
		quotient = (unsigned int)dQuotient;   // quotient has correct value or 1 more.
		remainder = dividend - quotient * divisor;
		if ((unsigned int)remainder >= (unsigned int)divisor)
		{     // remainder not in range 0 <= remainder < divisor. Adjust.
			quotient--;
			remainder += divisor;
		}
		//pDividend--;
	}
	return remainder;
}

/* Prod = Fact1*Fact2 */
void MultBigNbr(const int Fact1[], const int Fact2[], int Prod[], int nbrLen)
{
	double dRangeLimb = (double)(1U << BITS_PER_INT_GROUP);
	double dInvRangeLimb = 1 / dRangeLimb;
	int low = 0;
	int i, j,k=0 ;
	int factor1, factor2;
	double dAccumulator = 0;
	for (i = 0; i < nbrLen; i++)
	{
		for (j = 0; j <= i; j++)
		{
			factor1 = Fact1[j];
			factor2 = Fact2[i - j];
			low += factor1*factor2;
			dAccumulator += (double)factor1 * (double)factor2;
		}
		low &= MAX_INT_NBR;    // Trim extra bits.
		Prod[i] = low;
		// Subtract or add 0x20000000 so the multiplication by dVal is not nearly an integer.
		// In that case, there would be an error of +/- 1.
		if (low < HALF_INT_RANGE)
		{
			dAccumulator = floor((dAccumulator + HALF_INT_RANGE / 2)*dInvRangeLimb);
		}
		else
		{
			dAccumulator = floor((dAccumulator - HALF_INT_RANGE / 2)*dInvRangeLimb);
		}
		low = (int)(dAccumulator - floor(dAccumulator * dInvRangeLimb) * dRangeLimb);
	}
	Prod[i] = low;
	Prod[i + 1] = (int)floor(dAccumulator / dRangeLimb);
}

/* bigNbr = value */
void IntToBigNbr(int value, int bigNbr[], int nbrLength)
{
	if (value >= 0)
	{     // value is positive.
		bigNbr[0] = value;
		value = 0;
	}
	else
	{     // value is negative.
		bigNbr[0] = value & MAX_INT_NBR;
		value = MAX_INT_NBR;
	}
	for (int i = 1; i < nbrLength; i++)
	{
		bigNbr[i] = value;
	}
}

/* BigNbr = BigInt. Return value is number of limbs */
int BigIntToBigNbr(const BigInteger &BigInt, int BigNbr[])
{
	/*int nbrLenBigNbr = BigInt.nbrLimbs;
	const limb *Limbs = BigInt.limbs;*/
	memcpy(BigNbr, BigInt.limbs, BigInt.nbrLimbs * sizeof(int));
	return BigInt.nbrLimbs;
}

/* BigInt = BigNum */
void BigNbrToBigInt(BigInteger &pBigInt, const int BigNum[], int nbrLenBigNum)
{
	int nbrLimbs;

	limb *Limbs = pBigInt.limbs;
	pBigInt.sign = SIGN_POSITIVE;
	memcpy(Limbs, BigNum, nbrLenBigNum * sizeof(int));

	nbrLimbs = nbrLenBigNum;
	do 	{
		if (Limbs[nbrLimbs-1].x != 0) {
			break;
		}
	} while (--nbrLimbs > 1);
	pBigInt.nbrLimbs = nbrLimbs;
}

void GcdBigNbr(const int *pNbr1, const int *pNbr2, int *pGcd, int nbrLen)
{
	static BigInteger BigInt1, BigInt2, BigGcd;   // follow recommendation not to use stack

	BigNbrToBigInt(BigInt1, pNbr1, nbrLen);
	BigNbrToBigInt(BigInt2, pNbr2, nbrLen);
	BigIntGcd(BigInt1, BigInt2, BigGcd);
	memset(pGcd, 0, NumberLength * sizeof(int));
	BigIntToBigNbr(BigGcd, pGcd);
}

// Compute Nbr <- Nbr mod Mod. Note that both Nbr & Mod have the same length nbrLen
static void AdjustBigNbrModN(int *Nbr, const int *Mod, int nbrLen) {
	/* note use of cast to change ints to limbs! */
	AdjustModN((limb *)Nbr, (limb *)Mod, nbrLen);  
}

/* Prod = Nbr1*Nbr2 (mod Mod) */
void MultBigNbrModN(const int Nbr1[], const int Nbr2[], int Prod[], const int Mod[], 
	int nbrLen) {
	int i;
	int arr[MAX_LIMBS_SIQS];

	if (nbrLen >= 2 && Mod[nbrLen - 1] == 0) {
		nbrLen--;
	}
	(int)Nbr2[nbrLen] = 0;   // // 'value' of Nbr2 is not changed
	memset(Prod, 0, nbrLen * sizeof(Prod[0]));
	i = nbrLen;
	do {
		int Nbr = Nbr1[--i];
		int j = nbrLen;
		do {
			Prod[j] = Prod[j - 1];
		} while (--j > 0);
		Prod[0] = 0;
		AdjustBigNbrModN(Prod, Mod, nbrLen);
		MultBigNbrByIntModN(Nbr2, Nbr, arr, Mod, nbrLen);
		AddBigNbrModN(arr, Prod, Prod, Mod, nbrLen);
	} while (i > 0);
}
void MultBigNbrModN(Znum Nbr1, Znum Nbr2, Znum Prod, const Znum Mod) {
	Prod = Nbr1*Nbr2;
	mpz_mod(ZT(Prod), ZT(Prod), ZT(Mod));
}

/* Prod = Nbr1*Nbr2 (mod Mod) */
void MultBigNbrByIntModN(const int Nbr1[], int Nbr2, int Prod[], const int Mod[], int nbrLen)
{
	if (nbrLen >= 2 && *(Mod + nbrLen - 1) == 0) {
		nbrLen--;
	}
	(int)Nbr1[nbrLen] = 0;  // �value' of Nbr1 is not changed
	modmultIntExtended((limb *)Nbr1, Nbr2, (limb *)Prod, (limb *)Mod, nbrLen);
}
void MultBigNbrByIntModN(Znum Nbr1, int Nbr2, Znum Prod, const Znum Mod) {
	Prod = Nbr1*Nbr2;
	mpz_mod(ZT(Prod), ZT(Prod), ZT(Mod));
}

/* calculate NbrMod^Expon%currentPrime */
int intDoubleModPow (int NbrMod, int Expon, int currentPrime) {
	double Power = 1;
	double Square = NbrMod;
	double Modulus = currentPrime;
	while (Expon != 0) {
		if ((Expon & 1) == 1) {
			Power *= Square;
			Power -= floor(Power / Modulus)*Modulus;
		}
		Square *= Square;
		Square -= floor(Square / Modulus)*Modulus;
		Expon >>= 1;
	}
	return (int)Power;
}

/* get modular inverse of num wrt mod*/
void ModInvBigNbr(int *num, int *inv, int *mod, int nbrLenBigInt)
{
	int NumberLengthBigInt;
	int NumberLengthBak = NumberLength;
	static BigInteger Numerator, Denominator, Modulus, Quotient;  // follow recommendation not to use stack
	memset(inv, 0, nbrLenBigInt * sizeof(int));
	while (nbrLenBigInt > 1) {
		if (*(mod + nbrLenBigInt - 1) != 0) {
			break;
		}
		nbrLenBigInt--;
	}
	NumberLength = nbrLenBigInt;
	TestNbrBI.nbrLimbs = nbrLenBigInt;
	memcpy(TestNbr, mod, NumberLength * sizeof(limb));
	TestNbr[NumberLength].x = 0;
	TestNbrBI.sign = SIGN_POSITIVE;
	GetMontgomeryParms(NumberLength);
	BigNbrToBigInt(Denominator, num, nbrLenBigInt);
	BigNbrToBigInt(Modulus, mod, nbrLenBigInt);
	Numerator.sign = SIGN_POSITIVE;
	Numerator.nbrLimbs = 1;
	Numerator.limbs[0].x = 1;    // Numerator <- 1.
	BigIntModularDivision(Numerator, Denominator, Modulus, Quotient);
	NumberLengthBigInt = BigIntToBigNbr(Quotient, inv);
	NumberLength = NumberLengthBak;
	if (NumberLengthBigInt < NumberLength)
	{
		memset(inv + NumberLengthBigInt, 0, (NumberLength - NumberLengthBigInt) * sizeof(int));
	}
	return;
}