#include "baseX.h"

/*
class baseXHelper
Class that will hold constants, initialize and more
*/

//=====CONSTRUCTORS AND DESTRUCTOR=====


baseXHelper::baseXHelper() {
	GetDigits(BASEX_DEFAULT_DIGITS_FILENAME);
}
baseXHelper::~baseXHelper() {
	delete [] digits;
}

//=====OTHER FUNCTIONS=====

//=====PUBLIC=====

//=====PRIVATE=====
void baseXHelper::SetArraySize(char* &arr, unsigned short oldSize, unsigned short newSize, const char defaultValue) const {


	if(oldSize == newSize) return;

	char* newArr = new char[newSize];

	//TODO: Replace with a more efficient method, like memcpy
	
	if(oldSize > newSize) {
		for(unsigned short i = 0; i < newSize; i ++) {
			newArr[i] = arr[i];
		}
	}
	else {
		for(unsigned short i = 0; i < oldSize; i ++) {
			newArr[i] = arr[i];
		}
		for(unsigned short i = oldSize; i < newSize; i ++) {
			newArr[i] = defaultValue;
		}
	}

	delete [] arr;
	arr = newArr;
}

void baseXHelper::IncreaseArraySize(char* &arr, unsigned short& arrSize, unsigned short sizeIncrement) const {
	char* newArr = new char[arrSize * sizeIncrement];

	//TODO: Replace with a more efficient method, like memcpy
	for (unsigned short i = 0; i < arrSize; i ++) {
		newArr[i] = arr[i];
	}
	delete [] arr;
	arrSize *= sizeIncrement;
	arr = newArr;
}

void baseXHelper::GetDigits(ifstream &inFile) {
	/*
	Loads a series of digits into the digits[] array. The digits are to be used by all baseX objects
	*/
	unsigned short currentDigitsSize = 1;
	digits = new char[currentDigitsSize];

	int index = 0;
	while (!inFile.eof()) {
		if(index >= currentDigitsSize) {	//increasing the size of the array digits[]
			IncreaseArraySize(digits,currentDigitsSize,BASEX_DEFAULT_DIGITS_SIZE_INCREMENT);
		}
		char currentChar = ' ';
		inFile >> currentChar;
		if (currentChar != ' ' && currentChar != '\n') {	//make sure the character is not white space
			digits[index] = currentChar;
		}
		index ++;
	}
	SetArraySize(digits,currentDigitsSize,index);
}
void baseXHelper::GetDigits(string inFileName) {
	ifstream inFile;
	inFile.open(inFileName.c_str());
	GetDigits(inFile);
	inFile.close();
}

baseX baseXHelper::AddLongerToShorter(const baseX &a, const baseX &b) const {
	baseX result = a;

	bool carry = false;
	short currentDigitSum = 0;

	for (short j = 1; j <= b.numberOfDigits; j ++) {
		short aIndex = a.numberOfDigits - j;
		short bIndex = b.numberOfDigits - j;

		currentDigitSum = a.digitValues[aIndex] + b.digitValues[bIndex] + carry;

		carry = false;				//determining carry for the next pair of digits
		if (currentDigitSum >= a.base) {
			carry = true;
			currentDigitSum -= a.base;
		}

		result.digitValues[aIndex] = currentDigitSum;
	}

	//TODO: Fix for a carry at the end and a 9 at the index
	//			Example of this: 999999 + 21
	short lastIndex = a.numberOfDigits - b.numberOfDigits - 1;
	if (carry) {
		if (lastIndex < 0) {	//if there is a carry that will need to be a new digit in front
			lastIndex ++;
			result.ChangeNumberOfDigits(result.numberOfDigits + 1);
		}

		result.digitValues[lastIndex] ++;
	}

	result.GetValueFromDecimalDigits();
	return result;
}








/*
class baseX
Class that will take a value and a base, with defined characters for digits
*/

//=====CONSTRUCTORS AND DESTRUCTOR=====

