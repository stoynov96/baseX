#include <iostream>

#include "baseX.cpp"

using namespace std;

const unsigned short BASE = 10;

void TestAlgebra(baseX&, baseX&);
void TestEquality(baseX&, baseX&);

int main() {
	string s1,s2;
	ifstream inFile;
	inFile.open("input.txt");
	inFile >> s1 >> s2;
	inFile.close();

	baseX num1(BASE,s1);
	baseX num2(BASE,s2);

	num1.MultipliedByBase();
	//num1 = num1.MultipliedByBase();
	num1.Display();

	//TestAlgebra(num1,num2);
	//TestEquality(num1,num2);
}
void TestAlgebra (baseX &num1, baseX &num2) {

	cout << "\n\n";
	num1.Display();
	cout << "\n\n";
	num2.Display();
	cout << "\n=======================\n";

	baseX result;
	result = num1+num2;
	result.Display();
	result = num1-num2;
	result.Display();
	result = num1*num2;
	result.Display();
	result = num1/num2;
	result.Display();
}
void Format(string sign) {
	cout << ("\n" + sign + "\t");
}
void TestEquality (baseX &num1,baseX &num2) {
	Format("==");
	if(num1==num2) {
		cout << "T" << " ";
	}
	else {
		cout << "F" << " ";
	}

	Format(">=");
	if(num1>=num2) {
		cout << "T" << " ";
	}
	else {
		cout << "F" << " ";
	}

	Format(">");
	if(num1>num2) {
		cout << "T" << " ";
	}
	else {
		cout << "F" << " ";
	}

	Format("<=");
	if(num1<=num2) {
		cout << "T" << " ";
	}
	else {
		cout << "F" << " ";
	}

	Format("<");
	if(num1<num2) {
		cout << "T" << " ";
	}
	else {
		cout << "F" << " ";
	}
}