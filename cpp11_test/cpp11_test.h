#pragma once


#include <queue>
#include <deque>
#include <vector>
#include <iostream>
#include <map>
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <memory>

#include <utility>

#include <random>
#include <string>
#include <codecvt>

using namespace std;

void threadfun1()
{
	std::cout << "threadfun1 - 1\r\n" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "threadfun1 - 2" << std::endl;
}

void threadfun2(int iParam, std::string sParam)
{
	std::cout << "threadfun2 - 1" << std::endl;
	std::cout << iParam << " " << sParam.data() << endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "threadfun2 - 2" << std::endl;
}

std::atomic_bool bIsReady = false;
std::atomic_int iCount = 100;
void threadfun3()
{
	if (!bIsReady) {
		std::this_thread::yield();//让步，把当期未用完的时间片让出来，与其他线程一同重新等待时间片
	}
	while (iCount > 0)
	{
		printf("iCount:%d\r\n", iCount--);
	}
}

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

 void print_id(int id) {
	     std::unique_lock<std::mutex> lck(mtx);
	     while (!ready) cv.wait(lck);
	     // ...
		 std::cout << "thread " << id << '\n';
	
}

 void go() {
	     std::unique_lock<std::mutex> lck(mtx);
	     ready = true;
	     cv.notify_all();
	
}


 class Test
 {
 public:
	 Test()
	 {
		 std::cout << "Test()" << std::endl;
	 }
	 ~Test()
	 {
		 std::cout << "~Test()" << std::endl;
	 }
 };
//
class TestB;
class TestA
{
public:
	TestA()
	{
		std::cout << "TestA()" << std::endl;
	}
	void ReferTestB(std::shared_ptr<TestB> test_ptr)
	{
		m_TestB_Ptr = test_ptr;
	}
	void TestWork()
	{
		std::cout << "~TestA::TestWork()" << std::endl;
	}
	~TestA()
	{
		std::cout << "~TestA()" << std::endl;
	}
private:
	std::weak_ptr<TestB> m_TestB_Ptr;
};

class TestB
{
public:
	TestB()
	{
		std::cout << "TestB()" << std::endl;
	}

	void ReferTestB(std::shared_ptr<TestA> test_ptr)
	{
		m_TestA_Ptr = test_ptr;
	}
	void TestWork()
	{
		std::cout << "~TestB::TestWork()" << std::endl;
	}
	~TestB()
	{
		std::shared_ptr<TestA> tmp = m_TestA_Ptr.lock();
		tmp->TestWork();
		std::cout << "2 ref a:" << tmp.use_count() << std::endl;
		std::cout << "~TestB()" << std::endl;
	}
	std::weak_ptr<TestA> m_TestA_Ptr;
};

constexpr int calc_in_compile_0() {

	return 100;

}

constexpr int calc_in_compile_1(int a) {

	return a * 100;

}

constexpr int calc_in_compile_2(int b, int c) {

	return c * calc_in_compile_1(b);

}

#define EXPECT_EQ(v, exp) static_assert((v)==(exp),#exp)
EXPECT_EQ(100, calc_in_compile_0());

constexpr int a = 1;

EXPECT_EQ(100, calc_in_compile_1(a));

EXPECT_EQ(10000, calc_in_compile_2(a, calc_in_compile_1(a)));


typedef double db;
//c99

using db = double;
//c++11

typedef void(*function)(int, int);//c99，函数指针类型定义

using function = void(*)(int, int);//c++11，函数指针类型定义

using kvpairs = std::map<std::string, std::string>;
//c++11

using CompareOperator = std::function<int(kvpairs &, kvpairs &)>;
//c++11

using query_record = std::tuple<time_t, std::string>;
//c++11

template<class T> using twins = std::pair<T, T>;
//更广泛的还可以用于模板

int add(int a, int b) {
	return a + b;
}

std::string gb2312_to_utf8(std::string const &strGb2312)
{
	std::vector<wchar_t> buff(strGb2312.size());
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	wchar_t* pwszNext = nullptr;
	const char* pszNext = nullptr;
	mbstate_t state = {};
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).in(state,
			strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
			buff.data(), buff.data() + buff.size(), pwszNext);

	if (std::codecvt_base::ok == res)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
		return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
	}

	return "";

}