baseX::baseX() {
	isPositive = true;

	base = BASEX_DEFAULT_BASE;
	
	//setting value to the default
	value = new char[BASEX_DEFAULT_NUMBER_OF_DIGITS];
	value[0] = BASEX_DEFAULT_VALUE;
	numberOfDigits = BASEX_DEFAULT_NUMBER_OF_DIGITS;

	SeperateSignAndValue();
	GetDecimalValuesOfDigits();
	OptimizeSize();
}
baseX::baseX(string newValue) {
	isPositive = true;

	SetValue(newValue);
	
	base = BASEX_DEFAULT_BASE;

	SeperateSignAndValue();
	GetDecimalValuesOfDigits();
	OptimizeSize();
}
baseX::baseX(int newBase, string newValue) {
	isPositive = true;

	base = newBase;
	SetValue(newValue);

	SeperateSignAndValue();
	GetDecimalValuesOfDigits();
	OptimizeSize();
}
baseX::baseX(const baseX& b) {
	this->value = new char[BASEX_DEFAULT_NUMBER_OF_DIGITS];
	*this = b;
}
baseX::~baseX() {
	delete [] value;
}

//=====OTHER FUNCTIONS=====

void baseX::SetValue(string newValue) {
	unsigned short newValueSize = newValue.size();
	numberOfDigits = newValueSize;

	value = new char[newValueSize];
	//TODO: Optimize with memcpy
	for(unsigned short i = 0; i < newValueSize; i ++) {
		value[i] = newValue[i];
	}
}
void baseX::SetValue(char newValue[], unsigned short newValueSize) {
	numberOfDigits = newValueSize;

	value = new char [newValueSize];
	//TODO: Optimize with memcpy
	for(unsigned short i = 0; i < newValueSize; i ++) {
		value[i] = newValue[i];
	}
}

void baseX::ReSetValue(string newValue) {
	delete [] value;
	SetValue(newValue);
}
void baseX::ReSetValue(char newValue[], unsigned short newValueSize) {
	delete [] value;
	SetValue(newValue,newValueSize);
}

void baseX::AttachToValue(string attachment) {
	/*
	The function increases the value of numberOfDigits. Must not do it seperately
	*/
	unsigned short newValueSize = numberOfDigits + attachment.size();
	char *newValue = new char[newValueSize];

	//TODO: Optimize with memcpy
	for(unsigned short i = 0; i < numberOfDigits; i ++) {
		newValue[i] = value[i];
	}
	for(unsigned short i = numberOfDigits; i < newValueSize; i ++) {
		newValue[i] = attachment[i - numberOfDigits];
	}

	numberOfDigits = newValueSize;

	delete [] value;
	value = newValue;
}
char *baseX::GetValueSubstring(unsigned short startIndex, unsigned short length) {
	if(startIndex + length > numberOfDigits) {	//check if the desired substring is greater than the size of value
		length = numberOfDigits - startIndex;
	}
	char *newValue = new char[length];
	//TODO: Replace with memcpy
	for(unsigned short i = 0; i < length; i ++) {
		newValue[i] = value[startIndex + i];
	}
	delete [] value;
	value = newValue;
}
void baseX::PrintValue() const {
	for(unsigned short i = 0; i < numberOfDigits; i ++) {
		cout << value[i];
	}
}

void baseX::SwapWith(baseX& b) {
	baseX temp = *this;
	*this = b;
	b = temp;
}

bool baseX::Borrow(unsigned short index) {
	
	if (index <= 0) return false;	//cannot borrow. No digits to the left

	unsigned short i = index;
	
	while (i > 0) {
		i--;
		if(digitValues[i] == 0) {
			digitValues[i] = base - 1;
		}
		else {
			digitValues[i] --;
			digitValues[index] += base;
			return true;			//can borrow, and have borrowed
		}
	}
	return false;
}

