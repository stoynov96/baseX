#include <string>
#include <fstream>

const int BASEX_DEFAULT_BASE = 10;

const char BASEX_DEFAULT_VALUE = '0';
const unsigned short BASEX_DEFAULT_NUMBER_OF_DIGITS = 1;

const short BASEX_MAX_BASE = 100;
const short BASEX_MAX_NUMBER_OF_DIGITS = 50;
const string BASEX_DEFAULT_DIGITS_FILENAME = "baseDigits.txt";
const char BASEX_NUMBER_SIGNS[] = {'-','\0',};

const unsigned short BASEX_DEFAULT_DIGITS_SIZE_INCREMENT = 3;

/*
class baseXHelper
Class that will hold constants, initialize and more
*/
class baseXHelper {
public:

	friend class baseX;

	//variables
	//functions
	baseXHelper();
	~baseXHelper();

private:
	//variables
	
	char *digits;
	
	//functions
	void SetArraySize(char*&, unsigned short,unsigned short,char defValue='0') const;
	void IncreaseArraySize(char*&,unsigned short&,unsigned short) const;

	void GetDigits(ifstream&);
	void GetDigits(string);

	baseX AddLongerToShorter(const baseX&,const baseX&) const;

};
const baseXHelper baseXHelp;



/*
class baseX
Class that will take a value and a base, with defined characters for digits
*/
class baseX {
public:

	friend class baseXHelper;
	//variables

	//functions
	baseX();
	baseX(string);
	baseX(int, string);
	baseX(const baseX&);
	~baseX();

	int GetBase() const;
	string GetValue() const;
	void GetDigitValues(unsigned short[], unsigned short&) const;
	bool GetIsPositive() const;

	baseX AbsVal() const;
	baseX Opposite() const;

	void Display() const;

	bool operator==(const baseX&) const;
	bool operator>=(const baseX&) const;
	bool operator>(const baseX&) const;
	bool operator<=(const baseX&) const;
	bool operator<(const baseX&) const;

	void operator=(const baseX&);
	baseX operator+(const baseX&) const;
	baseX operator-(const baseX&) const;
	baseX operator*(const baseX&) const;
	baseX operator/(const baseX&) const;
	baseX operator%(const baseX&) const;

private: 
	//variables
	int base;
	char *value;
	unsigned short digitValues[BASEX_MAX_NUMBER_OF_DIGITS];
	unsigned short numberOfDigits;
	bool isPositive;

	//char *digits;

	//functions

	//working with value
	void SetValue(string);				//ONLY USE THIS IF THERE IS NO VALUE
	void SetValue(char*,unsigned short);
	void ReSetValue(string);			//ONLY USE THIS IF THERE ALREADY IS A VALUE
	void ReSetValue(char*,unsigned short);
	void AttachToValue(string);
	void PrintValue() const;

	void SwapWith(baseX&);

	bool Borrow(unsigned short);

	//Addition and Subtraction helpers
	baseX UnsignedAddition(baseX) const;
	baseX UnsignedSubtraction(baseX) const;

	//Multiplication Helpers
public:
	baseX MultipliedByBase() const;
	baseX MultipliedByDigit(unsigned short) const;
private:
	
	//Division Helpers
	baseX TakeFirstDigits(unsigned short) const;

	void ChangeNumberOfDigits(unsigned short newNumber);
	void AddZeroes(unsigned short);
	void OptimizeSize();			//removes zeroes from the beginning

	baseX OfBase(unsigned short) const;

	void SeperateSignAndValue();
	void GetValueFromDecimalDigits();
	void GetDecimalValuesOfDigits();
};