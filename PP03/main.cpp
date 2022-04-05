#include <iostream>
using namespace std;

class A
{
public:
	virtual void dynamic_print()
	{
		cout << "´ÙÀÌ³ª¹ÌÅ© µ¥Çò\n";
	}

	void static_print()
	{
		cout << "µ¥Çì\n";
	}
};

class B : public A
{
public :
	void dynamic_print()
	{
		cout << "´ÙÀÌ³ª¹ÌÅ© ÇìÂm\n";
	}
	void static_print()
	{
		cout << "ÇìÂm\n";
	}

	void static_print(int a)
	{
		cout << a <<"ÇìÂm\n";
	}
};



void main()
{
	A* tempA;
	B* tempB = new B();
	tempA = tempB;
	tempA->static_print();
	tempB->static_print();

	tempA->dynamic_print();
	tempB->dynamic_print();
}