baseX baseX::UnsignedAddition(baseX b) const {
	baseX result(base, "");

	if (this->numberOfDigits > b.numberOfDigits) {
		result = baseXHelp.AddLongerToShorter(this->AbsVal(),b.AbsVal());
	}

	if (this->numberOfDigits <= b.numberOfDigits) {
		result = baseXHelp.AddLongerToShorter(b.AbsVal(),this->AbsVal());
	}

	return result;
}

baseX baseX::UnsignedSubtraction(baseX b) const {
	baseX a = this->AbsVal();
	b = b.AbsVal();
	baseX result;

	if(a<b) {
		a.SwapWith(b);		//making sure a is the bigger number and b is the smaller number
	}

	result = a;


	b.ChangeNumberOfDigits(result.numberOfDigits);

	bool validOperation = true;

	for(unsigned short i = 0; i < a.numberOfDigits; i ++) {
		unsigned short indexThis = a.numberOfDigits - 1 - i;
		unsigned short indexB = b.numberOfDigits - 1 - i;								//getting the current digit index of both numbers
		
		if(a.digitValues[indexThis] < b.digitValues[indexB]) {
			validOperation = a.Borrow(indexThis);	//borrowing from the left if needed. Storing whether we can borrow in validOperation
		}
		if(!validOperation) {
			baseX invalid(2,"invalid operation");
			return invalid;
		}

		result.digitValues[indexThis] = a.digitValues[indexThis] - b.digitValues[indexB];
	}
	result.OptimizeSize();
	result.GetValueFromDecimalDigits();

	return result;
}

baseX baseX::MultipliedByBase() const {
/*
	Multiplies the number by its base by just adding a zero at the end
*/

	baseX newNumber(*this);
	newNumber.AttachToValue("0");					//adding the new zero to the string

	newNumber.digitValues[numberOfDigits] = 0;		//making the new digit zero

	return newNumber;
}

baseX baseX::MultipliedByDigit(unsigned short dVal) const {
/*
	parameters:
		dVal - value of the digit that is being passed (short for digitValue)
*/
	baseX result = *this;
	unsigned short currentDigitProduct = 1;
	unsigned short carry = 0;
	for (short i = numberOfDigits - 1; i >= 0; i --) {
		currentDigitProduct = this->digitValues[i] * dVal + carry;
		carry = 0;

		if(currentDigitProduct >= this->base) {
			carry = currentDigitProduct / base;		//getting the value of the first digit of the base (this->base) number that is currentDigitProduct
			currentDigitProduct %= base;
		}
		result.digitValues[i] = currentDigitProduct;
	}
	if(carry > 0) {
		result.ChangeNumberOfDigits(result.numberOfDigits + 1);
		result.digitValues[0] = carry;
	}
	result.GetValueFromDecimalDigits();

	return result;
}

baseX baseX::DividePositives(const baseX &a, const baseX &b) const {
	/*
	Algorithm:
	1)Find minuend - check if the current one is less than b. If so:
		a)append the next digit from *this
		b)move to the next digit for any subsequent operations
	2)Find x in (b * x = minuend)
		a)find the current digit (x) from this division opereation
		b)apend that digit to the result value
		c)find the subtrahend by multiplying b by x
	3)Subtract
		a)find the difference between the minuend and the subtrahend
		b)set the minuent to be the difference for the next operation
	4)Move to the next itteration of the cycle
	*/

	if(a<b) {
		baseX numberZero(a.base,"0");
		return numberZero;
	}

	//Algorithm starts here....
	baseX result(a.base,"");

	//=======We start by doing the first operation, then we cycle all the remaining ones========

	unsigned short divisionDigitValue;			//this is the largest possible digit that the divisor can be multiplied by
	baseX intermediateMinuend;					//the number we will be subtracting from
	baseX intermediateSubtrahend;				//the number that will be subtracted
	baseX intermediateDifference;				//the difference between the two
	unsigned short nextDigitIndex;				//the index of the digit that should be accessed next

	//Initializations
	intermediateMinuend = a.TakeFirstDigits(b.numberOfDigits);
	nextDigitIndex = b.numberOfDigits;

	while(nextDigitIndex < a.numberOfDigits) {
		//===(1)===
		if(intermediateMinuend < b) {
			//==(a)==
			intermediateMinuend.ApendDigit(a.digitValues[nextDigitIndex]);
			//==(b)==
			nextDigitIndex ++;
		}
		//===(2)===
		//	==(a)==
		divisionDigitValue = FindDivisionDigitValue(intermediateMinuend, b);
		//	==(b)==
		result.ApendDigit(divisionDigitValue);
		//	==(c)==
		intermediateSubtrahend = b.MultipliedByDigit(divisionDigitValue);
		//===(3)===
		//	==(a)==
		intermediateDifference = intermediateMinuend - intermediateSubtrahend;
		//	==(b)==
		intermediateMinuend = intermediateDifference;
		//===(4)====
	}


	return result;
}

