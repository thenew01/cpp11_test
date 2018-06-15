#include "stdafx.h"
#include "cpp11_test.h"

void func(const int& i) {
	std::cout << "func(const int& i) = " << i << std::endl;
}

void func(std::string& s) {
	std::cout << "func(std::string& s) = " << s << std::endl;
}

void func(int&& i) {
	std::cout << "func(int&& i) = " << i << std::endl;
}

/*
无名右值引用
一般由static_cast < T&& >(t)转换操作转换而来
也可以用标准库提供的std::move()来将左值转换成右值引用

带名右值引用
T&& 这是一个左值，只不过她的类型是右值引用，只能绑定右值
额外说一句：如果类型是T&& 且这个T类型无需推导即可确定，那么这个变量称为带名右值引用；如果这个T类型需要推导，那么这个变量称为转发型引用。

1、新标准为c++带来了一个新的左值类型：带名右值引用；带来了一个新的右值类型：无名右值引用
2、左值引用(就是以前的引用)可以绑定左值，也可以绑定右值；右值引用只能绑定右值(新型右值类型(右值引用)、传统右值类型(临时对象))。
*/
/*
什么是移动语义？
可以对比拷贝语义对着看

目的：
移动：接管另一个对象所有外部资源的所有权
拷贝：复制另一个对象所有外部资源，并接管新生资源的所有权

过程：
移动：
释放this资源
将this对象的指针或句柄指向that对象拥有的资源
将that对象中指向资源的指针或句柄置空
拷贝：
释放this资源
拷贝that拥有的资源
将this对象的指针或句柄指向新生资源
如果that对象时临时对象(右值)，在拷贝完之后，that对象会被释放
*/

void rvalue() {
	std::cout << std::endl << std::endl << "-------------------------- rvalue test------------------------" << std::endl;

	int i = 123;
	std::string s = "hello";

	func(i);  // ok
	func(s);  // ok

	func(1);  // error : error C2665: “func”: 2 个重载中没有一个可以转换所有参数类型


	
	++i = 3;  // ok
	//i++ = 3;  // error C2106: “=”: 左操作数必须为左值
}

class MemoryBlock
{
public:

	// 构造器（初始化资源）
	explicit MemoryBlock(size_t length)
		: _length(length)
		, _data(new int[length])
	{
	}

	// 析构器（释放资源）
	~MemoryBlock()
	{
		if (_data != nullptr)
		{
			delete[] _data;
		}
	}

	// 拷贝构造器（实现拷贝语义：拷贝that）
	MemoryBlock(const MemoryBlock& that)
		// 拷贝that对象所拥有的资源
		: _length(that._length)
		, _data(new int[that._length])
	{
		std::copy(that._data, that._data + _length, _data);
	}

	// 拷贝赋值运算符（实现拷贝语义：释放this ＋ 拷贝that）
	MemoryBlock& operator=(const MemoryBlock& that)
	{
		if (this != &that)
		{
			// 释放自身的资源
			delete[] _data;

			// 拷贝that对象所拥有的资源
			_length = that._length;
			_data = new int[_length];
			std::copy(that._data, that._data + _length, _data);
		}
		return *this;
	}

	// 移动构造器（实现移动语义：移动that）
	MemoryBlock(MemoryBlock&& that)
		// 将自身的资源指针指向that对象所拥有的资源
		: _length(that._length)
		, _data(that._data)
	{
		// 将that对象原本指向该资源的指针设为空值
		that._data = nullptr;
		that._length = 0;
	}

	// 移动赋值运算符（实现移动语义：释放this ＋ 移动that）
	MemoryBlock& operator=(MemoryBlock&& that)
	{
		if (this != &that)
		{
			// 释放自身的资源
			delete[] _data;

			// 将自身的资源指针指向that对象所拥有的资源
			_data = that._data;
			_length = that._length;

			// 将that对象原本指向该资源的指针设为空值
			that._data = nullptr;
			that._length = 0;
		}
		return *this;
	}
private:
	size_t _length; // 资源的长度
	int* _data; // 指向资源的指针，代表资源本身
};

MemoryBlock f() { return MemoryBlock(50); }

