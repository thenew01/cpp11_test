#include <iostream>
#include <memory>
#include <tchar.h>

using namespace std;

void foo(std::shared_ptr<int> i)
{
	(*i)++;
}

struct A;
struct B;

struct A {
	//std::shared_ptr<B> pointer;
	std::weak_ptr<B> pointer;
	~A() {
		std::cout << "A 被销毁" << std::endl;
	}
};
struct B {
	std::shared_ptr<A> pointer;
	~B() {
		std::cout << "B 被销毁" << std::endl;
	}
};

int main()
{
	// auto pointer = new int(10); // 非法, 不允许直接赋值
	// 构造了一个 std::shared_ptr
	auto pointer = std::make_shared<int>(10);
	foo(pointer);
	std::cout << *pointer << std::endl; // 11

	auto pointer2 = pointer;    // 引用计数+1
	auto pointer3 = pointer;    // 引用计数+1
	int *p = pointer.get();             // 这样不会增加引用计数
				
	std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;      // 3
	std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;    // 3
	std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;    // 3

	pointer2.reset();
	std::cout << "reset pointer2:" << std::endl;
	std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;      // 2
	std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;    // 0, pointer2 已 reset
	std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;    // 2

	pointer3.reset();
	std::cout << "reset pointer3:" << std::endl;
	std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;      // 1
	std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;    // 0
	std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;    // 0, pointer3 已 reset
								
	std::unique_ptr<int> pointer0 = std::make_unique<int>(10);   // make_unique 从 C++14 引入
	std::unique_ptr<int> pointer20 = std::move(pointer0);    // 非法


	
	{
		auto a = std::make_shared<A>();
		auto b = std::make_shared<B>();
		a->pointer = b;
		b->pointer = a;
	}

	// 离开作用域前，shared_ptr 会被析构，从而释放内存
	return 0;
}