baseX baseX::TakeFirstDigits(unsigned short numDigsTaken) const {
	//TODO: See if we can just use the copy constructor
	baseX newNumber(this->base,this->value);
	newNumber.isPositive = this->isPositive;

	newNumber.numberOfDigits = numDigsTaken;
	for(unsigned short i = 0; i < numDigsTaken; i ++) {
		newNumber.digitValues[i] = this->digitValues[i];
	}
	newNumber.GetValueFromDecimalDigits();

	return newNumber;
}
void baseX::ApendDigit(unsigned short digitValue) {
	digitValues[numberOfDigits] = digitValue;
	numberOfDigits ++;
	GetValueFromDecimalDigits();
}

unsigned short baseX::FindDivisionDigitValue(const baseX& iMinuend, const baseX& baseISubtrahend) const {
	//TODO: Fix for base 1
	//TODO: Do a binary search
	//if(baseISubtrahend > iMinuend) return 0;
	for(unsigned short i = 0; i < base; i ++) {
		if(baseISubtrahend.MultipliedByDigit(i + 1) > iMinuend) {
			return (i);
		}
	}
	return 0;

}




int baseX::GetBase() const {
	return this->base;
}
string baseX::GetValue() const {
	return this->value;
}
void baseX::GetDigitValues(unsigned short digV[], unsigned short &numOfD) const {
	numOfD = this->numberOfDigits;
	for (short i = 0; i < this->numberOfDigits; i ++) {
		digV[i] = digitValues[i];
	}
}
bool baseX::GetIsPositive() const {
	return isPositive;
}

baseX baseX::AbsVal() const {
	baseX a = *this;
	a.isPositive = true;
	return a;
}

baseX baseX::Opposite() const {
	baseX a = *this;
	a.isPositive = !(a.isPositive);
	return a;
}

void baseX::ChangeNumberOfDigits(unsigned short newNumberOfD) {
	baseX oldNumber(*this);
	short numberIncrement = newNumberOfD - this->numberOfDigits;
	if(numberIncrement > 0) {	//if we want to have more digits
		for (short i = 0; i < oldNumber.numberOfDigits; i ++) {		//shifting the digits
			this->digitValues[i + numberIncrement] = oldNumber.digitValues[i];
		}
		for (short i = 0; i < numberIncrement; i ++) {		//adding zeroes in front (or whatever the first digit of the base is)
			this->digitValues[i] = 0;
			this->AttachToValue("0");		//This does NOT properly get the value. Its purpose is to keep the size of the variable value adequate.
		}
		this->numberOfDigits = newNumberOfD;
	}

	this->GetValueFromDecimalDigits();
}

void baseX::AddZeroes(unsigned short zeroesCount) {
	for (unsigned short i = 0; i < zeroesCount; i ++) {
		*this = this->MultipliedByBase();
	}
}