int main()
{
	MemoryBlock a = f();            // 调用移动构造器，移动语义
	MemoryBlock b = a;              // 调用拷贝构造器，拷贝语义
	MemoryBlock c = std::move(a);   // 调用移动构造器，移动语义
	a = f();                        // 调用移动赋值运算符，移动语义
	b = a;                          // 调用拷贝赋值运算符，拷贝语义
	c = std::move(a);               // 调用移动赋值运算符，移动语义

	/*int numbers[] = { 1,2,3,4,5 };
	std::cout << "numbers:" << std::endl;
	for (auto number : numbers)
	{
	std::cout << number << std::endl;
	}*/

	//std::array<double, 4> arrayDemo = { 1,2,3,4 };
	//std::cout << "arrayDemo:" << std::endl;
	//for (auto itor : arrayDemo)
	//{
	//	std::cout << itor << std::endl;
	//}
	//int arrayDemoSize = sizeof(arrayDemo);
	//std::cout << "arrayDemo size:" << arrayDemoSize << std::endl;


	//std::forward_list<int> numbers = { 1,2,3,4,5,4,4 };
	//std::cout << "numbers:" << std::endl;
	//for (auto number : numbers)
	//{
	//	std::cout << number << std::endl;
	//}
	//numbers.remove(4);
	//std::cout << "numbers after remove:" << std::endl;
	//for (auto number : numbers)
	//{
	//	std::cout << number << std::endl;
	//}

	//std::unordered_map<std::string, std::string> mymap =
	//{
	//	{ "house","maison" },
	//	{ "apple","pomme" },
	//	{ "tree","arbre" },
	//	{ "book","livre" },
	//	{ "door","porte" },
	//	{ "grapefruit","pamplemousse" }
	//};
	//unsigned n = mymap.bucket_count();
	//std::cout << "mymap has " << n << " buckets.\n";
	//for (unsigned i = 0; i<n; ++i)
	//{
	//	std::cout << "bucket #" << i << " contains: ";
	//	for (auto it = mymap.begin(i); it != mymap.end(i); ++it)
	//		std::cout << "[" << it->first.data() << ":" << it->second.data() << "] ";
	//	std::cout << "\n";
	//}

	//for (auto it = mymap.begin(); it != mymap.end(); ++it)
	//{
	//	std::cout << "[" << it->first.data() << ":" << it->second.data() << "] ";
	//	std::cout << endl;
	//}

	/*std::unordered_set<int> unorder_set;
	unorder_set.insert(7);
	unorder_set.insert(5);
	unorder_set.insert(3);
	unorder_set.insert(4);
	unorder_set.insert(6);
	std::cout << "unorder_set:" << std::endl;
	for (auto itor : unorder_set)
	{
	std::cout << itor << std::endl;
	}

	std::set<int> set;
	set.insert(7);
	set.insert(5);
	set.insert(3);
	set.insert(4);
	set.insert(6);
	std::cout << "set:" << std::endl;
	for (auto itor : set)
	{
	std::cout << itor << std::endl;
	}*/

	//std::thread t1(threadfun1);
	//std::thread t2(threadfun2, 10, "abc");
	//t1.join();
	////t2.join();
	//std::cout << "join" << std::endl;
	//t2.detach();
	//std::cout << "detach" << std::endl;

	/*std::atomic_bool b;
	std::list<std::thread> lstThread;
	for (int i = 0; i < 10; ++i)
	{
	lstThread.push_back(std::thread(threadfun1));
	}
	for (auto& th : lstThread)
	{
	th.join();
	}
	*/


	//   std::thread threads[10];
	//   // spawn 10 threads:
	//   for (int i = 0; i<10; ++i)
	//        threads[i] = std::thread(print_id, i);

	//   std::cout << "10 threads ready to race...waiting fro 5 seconds\n";
	//std::this_thread::sleep_for( std::chrono::seconds(5));
	//   go();                       // go!

	//   for (auto& th : threads) th.join();

	/*std::shared_ptr<Test> p1 = std::make_shared<Test>();
	std::cout << "1 ref:" << p1.use_count() << std::endl;
	{
	std::shared_ptr<Test> p2 = p1;
	std::cout << "2 ref:" << p1.use_count() << std::endl;
	}
	std::cout << "3 ref:" << p1.use_count() << std::endl;*/

	//std::shared_ptr<TestA> ptr_a = std::make_shared<TestA>();
	//std::shared_ptr<TestB> ptr_b = std::make_shared<TestB>();
	//ptr_a->ReferTestB(ptr_b);
	//ptr_b->ReferTestB(ptr_a);
	//std::cout << "1 ref a:" << ptr_a.use_count() << std::endl;
	//std::cout << "1 ref b:" << ptr_b.use_count() << std::endl;

	/*auto add = [](int a, int b)->int {
	return a + b;
	}(1, 2 );
	int ret = add;
	std::cout << "ret:" << ret << std::endl;*/


	//db db1 = 1.0f;
	//std::cout << db1 << endl;


	//auto a = 1;
	//decltype(a) b = 2;
	//decltype(b) c = add(a, b);
	//std::cout << c << endl;


	//std::cout << to_string(1).data() << " " << std::stoi(string("123")) << endl;

	//std::random_device rd;
	//int randint = rd();

	//std::chrono::duration<double> duration(1.0);
	////时间间隔
	//std::this_thread::sleep_for(duration);
	////sleep

	//std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();

	//std::cout <<  << endl;
	//获取当前时间

	/*std::vector<int> v{ 1, 2, 3, 4, 5 };
	v.push_back(1);
	std::cout << "before shrink_to_fit: " << v.capacity() << std::endl;
	v.shrink_to_fit();
	std::cout << "after shrink_to_fit: " << v.capacity() << std::endl;
	*/
	//LOG(INFO) << "before move to object ot, t's size is: ";

	//struct A {
	//	int a;
	//	A(int _a) :a(_a + 100) {}
	//};
	//struct B : public A {
	//	int b;
	//	B(int _b) :A(_b), b(_b + 10000) {}
	//};
	//B obj(1);
	//std::cout << obj.a << ", " << obj.b << std::endl;



	class father {
	public:
		int a_;
		int GetA() { return a_; }
		virtual void SetA(int a) /*final*/ {
			a_ = a;
			//LOG(INFO) << "father modify a to " << a_;
		}
		//add keyword final to avoid non-anticipated inherit in compling but not errored in running  
		//virtual void SetA(int a) final {a_ = a;}  
	public:
		father(int a) :a_(a) {}
	};

	class Son : public father {
		int b_;
	public:
		Son(int a, int b) :father(a), b_(b) {}
		//add keyword override to avoid the error in compling but not errored in running.(eg. 'int SetA(double a){...} override' woule be errored by compiler)  
		virtual void SetA(int a) override {
			a_ = a;
			//LOG(INFO) << "son modify a to " << a_;
		}
		//virtual void SetA(double a) override {a_ = a;}  
	};

	std::wstring wide_str = L"中国人";
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>
		converter(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
	std::string narrow_str = converter.to_bytes(wide_str);
	std::wstring wstr = converter.from_bytes(narrow_str);
	std::cout << narrow_str << std::endl;
	wcout.imbue(std::locale("chs"));
	std::wcout << wstr << std::endl;
	std::cout << wstr.size() << " " << wstr.length() << endl;
	std::cout << narrow_str.size() << " " << narrow_str.length() << endl;


	string s = gb2312_to_utf8("哈哈哈");
	string s2 = utf8_to_gb2312(s);
	std::cout << s.c_str() << " " << s2.data() << endl;

	myprint(1.0, 10.1, "cc");
	cout << sum(1, 2, 3, 4) << endl; //10
	myexpand(1, 2, 3, 4);
	myexpand('a', "aaab", 1, 1.2); //myexpand 里面的int数组和它的参数无关
	myexpand2([](auto i) {cout << i << endl; }, 1, 2 + 2, "text", 9.213, 'z');

	MyPrint("%c %f %f %s\n", 'c', 1.0, 10.1, "cc");
	func0(1.0, 3.1);

	int l1 = Sum<int, bool, double>::value;
	cout << l1 << endl;

	int l2 = Sum2<int, double, short>::value;//值为14
	cout << l2 << endl;

	using T = MakeIndexes<3>::type;
	cout << typeid(T).name() << endl;

	AA aa;
	auto d = CreateDelegate(&aa, &AA::Fun); //创建委托
	cout << typeid(d).name() << endl;
	d(1); //调用委托，将输出1
	auto d1 = CreateDelegate(&aa, &AA::Add); //创建委托
	cout << typeid(d1).name() << endl;
	d1(1, 2.5); //调用委托，将输出3.5

	test_forward1();
	test_forward2();
	test_forward3();
	test_forward4();

	rvalue();

	return 0;
}