std::string utf8_to_gb2312(std::string const &strUtf8)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
	std::wstring wTemp = cutf8.from_bytes(strUtf8);
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	const wchar_t* pwszNext = nullptr;
	char* pszNext = nullptr;
	mbstate_t state = {};

	std::vector<char> buff(wTemp.size() * 2);
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).out(state,
			wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
			buff.data(), buff.data() + buff.size(), pszNext);

	if (std::codecvt_base::ok == res)
	{
		return std::string(buff.data(), pszNext);
	}
	return "";
}



//------------------------------可变模板参数
//递归终止函数
template<typename T>
void myprint(T end)//递归到最后一次，调用单参数函数
{
	cout << "parameter " << end << endl;
}

//展开函数
template<typename T, class ...Args>
void myprint(T head, Args... rest)
{
	cout << "parameter " << head << endl;
	myprint(rest...);
}

template<typename T>
T sum(T t)
{
	return t;
}

template<typename T, typename ...Types>
T sum(T first, Types... rest)
{
	return first + sum<T>(rest...);
}

template<typename T>
void printarg(T t)
{
	cout << t << endl;
}

template<typename ...Args>
void myexpand(Args... args)
{
	int arr[] = { (printarg(args), 0)... };		//逗号表达式
}

template<typename F, typename ...Args>
void myexpand2(const F &f, Args &&...args)
{
	//使用了完美转发
	initializer_list<int>{ ( f( std::forward<Args>(args) ), 0 ) ...};	 //逗号表达式
}

template <typename... TS>   // typename... TS为模板形参包，TS为模式
static void MyPrint(const char* s, TS... args)  // TS... args为函数形参包，args为模式
{
	printf(s, args...);
}

template <typename... Types>
static void func0(Types... args)
{
	printf("The sample values are: %f, %f\n", args...);
}

template<typename First, typename... Rest>
struct Sum
{
	enum { value = Sum<First>::value + Sum<Rest...>::value };
};

template<typename Last>
struct Sum<Last>
{
	enum { value = sizeof(Last) };
};

//-----------------------------
//前向声明
template<typename First, typename... Args>
struct Sum2;

//基本定义
template<typename First, typename... Rest>
struct Sum2<First, Rest...> : std::integral_constant<int, Sum2<First>::value + Sum<Rest...>::value>
{
};

//递归终止
template<typename Last>
struct Sum2<Last> : std::integral_constant<int, sizeof(Last)>
{
};

//整型序列的定义
template<int...>
struct IndexSeq {};

//继承方式，开始展开参数包
template<int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...> {};

// 模板特化，终止展开参数包的条件
template<int... Indexes>
struct MakeIndexes<0, Indexes...>
{
	typedef IndexSeq<Indexes...> type;
};

template<int N, int... Indexes>
struct MakeIndexes3
{
	using type = typename MakeIndexes3<N - 1, N - 1, Indexes...>::type;
};

template<int... Indexes>
struct MakeIndexes3<0, Indexes...>
{
	typedef IndexSeq<Indexes...> type;
};

//==============================
struct AAA
{
	AAA(int) {}
};

struct BBB
{
	BBB(int, double) {}
};
template<typename T, typename... Args>
T* Instance(Args&&... args)
{
	return new T(std::forward<Args>(args)...);
}
AAA* pa = Instance<AAA>(1);
BBB* pb = Instance<BBB>(1, 2);
//===================================

template <class T, class R, typename... Args>
class  MyDelegate
{
public:
	MyDelegate(T* t, R(T::*f)(Args...)) :m_t(t), m_f(f) {}

	R operator()(Args&&... args)
	{
		return (m_t->*m_f)(std::forward<Args>(args) ...);
	}

private:
	T * m_t;
	R(T::*m_f)(Args...);
};

template <class T, class R, typename... Args>
MyDelegate<T, R, Args...> CreateDelegate(T* t, R(T::*f)(Args...))
{
	return MyDelegate<T, R, Args...>(t, f);
}

struct AA
{
	void Fun(int&& i) { cout << i << endl; }
	int Add(int&& i, double j) { cout << i + j << endl; return i + j; }
};


