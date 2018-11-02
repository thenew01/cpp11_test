#include <iostream>

#include <utility>  // std::move
#include <vector>   // std::vector
#include <string>   // std::string

void reference(std::string& str) {
	std::cout << "左值" << std::endl;
}
void reference(std::string&& str) {
	std::cout << "右值" << std::endl;
}



//移动语义
class A {
public:
	int *pointer;
	A() :pointer(new int(1)) { std::cout << "构造" << pointer << std::endl; }
	A(A& a) :pointer(new int(*a.pointer)) { std::cout << "拷贝" << pointer << std::endl; }    // 无意义的对象拷贝
	A(A&& a) :pointer(a.pointer) { a.pointer = nullptr; std::cout << "移动" << pointer << std::endl; }
	~A() { 
		std::cout << "析构" << pointer << std::endl; 
		delete pointer; 
	}
};
// 防止编译器优化
A return_rvalue(bool test) {
	A a, b;
	if (test) return a;
	else return b;
}
int main() {
	std::string  lv1 = "string,";       // lv1 是一个左值
										// std::string&& r1 = s1;           // 非法, 右值引用不能引用左值
	std::string&& rv1 = std::move(lv1); // 合法, std::move可以将左值转移为右值
	std::cout << rv1 << std::endl;      // string,

	const std::string& lv2 = lv1 + lv1; // 合法, 常量左值引用能够延长临时变量的申明周期
										// lv2 += "Test";                   // 非法, 引用的右值无法被修改
	std::cout << lv2 << std::endl;      // string,string

	std::string&& rv2 = lv1 + lv2;      // 合法, 右值引用延长临时对象声明周期
	std::string&& rv3 = "a";
	reference(rv2);                     // 输出左值
	reference(rv3);                     // 输出左值

	rv2 += "Test";                      // 合法, 非常量引用能够修改临时变量
	std::cout << rv2 << std::endl;      // string,string,string,

	reference(rv2);                     // 输出左值

	//注意：rv2 虽然引用了一个右值，但由于它是一个引用，所以 rv2 依然是一个左值。

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	//函数返回后，产生一个将亡值，被 A 的移动构造（A(A&&)）引用，从而延长生命周期，
	// 并将这个右值中的指针拿到，保存到了 obj 中，
	// 而将亡值的指针被设置为 nullptr，防止了这块内存区域被销毁。
	A obj = return_rvalue(false);
	std::cout << "obj:" << std::endl;
	std::cout << obj.pointer << std::endl;
	std::cout << *obj.pointer << std::endl;


	std::string str = "Hello world.";
	std::vector<std::string> v;

	// 将使用 push_back(const T&), 即产生拷贝行为
	v.push_back(str);
	// 将输出 "str: Hello world."
	std::cout << "str: " << str << std::endl;

	// 将使用 push_back(const T&&), 不会出现拷贝行为
	// 而整个字符串会被移动到 vector 中，所以有时候 std::move 会用来减少拷贝出现的开销
	// 这步操作后, str 中的值会变为空
	v.push_back(std::move(str));
	// 将输出 "str: "
	std::cout << "str: " << str << std::endl;

	return 0;
}