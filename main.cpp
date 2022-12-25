#include <functional>
#include <iostream>
using namespace std;

struct A {};
struct B {};
struct C {};

namespace Impl {
template <typename T1, typename T2>
void f(T2 b,
       typename std::enable_if<std::is_same<T1, A>::value>::type* = nullptr) {
  std::cout << "Was here in A" << std::endl;
}

template <typename T1, typename T2>
void f(T2 b,
       typename std::enable_if<std::is_same<T1, C>::value>::type* = nullptr) {
  std::cout << "Was here in C" << std::endl;
}
}  // namespace Impl

template <typename T1, typename T2>
void f(T2 b) {
  Impl::f<T1>(b);
}

template <typename T>
void invoke_f(std::function<void(T)> f, B& t) {
  f(t);
}

int main() {
  B t;
  f<A>(t);
  f<C>(t);

  // std::function<void(int)> f_display = print_num;
  std::function<void(B)> f_ptr_A = f<A, B>;
  f_ptr_A(t);

  std::function<void(B)> f_ptr_C = f<C, B>;
  f_ptr_C(t);

  invoke_f(f_ptr_A, t);
  invoke_f(f_ptr_C, t);
  return 0;
}