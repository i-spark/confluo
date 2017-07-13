#ifndef LIBDIALOG_DIALOG_ARITHMETIC_OPS_H_
#define LIBDIALOG_DIALOG_ARITHMETIC_OPS_H_

#include <cstdint>

#include "exceptions.h"

namespace dialog {

/**
 * Unary arithmetic/bitwise operators
 */
enum unaryop_id
  : uint8_t {
    ASSIGN = 0,  //!< ASSIGN (=)
  NEGATIVE = 1,  //!< NEGATIVE (-)
  POSITIVE = 2,  //!< POSITIVE (+)
  BW_NOT = 3   //!< BW_NOT (~)
};

/**
 * Binary arithmetic/bitwise operators
 */
enum binaryop_id
  : uint8_t {
    ADD = 0,      //!< ADD (+)
  SUBTRACT = 1,  //!< SUBTRACT (-)
  MULTIPLY = 2,  //!< MULTIPLY (*)
  DIVIDE = 3,   //!< DIVIDE (/)
  MODULO = 4,   //!< MODULO (%)
  BW_AND = 5,   //!< BW_AND (&)
  BW_OR = 6,    //!< BW_OR (|)
  BW_XOR = 7,   //!< BW_XOR (^)
  BW_LSHIFT = 8,   //!< BW_LSHIFT (<<)
  BW_RSHIFT = 9  //!< BW_RSHIFT (>>)
};

typedef void (*unary_fn)(void* res, const void* v);

typedef void (*binary_fn)(void* res, const void* v1, const void* v2);

typedef std::vector<unary_fn> unary_ops_t;
typedef std::vector<binary_fn> binary_ops_t;

// Unary arithmetic operators
template<typename T>
inline void assign(void* res, const void* v) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v));
}

template<>
inline void assign<std::string>(void* res, const void* v) {
  THROW(unsupported_exception, "= not supported for string type");
}

template<>
inline void assign<void>(void* res, const void* v) {
  THROW(unsupported_exception, "= not supported for none type");
}

template<typename T>
inline void negative(void* res, const void* v) {
  *(reinterpret_cast<T*>(res)) = -*(reinterpret_cast<const T*>(v));
}

template<>
inline void negative<std::string>(void* res, const void* v) {
  THROW(unsupported_exception, "- not supported for string type");
}

template<>
inline void negative<void>(void* res, const void* v) {
  THROW(unsupported_exception, "- not supported for none type");
}

template<typename T>
inline void positive(void* res, const void* v) {
  *(reinterpret_cast<T*>(res)) = +*(reinterpret_cast<const T*>(v));
}

template<>
inline void positive<std::string>(void* res, const void* v) {
  THROW(unsupported_exception, "+ not supported for string type");
}

template<>
inline void positive<void>(void* res, const void* v) {
  THROW(unsupported_exception, "+ not supported for none type");
}

// Binary arithmetic operators
template<typename T>
inline void add(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      + *(reinterpret_cast<const T*>(v2));
}

template<>
inline void add<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "+ not supported for string type");
}

template<>
inline void add<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "+ not supported for none type");
}

template<typename T>
inline void subtract(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      - *(reinterpret_cast<const T*>(v2));
}

template<>
inline void subtract<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "- not supported for string type");
}

template<>
inline void subtract<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "- not supported for none type");
}

template<typename T>
inline void multiply(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      * *(reinterpret_cast<const T*>(v2));
}

template<>
inline void multiply<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "* not supported for string type");
}

template<>
inline void multiply<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "* not supported for none type");
}

template<typename T>
inline void divide(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      / *(reinterpret_cast<const T*>(v2));
}

template<>
inline void divide<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "/ not supported for string type");
}

template<>
inline void divide<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "/ not supported for none type");
}

template<typename T>
inline void modulo(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      % *(reinterpret_cast<const T*>(v2));
}

template<>
inline void modulo<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "% not supported for string type");
}

template<>
inline void modulo<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "% not supported for none type");
}

template<>
inline void modulo<float>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "% not supported for float type");
}

template<>
inline void modulo<double>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "% not supported for double type");
}

// Bitwise operators
template<typename T>
inline void bw_not(void* res, const void* v) {
  *(reinterpret_cast<T*>(res)) = ~*(reinterpret_cast<const T*>(v));
}

template<>
inline void bw_not<std::string>(void* res, const void* v) {
  THROW(unsupported_exception, "~ not supported for string type");
}

template<>
inline void bw_not<void>(void* res, const void* v) {
  THROW(unsupported_exception, "~ not supported for none type");
}

template<>
inline void bw_not<float>(void* res, const void* v) {
  THROW(unsupported_exception, "~ not supported for float type");
}

template<>
inline void bw_not<double>(void* res, const void* v) {
  THROW(unsupported_exception, "~ not supported for double type");
}

template<typename T>
inline void bw_and(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      & *(reinterpret_cast<const T*>(v2));
}

template<>
inline void bw_and<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "& not supported for string type");
}

template<>
inline void bw_and<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "& not supported for none type");
}

template<>
inline void bw_and<float>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "& not supported for float type");
}

template<>
inline void bw_and<double>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "& not supported for double type");
}

template<typename T>
inline void bw_or(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      | *(reinterpret_cast<const T*>(v2));
}

template<>
inline void bw_or<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "| not supported for string type");
}

template<>
inline void bw_or<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "| not supported for none type");
}

template<>
inline void bw_or<float>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "| not supported for float type");
}

template<>
inline void bw_or<double>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "| not supported for double type");
}

template<typename T>
inline void bw_xor(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      ^ *(reinterpret_cast<const T*>(v2));
}

template<>
inline void bw_xor<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "^ not supported for string type");
}

template<>
inline void bw_xor<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "^ not supported for none type");
}

template<>
inline void bw_xor<float>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "^ not supported for float type");
}

template<>
inline void bw_xor<double>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "^ not supported for double type");
}

template<typename T>
inline void bw_lshift(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      << *(reinterpret_cast<const T*>(v2));
}

template<>
inline void bw_lshift<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "<< not supported for string type");
}

template<>
inline void bw_lshift<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "<< not supported for none type");
}

template<>
inline void bw_lshift<float>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "<< not supported for double type");
}

template<>
inline void bw_lshift<double>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, "<< not supported for float type");
}

template<typename T>
inline void bw_rshift(void* res, const void* v1, const void* v2) {
  *(reinterpret_cast<T*>(res)) = *(reinterpret_cast<const T*>(v1))
      >> *(reinterpret_cast<const T*>(v2));
}

template<>
inline void bw_rshift<std::string>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, ">> not supported for string type");
}

template<>
inline void bw_rshift<void>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, ">> not supported for none type");
}

template<>
inline void bw_rshift<float>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, ">> not supported for float type");
}

template<>
inline void bw_rshift<double>(void* res, const void* v1, const void* v2) {
  THROW(unsupported_exception, ">> not supported for double type");
}

template<typename T>
static unary_ops_t init_unaryops() {
  return {assign<T>, negative<T>, positive<T>, bw_not<T>};
}

template<typename T>
static binary_ops_t init_binaryops() {
  return {add<T>, subtract<T>, multiply<T>, divide<T>,
    modulo<T>, bw_and<T>, bw_or<T>, bw_xor<T>, bw_lshift<T>, bw_rshift<T>};
}

}

#endif /* LIBDIALOG_DIALOG_ARITHMETIC_OPS_H_ */