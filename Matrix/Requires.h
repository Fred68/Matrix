#ifndef REQUIRESH
#define REQUIRESH

#include <concepts>
#include <complex>
#include <cmath>
#include <cstddef>
#include <limits>
//#include <type_traits>

namespace matrix
{

/* Assegnazione semplice */
template<class T>
concept RQassign = requires (T x, T y) { x = y; };

/* Assegnazione e ordinamento*/
template<class T>
concept RQorder = RQassign<T> && std::totally_ordered<T>;

/* Somma (valida anche per stringhe) */
template<class T>
concept RQsum = RQassign<T> && requires (T x, T y) { x += y; x + y; };

/* Somma, differerenza, prodotto */
template<class T>
concept RQsumdifprod = RQsum<T> && requires (T x, T y) { x -= y; x - y; x *= y; x * y; };

/* Per tutti i numeri a virgola mobile o complessi */
template<class T>
concept RQfloat = RQsumdifprod<T> && (std::is_floating_point<T>::value || std::is_same_v<T, std::complex<typename T::value_type>>);

/* Per tutti i numeri a virgola mobile o complessi con funzione abs()*/
template<class T, class A>
concept RQfloatabs = RQfloat<T> && RQfloat<A> && requires (T x, A a) { a = abs(x);};

}

#endif

/*
constexpr T max_value{ std::numeric_limits<T>::max() };
*/
/*
To constrain a generic type T to be std::complex, you can use the std::is_same type trait 1. Here’s an example:
#include <complex>
#include <type_traits>
template <typename T>
concept Complex = std::is_same_v<T, std::complex<typename T::value_type>>;
template <Complex T>
void foo(T t) {
  // ...
}
In the above code, the Complex concept is defined as a type trait that checks if the given type T is the same as std::complex<typename T::value_type>. The foo function is then constrained to accept only types that satisfy the Complex concept.
*/
