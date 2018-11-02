
#include <iostream>
using namespace std;

class Base {
public:
	int value1;
	int value2;
	Base() {
		value1 = 1;
	}
	Base(int value) : Base() {  // ί�� Base() ���캯��
		value2 = 2;
	}
};

class Subclass : public Base {
public:
	using Base::Base;          // �̳й���
};

struct Base0 {
	virtual void foo(int);
};
struct SubClass : Base0 {
	virtual void foo(int) override; // �Ϸ�
	//virtual void foo(float) override; // �Ƿ�, ����û�д��麯��
};

struct Base1 {
	virtual void foo() final;
};
struct SubClass1 final : Base1 {
};                  // �Ϸ�

struct SubClass2 : SubClass1 {
};                  // �Ƿ�, SubClass1 �� final

struct SubClass3 : Base1 {
	void foo(); // �Ƿ�, foo �� final
};


using namespace std;
class Test1
{
public:
	Test1(int num) :n(num) {}
private:
	int n;
};
class Test2
{
public:
	explicit Test2(int num) :n(num) {}
private:
	int n;
};

int main() {

	Base b(2);
	std::cout << b.value1 << std::endl;
	std::cout << b.value2 << std::endl;

	Subclass s(3);
	std::cout << s.value1 << std::endl;
	std::cout << s.value2 << std::endl;

	Test1 t1 = 12;
	Test2 t2(13);
	Test2 t3 = 14;	//����ʼ����: �޷��ӡ�int��ת��Ϊ��Test2��
}
