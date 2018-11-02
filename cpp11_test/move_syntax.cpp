#include <iostream>

#include <utility>  // std::move
#include <vector>   // std::vector
#include <string>   // std::string

void reference(std::string& str) {
	std::cout << "��ֵ" << std::endl;
}
void reference(std::string&& str) {
	std::cout << "��ֵ" << std::endl;
}



//�ƶ�����
class A {
public:
	int *pointer;
	A() :pointer(new int(1)) { std::cout << "����" << pointer << std::endl; }
	A(A& a) :pointer(new int(*a.pointer)) { std::cout << "����" << pointer << std::endl; }    // ������Ķ��󿽱�
	A(A&& a) :pointer(a.pointer) { a.pointer = nullptr; std::cout << "�ƶ�" << pointer << std::endl; }
	~A() { 
		std::cout << "����" << pointer << std::endl; 
		delete pointer; 
	}
};
// ��ֹ�������Ż�
A return_rvalue(bool test) {
	A a, b;
	if (test) return a;
	else return b;
}
int main() {
	std::string  lv1 = "string,";       // lv1 ��һ����ֵ
										// std::string&& r1 = s1;           // �Ƿ�, ��ֵ���ò���������ֵ
	std::string&& rv1 = std::move(lv1); // �Ϸ�, std::move���Խ���ֵת��Ϊ��ֵ
	std::cout << rv1 << std::endl;      // string,

	const std::string& lv2 = lv1 + lv1; // �Ϸ�, ������ֵ�����ܹ��ӳ���ʱ��������������
										// lv2 += "Test";                   // �Ƿ�, ���õ���ֵ�޷����޸�
	std::cout << lv2 << std::endl;      // string,string

	std::string&& rv2 = lv1 + lv2;      // �Ϸ�, ��ֵ�����ӳ���ʱ������������
	std::string&& rv3 = "a";
	reference(rv2);                     // �����ֵ
	reference(rv3);                     // �����ֵ

	rv2 += "Test";                      // �Ϸ�, �ǳ��������ܹ��޸���ʱ����
	std::cout << rv2 << std::endl;      // string,string,string,

	reference(rv2);                     // �����ֵ

	//ע�⣺rv2 ��Ȼ������һ����ֵ������������һ�����ã����� rv2 ��Ȼ��һ����ֵ��

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	//�������غ󣬲���һ������ֵ���� A ���ƶ����죨A(A&&)�����ã��Ӷ��ӳ��������ڣ�
	// ���������ֵ�е�ָ���õ������浽�� obj �У�
	// ������ֵ��ָ�뱻����Ϊ nullptr����ֹ������ڴ��������١�
	A obj = return_rvalue(false);
	std::cout << "obj:" << std::endl;
	std::cout << obj.pointer << std::endl;
	std::cout << *obj.pointer << std::endl;


	std::string str = "Hello world.";
	std::vector<std::string> v;

	// ��ʹ�� push_back(const T&), ������������Ϊ
	v.push_back(str);
	// ����� "str: Hello world."
	std::cout << "str: " << str << std::endl;

	// ��ʹ�� push_back(const T&&), ������ֿ�����Ϊ
	// �������ַ����ᱻ�ƶ��� vector �У�������ʱ�� std::move ���������ٿ������ֵĿ���
	// �ⲽ������, str �е�ֵ���Ϊ��
	v.push_back(std::move(str));
	// ����� "str: "
	std::cout << "str: " << str << std::endl;

	return 0;
}