//////////////////////////////////////////////  
// reference: http://en.cppreference.com/w/cpp/utility/forward  
struct A {
	A(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
	A(int& n) { std::cout << "lvalue overload, n=" << n << "\n"; }
};

class B {
public:
	template<class T1, class T2, class T3>
	B(T1&& t1, T2&& t2, T3&& t3) :
		a1_{ std::forward<T1>(t1) },
		a2_{ std::forward<T2>(t2) },
		a3_{ std::forward<T3>(t3) }
	{
	}

private:
	A a1_, a2_, a3_;
};

template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
	return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

template<class T, class... U>
std::unique_ptr<T> make_unique2(U&&... u)
{
	return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}

int test_forward1()
{
	auto p1 = make_unique1<A>(2); // rvalue  
	int i = 1;
	auto p2 = make_unique1<A>(i); // lvalue  

	std::cout << "B\n";
	auto t = make_unique2<B>(2, i, 3);

	return 0;
}

////////////////////////////////////////////////////////  
// reference: http://www.cplusplus.com/reference/utility/forward/  
// function with lvalue and rvalue reference overloads:  
void overloaded(const int& x) { std::cout << "[lvalue]"; }
void overloaded(int&& x) { std::cout << "[rvalue]"; }

// function template taking rvalue reference to deduced type:  
template <class T> void fn(T&& x) {
	overloaded(x);                   // always an lvalue  
	overloaded(std::forward<T>(x));  // rvalue if argument is rvalue  
}

int test_forward2()
{
	int a;

	std::cout << "calling fn with lvalue: ";
	fn(a);
	std::cout << '\n';

	std::cout << "calling fn with rvalue: ";
	fn(0);
	std::cout << '\n';

	return 0;
}

//////////////////////////////////////////////////////  
// reference: http://stackoverflow.com/questions/8526598/how-does-stdforward-work  
template<class T>
struct some_struct {
	T _v;
	template<class U>
	some_struct(U&& v) : _v(static_cast<U&&>(v)) {} // perfect forwarding here  
													// std::forward is just syntactic sugar for this  
};

int test_forward3()
{
	/* remember the reference collapsing rules(引用折叠规则):
	前者代表接受类型，后者代表进入类型，=>表示引用折叠之后的类型，即最后被推导决断的类型
	TR   R

	T&   &->T&   // lvalue reference to cv TR -> lvalue reference to T
	T&   &&->T&  // rvalue reference to cv TR -> TR (lvalue reference to T)
	T&&  &->T&   // lvalue reference to cv TR -> lvalue reference to T
	T&&  &&->T&& // rvalue reference to cv TR -> TR (rvalue reference to T) */

	some_struct<int> s1(5);
	// in ctor: '5' is rvalue (int&&), so 'U' is deduced as 'int', giving 'int&&'  
	// ctor after deduction: 'some_struct(int&& v)' ('U' == 'int')  
	// with rvalue reference 'v' bound to rvalue '5'  
	// now we 'static_cast' 'v' to 'U&&', giving 'static_cast<int&&>(v)'  
	// this just turns 'v' back into an rvalue  
	// (named rvalue references, 'v' in this case, are lvalues)  
	// huzzah, we forwarded an rvalue to the constructor of '_v'!  

	// attention, real magic happens here  
	int i = 5;
	some_struct<int> s2(i);
	// in ctor: 'i' is an lvalue ('int&'), so 'U' is deduced as 'int&', giving 'int& &&'  
	// applying the reference collapsing rules yields 'int&' (& + && -> &)  
	// ctor after deduction and collapsing: 'some_struct(int& v)' ('U' == 'int&')  
	// with lvalue reference 'v' bound to lvalue 'i'  
	// now we 'static_cast' 'v' to 'U&&', giving 'static_cast<int& &&>(v)'  
	// after collapsing rules: 'static_cast<int&>(v)'  
	// this is a no-op, 'v' is already 'int&'  
	// huzzah, we forwarded an lvalue to the constructor of '_v'!  

	return 0;
}

////////////////////////////////////////////////////  
// reference: https://oopscenities.net/2014/02/01/c11-perfect-forwarding/  
void sum(int a, int b)
{
	std::cout << a + b << std::endl;
}

void concat(const std::string& a, const std::string& b)
{
	std::cout << a + b << std::endl;
}

void successor(int a, int& b)
{
	b = ++a;
}

template <typename PROC, typename A, typename B>
void invoke(PROC p, A&& a, B&& b)
{
	p(std::forward<A>(a), std::forward<B>(b));
}

int test_forward4()
{
	invoke(sum, 10, 20);
	invoke(concat, "Hello", "world");
	int s = 0;
	invoke(successor, 10, s);
	std::cout << s << std::endl;

	return 0;
}