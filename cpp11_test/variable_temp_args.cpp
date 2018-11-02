

#include <iostream>
template<typename T>
void printf(T value) {
	std::cout << value << std::endl;
}

template<typename T, typename... Args>
void printf(T value, Args... args) {
	std::cout << value << std::endl;
	printf(args...);
}


// 编译这个代码需要开启 -std=c++14
template<typename T, typename... Args>
auto print2(T value, Args... args) {
	std::cout << value << std::endl;
	return std::initializer_list<T>{([&] {
		std::cout << args << std::endl;
	}(), value)...};
}


int main() {
	printf(1, 2, "123", 1.1);
	print2(1, 2.1, "123");
	return 0;
}

