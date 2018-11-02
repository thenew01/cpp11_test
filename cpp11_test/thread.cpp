#include <iostream>
#include <future>
#include <thread>
#include <string>

int main()
{
	// 将一个返回值为7的 lambda 表达式封装到 task 中
	// std::packaged_task 的模板参数为要封装函数的类型
	std::packaged_task<int()> task([]() {return 7; });
	// 获得 task 的 future
	std::future<int> result = task.get_future();    // 在一个线程中执行 task
	std::thread(std::move(task)).detach();    
	std::cout << "Waiting...";
	result.wait();
	// 输出执行结果
	std::cout << "Done!" << std::endl << "Result is " << result.get() << '\n';
}

/*
// 字符串字面量自定义必须设置如下的参数列表
std::string operator"" _wow1(const char *wow1, size_t len) {
	return std::string(wow1) + "woooooooooow, amazing";
}

std::string operator"" _wow2(unsigned long long i) {
	return std::to_string(i) + "woooooooooow, amazing";
}

int main() {
	auto str = "abc"_wow1;
	auto num = 1_wow2;
	std::cout << str << std::endl;
	std::cout << num << std::endl;
	return 0;
}*/