void baseX::OptimizeSize() {
/*
	TODO: Optimize this algorithm. This is a temporary version
*/
	if (this->value[0] == '0' && this->numberOfDigits == 1) return;			//we do not want to remove the only digit of the number
	if (this->digitValues[0] != 0) return;	//no need for unnececary steps if the number does not start with a zero

	unsigned short leadingZeroesCount = 0;
	for (unsigned short i = 1; i < this->numberOfDigits; i ++) {
		if(digitValues[i] != 0) {
			leadingZeroesCount = i;
			break;
		}			
	}
	this->numberOfDigits = this->numberOfDigits - leadingZeroesCount;
	for (unsigned short i = 0; i < this->numberOfDigits; i ++) {
		digitValues[i] = digitValues[i + leadingZeroesCount];
	}
	this->GetValueFromDecimalDigits();
}

baseX baseX::OfBase(unsigned short newBase) const {
	//======CASE1: Switching to a lower base=====
	if(this->base > newBase) {

	}

	//======CASE2: Switching to a higher base=====
	//TODO
}

void baseX::GetValueFromDecimalDigits() {
	delete [] value;
	value = new char[numberOfDigits];
	for (int i = 0; i < numberOfDigits; i ++) {
		value[i] = baseXHelp.digits[digitValues[i]];
	}
}

void baseX::GetDecimalValuesOfDigits() {
	for (int i = 0; i < numberOfDigits; i ++) {
		for (int j = 0; j < base; j ++) {
			if (value[i] == baseXHelp.digits[j]) {
				digitValues[i] = j;
			}
		}
	}
}

void baseX::SeperateSignAndValue() {
	isPositive = !(value[0] == BASEX_NUMBER_SIGNS[0]);		//BASEX_NUMBER_SIGNS holds the signs for + and -. [0] is minus, because 0 = false (for isPositive)
	if(!isPositive) {
		value = GetValueSubstring(1);
		numberOfDigits --;
	}
}

void baseX::Display() const {
	cout << "BASE: " << base << endl;
	
	cout << "SIGN: " << BASEX_NUMBER_SIGNS[isPositive] << endl;

	cout << "DIGIT VALUES: ";
	for (int i = 0; i < numberOfDigits; i ++) {
		cout << digitValues[i] << " ";
	}
	cout << endl;

	cout << "VALUE: ";
	PrintValue();
	cout << endl;

	cout <<"FINAL DISPLAY: " << BASEX_NUMBER_SIGNS[isPositive];
	PrintValue();
	cout << "\n\n";
}

//=====OPERATORS=====

