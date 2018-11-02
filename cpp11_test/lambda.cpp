
/*
--------lamda语法--------
[capture list](params list) mutable exception-> return type{ function body }
C++11中，默认情况下lambda函数是一个const函数。因此，
在Lambda表达式中，如果以传值方式捕获外部变量，则函数体中不能修改该外部变量，否则会引发编译错误

// 完整语法
[ capture-list ] ( params ) mutable(optional) constexpr(optional)(c++17) exception attribute -> ret { body }

// 可选的简化语法
[ capture-list ] ( params ) -> ret { body }
[ capture-list ] ( params ) { body }
[ capture-list ] { body }

*/
/*
----混合方式----
捕获形式	说明
[]	不捕获任何外部变量
[变量名, …]	默认以值得形式捕获指定的多个外部变量（用逗号分隔），如果引用捕获，需要显示声明（使用&说明符）
[this]	以值的形式捕获this指针
[*this]：通过传值方式捕获当前对象
[=]	以值的形式捕获所有外部变量
[&]	以引用形式捕获所有外部变量
[=, &x]	变量x以引用形式捕获，其余变量以传值形式捕获
[&, x]	变量x以值的形式捕获，其余变量以引用形式捕获

-----限制-----
参数列表中不能有默认参数
不支持可变参数
所有参数必须有参数名
**/

//局部函数，在函数作用域中定义的函数，也称为内嵌函数。
//每当你定义一个lambda表达式后，编译器会自动生成一个匿名类（这个类当然重载了()运算符）
/*
1.函数对象带有状态
2.每个函数对象有自己的类型：对于普通函数来说，只要签名一致，其类型就是相同的。
但是这并不适用于函数对象，因为函数对象的类型是其类的类型。
这样，函数对象有自己的类型，这意味着函数对象可以用于模板参数，这对泛型编程有很大提升。
*/

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

bool cmp(int a, int b)
{
	return  a < b;
}

int main()
{

	vector<int> myvec{ 3, 2, 5, 7, 3, 2 };
	vector<int> lbvec(myvec);

	sort(myvec.begin(), myvec.end(), cmp); // 旧式做法
	cout << "predicate function:" << endl;
	for (int it : myvec)
		cout << it << ' ';
	cout << endl;

	sort(lbvec.begin(), lbvec.end(), [](int a, int b) -> bool { return a < b; });   // Lambda表达式
	cout << "lambda expression:" << endl;
	for (int it : lbvec)
		cout << it << ' ';
	cout << endl;

	//////////////////////////////////////////////////////////////////////////
	int a0 = 123;
	auto f = [a0] { cout << "a0:" << a0 << endl; };
	f(); // 输出：123

		 //或通过“函数体”后面的‘()’传入参数
	[](int a) {cout << a << endl; }(123);
	[](int a) {cout << a << endl; }(a0);
	//////////////////////////////////////////////////////////////////////////

	//传值捕获
	int a1 = 123;
	auto ff = [a1] { cout << "a1: " << a1 << endl; };
	a1 = 321;
	ff(); // 输出：123

		  // 引用捕获
	int a = 123;
	auto f0 = [&a] { cout << "a: " << a << endl; };
	a = 321;
	f0(); //123

	a = 123;
	// 隐式捕获 [=],[&]
	auto f1 = [=]() { cout << "隐式捕获[=] " << a << endl;  };
	a = 321;
	f1();  //123

	a = 123;
	auto f2 = [&]() { cout << "隐式捕获[&] " << a << endl;  };
	a = 321;
	f2();	// 输出：321

	a = 123;
	auto f3 = [=]() mutable { a = 1;  cout << "隐式捕获[=]mutable " << a << endl; };
	a = 321;
	f3();

	a = 123;
	auto f4 = [&]() mutable { a = 2;  cout << "隐式捕获[&]mutable " << a << endl; };
	a = 321;
	f4();

	// 高级用法 
	{
		//闭包
		int m = [](int x) { return [](int y) { return y * 2; }(x)+6; }(5);
		std::cout << "m:" << m << std::endl;	//输出m:16
		std::cout << "n:" << [](int x, int y) { return x + y; }(5, 4) << std::endl;            //输出n:9

		auto gFunc = [](int x) -> function<int(int)> { return [=](int y) { return x + y; }; };
		auto lFunc = gFunc(4);
		std::cout << lFunc(5) << std::endl;

		auto hFunc = [](const function<int(int)>& f, int z) { return f(z) + 1; };
		auto aa = hFunc(gFunc(7), 8);
		std::cout << aa << endl;

		//混合捕获方式
		int a = 111, b = 222;
		auto func = [=, &b]()mutable { a = 22; b = 333; std::cout << "a:" << a << " b:" << b << std::endl; };
		func();
		std::cout << "a:" << a << " b:" << b << std::endl;

		a = 333;
		auto func2 = [=, &a] { a = 444; std::cout << "a:" << a << " b:" << b << std::endl; };
		func2();

		std::function<void(int x)> f_display_42 = [](int x) {
			//print_num(x); 
			std::cout << x << endl;
		};
		f_display_42(44);
	}

	//泛型lambda表达式	
	auto add = [](auto x, auto y) { return x + y; };
	int x0 = add(2, 3);   // 5
	double y0 = add(2.5, 3.5);  // 6.0

								//捕捉表达式
								// 利用表达式捕获，可以更灵活地处理作用域内的变量
	int x = 4;
	auto y = [&r = x, x = x + 1]{ r += 2; return x * x; }();
	// 此时 x 更新为6，y 为25

	// 直接用字面值初始化变量
	auto z = [str = "string"]{ return str; }();
	// 此时z是const char* 类型，存储字符串 string

	//绑定器（binder）
	//绑定器std::bind是最常用的函数适配器，它可以将函数对象的参数绑定至特定的值。
	// 对于没有绑定的参数可以使用std::placeholers::_1, std::placeholers::_2等标记。
	auto minus10 = std::bind(std::minus<int>{}, std::placeholders::_1, 10);
	cout << minus10(20) << endl;  // 输出10

								  //重新排列参数的顺序
	auto vminus = std::bind(std::minus<int>{}, std::placeholders::_2, std::placeholders::_1);
	cout << vminus(20, 10) << endl;  // 输出-10

									 //绑定器还可以互相嵌套，从而实现函数对象的组合
									 // 定义一个接收一个参数，然后将参数加10再乘以2的函数对象
	auto plus10times2 = std::bind(std::multiplies<int>{},
		std::bind(std::plus<int>{}, std::placeholders::_1, 10), 2);
	cout << plus10times2(4) << endl; // 输出： 28 

									 // 定义3次方函数对象
	auto pow3 = std::bind(std::multiplies<int>{},
		std::bind(std::multiplies<int>{}, std::placeholders::_1, std::placeholders::_1),
		std::placeholders::_1);
	cout << pow3(3) << endl;  // 输出：27
}