bool baseX::operator==(const baseX &b1) const {
	baseX b(b1);
	if (this->numberOfDigits != b.numberOfDigits) return false;		//if they are a different size, they are obviously not equal
	if (this->isPositive != b.isPositive) return false;				//if they have a different sign, they are obviously not equal
	for (int i = 0; i < this->numberOfDigits; i ++) {
		if(this->digitValues[i] != b.digitValues[i]) return false;
	}
	return true;
}
bool baseX::operator>=(const baseX &b1) const {
	baseX b(b1);
	if(!(this->isPositive) && b.isPositive) return false;	//negative is never greater than positive
	if(this->isPositive && !(b.isPositive)) return true;	//positive is always greater than negative

	//======BELOW THIS POINT *this and b have the same sign=====

	if(this->numberOfDigits != b.numberOfDigits) {
		return (this->numberOfDigits > b.numberOfDigits) == this->isPositive;		//formula gotten from a truth table
	}

	for(unsigned short i = 0; i < this->numberOfDigits; i ++) {
		if (this->digitValues[i] != b.digitValues[i]) {
			return (this->digitValues[i] > b.digitValues[i]) == this->isPositive;	//formula gotten from a truth table
		}
	}
	//=====IF NO DIFFERENT DIGITS WERE ENCOUNTERED THE NUMBERS ARE EQUAL=====

	return true;
}
bool baseX::operator>(const baseX &b1) const {
	baseX b(b1);
	if(!(this->isPositive) && b.isPositive) return false;	//negative is never greater than positive
	if(this->isPositive && !(b.isPositive)) return true;	//positive is always greater than negative

	//======BELOW THIS POINT *this and b have the same sign=====

	if(this->numberOfDigits != b.numberOfDigits) {
		return (this->numberOfDigits > b.numberOfDigits) == this->isPositive;		//formula gotten from a truth table
	}

	for(unsigned short i = 0; i < this->numberOfDigits; i ++) {
		if (this->digitValues[i] != b.digitValues[i]) {
			return (this->digitValues[i] > b.digitValues[i]) == this->isPositive;	//formula gotten from a truth table
		}
	}
	//=====IF NO DIFFERENT DIGITS WERE ENCOUNTERED THE NUMBERS ARE EQUAL=====

	return false;
}
bool baseX::operator<=(const baseX &b1) const {
	baseX b(b1);
	if(!(this->isPositive) && b.isPositive) return true;	//negative is always less than positive
	if(this->isPositive && !(b.isPositive)) return false;	//positive is never less than negative

	//======BELOW THIS POINT *this and b have the same sign=====

	if(this->numberOfDigits != b.numberOfDigits) {
		return !((this->numberOfDigits > b.numberOfDigits) == this->isPositive);		//formula gotten from compliment of >= truth table
	}

	for(unsigned short i = 0; i < this->numberOfDigits; i ++) {
		if (this->digitValues[i] != b.digitValues[i]) {
			return !((this->digitValues[i] > b.digitValues[i]) == this->isPositive);	//formula gotten from compliment of >= truth table
		}
	}
	//=====IF NO DIFFERENT DIGITS WERE ENCOUNTERED THE NUMBERS ARE EQUAL=====

	return true;
}
bool baseX::operator<(const baseX &b1) const {
	baseX b(b1);
	if(!(this->isPositive) && b.isPositive) return true;	//negative is always less than positive
	if(this->isPositive && !(b.isPositive)) return false;	//positive is never less than negative

	//======BELOW THIS POINT *this and b have the same sign=====

	if(this->numberOfDigits != b.numberOfDigits) {
		return !((this->numberOfDigits > b.numberOfDigits) == this->isPositive);		//formula gotten from compliment of > truth table
	}

	for(unsigned short i = 0; i < this->numberOfDigits; i ++) {
		if (this->digitValues[i] != b.digitValues[i]) {
			return !((this->digitValues[i] > b.digitValues[i]) == this->isPositive);	//formula gotten from compliment of > truth table
		}
	}
	//=====IF NO DIFFERENT DIGITS WERE ENCOUNTERED THE NUMBERS ARE EQUAL=====

	return false;
}


void baseX::operator=(const baseX &b) {
	this->base = b.base;
	this->ReSetValue(b.value,b.numberOfDigits);
	this->numberOfDigits = b.numberOfDigits;
	this->isPositive = b.isPositive;

	for (int i = 0; i < this->numberOfDigits; i ++) {	//equating digit values
		this->digitValues[i] = b.digitValues[i];
	}
}

baseX baseX::operator+(const baseX &b) const {
	baseX result;

	if(this->isPositive == b.isPositive) result = this->UnsignedAddition(b);
	else result = this->UnsignedSubtraction(b);

	result.isPositive = (b.Opposite() <= *this);	//*this >= b.Opposite()
	return result;
}

baseX baseX::operator-(const baseX &b) const {
	baseX result;

	if(this->isPositive == b.isPositive) result = this->UnsignedSubtraction(b);
	else result = this->UnsignedAddition(b);

	result.isPositive = (b <= *this);	//*this >= b
	return result;
}

baseX baseX::operator*(const baseX &b) const {
	baseX result(this->base,"0");

	for (short i = 0; i < b.numberOfDigits; i ++) {
		short bIndex = b.numberOfDigits - 1 - i;
		baseX intermediateResult = this->MultipliedByDigit(b.digitValues[bIndex]);
		intermediateResult.AddZeroes(i);

		result = result + intermediateResult;
	}

	result.isPositive = (this->isPositive == b.isPositive);
	return result;
}

baseX baseX::operator/(const baseX &b) const {
	return DividePositives(*this,b);
}

baseX baseX::operator%(const baseX &b) const {

}
