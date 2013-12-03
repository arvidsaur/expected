//! \file test_expected.cpp

// Copyright Pierre Talbot 2013.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Boost test of the expected library.

// See http://www.boost.org/doc/libs/1_46_1/libs/test/doc/html/utf/testing-tools/reference.html

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "Expected Test Suite"
#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-gd-1_47.lib (trunk at 12 Jun 11)

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink
#include <boost/test/included/unit_test.hpp>

#ifdef EXPECTED_CPP11_TESTS
  #define BOOST_RESULT_OF_USE_DECLTYPE
  #define BOOST_EXPECTED_USE_STD_EXCEPTION_PTR
#endif

#include "../../../boost/expected/expected.hpp"

#ifdef EXPECTED_CPP11_TESTS
  #define MAKE_EXCEPTION_PTR(exception) std::make_exception_ptr(exception)
  typedef std::exception_ptr exception_ptr_type;
#else
  #define MAKE_EXCEPTION_PTR(exception) boost::copy_exception(exception)
  typedef boost::exception_ptr exception_ptr_type;
#endif

#include <boost/bind.hpp>

#include <stdexcept>
#include <exception>

#ifdef EXPECTED_CPP11_TESTS
  #include <system_error>
  #define ERROR_CONDITION_NS std
#else
  #include <boost/system/error_code.hpp>
  #include <boost/system/system_error.hpp>
  #define ERROR_CONDITION_NS boost::system
#endif

using namespace boost;

class test_exception : public std::exception
{
};

int throwing_fun(){ throw test_exception();  }
int nothrowing_fun(){ return 4; }

void void_throwing_fun(){ throw test_exception();  }
void do_nothing_fun(){}

BOOST_AUTO_TEST_SUITE(except_expected_constructors)

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_default_constructor)
{
  expected<int> e;
  BOOST_CHECK_EQUAL(e.valid(), true);
  BOOST_CHECK_EQUAL(e.get(), int());

  class DefaultConstructibleTest
  {
    int s;
  public:
    DefaultConstructibleTest() = default;
    DefaultConstructibleTest(int i) : s(i){}

    int get() const { return s; }
  };

  expected<DefaultConstructibleTest> e2;
  BOOST_CHECK_EQUAL(e2.valid(), true);
  BOOST_CHECK_EQUAL(e2.get().get(), 0);
}
#endif

BOOST_AUTO_TEST_CASE(expected_from_value)
{
  // From value constructor.
  expected<int> e(5);
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), 5);
  BOOST_CHECK_EQUAL(*e, 5);
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_exception)
{
  // From exceptional constructor.
  expected<int> e(exceptional, test_exception());
  BOOST_REQUIRE_THROW(e.get(), test_exception);
  BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_copy_value)
{
  // From copy constructor.
  expected<int> ef(5);
  expected<int> e(ef);
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), 5);
  BOOST_CHECK_EQUAL(*e, 5);
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_copy_exception)
{
  // From exceptional constructor.
  expected<int> ef(exceptional, test_exception());
  expected<int> e(ef);
  BOOST_REQUIRE_THROW(e.get(), test_exception);
  BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif
}

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_from_emplace)
{
  // From emplace constructor.
  expected<std::string> e(emplace, "emplace");
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), "emplace");
  BOOST_CHECK_EQUAL(*e, "emplace");
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}
#endif

BOOST_AUTO_TEST_CASE(expected_from_exception_ptr)
{
  // From exception_ptr constructor.
  expected<int> e(exceptional, MAKE_EXCEPTION_PTR(test_exception()));
  BOOST_REQUIRE_THROW(e.get(), test_exception);
  BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_moved_value)
{
  // From move value constructor.
  std::string value = "my value";
  expected<std::string> e = std::move(value);
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), "my value");
  BOOST_CHECK_EQUAL(*e, "my value");
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_catch_block)
{
  // From catch block
  try
  {
    throw test_exception();
  }
  catch(...)
  {
    expected<int> e(exceptional);

    BOOST_REQUIRE_THROW(e.get(), std::exception);
    BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
    BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(error_expected_constructors)

BOOST_AUTO_TEST_CASE(expected_from_value)
{
  // From value constructor.
  expected<int, ERROR_CONDITION_NS::error_condition> e(5);
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), 5);
  BOOST_CHECK_EQUAL(*e, 5);
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_from_error)
{
  // From exceptional constructor.
  expected<int, ERROR_CONDITION_NS::error_condition> e(exceptional, ERROR_CONDITION_NS::make_error_condition(ERROR_CONDITION_NS::errc::invalid_argument));
  auto error_from_except_check = [](const bad_expected_access<ERROR_CONDITION_NS::error_condition>& except)
  { 
    return ERROR_CONDITION_NS::errc(except.error().value()) == ERROR_CONDITION_NS::errc::invalid_argument;
  };
  BOOST_REQUIRE_EXCEPTION(e.get(), bad_expected_access<ERROR_CONDITION_NS::error_condition>, error_from_except_check);
  BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif
}
#endif

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(except_expected_assignment)

BOOST_AUTO_TEST_CASE(expected_from_value)
{
  expected<int> e(5);
  BOOST_CHECK_EQUAL(e.get(), 5);

  // From value assignment.
  e = 8;
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), 8);
  BOOST_CHECK_EQUAL(*e, 8);
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_copy_expected)
{
  expected<int> e(5);
  expected<int> e2(8);

  // From value assignment.
  e = e2;
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), 8);
  BOOST_CHECK_EQUAL(*e, 8);
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_from_moved_expected)
{
  expected<std::string> e("e");
  expected<std::string> e2("e2");

  // From value assignment.
  e = std::move(e2);
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), "e2");
  BOOST_CHECK_EQUAL(*e, "e2");
  BOOST_CHECK(e.valid());
  BOOST_CHECK(static_cast<bool>(e));

  BOOST_REQUIRE_NO_THROW(e2.get());
  BOOST_CHECK_EQUAL(e2.get(), "");
  BOOST_CHECK_EQUAL(*e2, "");
  BOOST_CHECK(e2.valid());
  BOOST_CHECK(static_cast<bool>(e2));
}
#endif

BOOST_AUTO_TEST_CASE(expected_from_emplace)
{
  // From emplace constructor.
  expected<std::string> e(emplace, "emplace");
  BOOST_CHECK_EQUAL(e.get(), "emplace");

  // From emplace method.
  e.emplace("emplace method");
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), "emplace method");
  BOOST_CHECK_EQUAL(*e, "emplace method");
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_from_move_value)
{
  expected<std::string> e("v");

  std::string value = "my value";
  // From assignment operator.
  e = std::move(value);
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), "my value");
  BOOST_CHECK_EQUAL(*e, "my value");
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}
#endif

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(expected_factories)

BOOST_AUTO_TEST_CASE(expected_from_emplace)
{
  // From emplace factory.
  boost::expected<std::string> e = make_expected<std::string>("emplace");
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), "emplace");
  BOOST_CHECK_EQUAL(*e, "emplace");
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_emplace_error)
{
  // From emplace factory.
  boost::expected<std::string, ERROR_CONDITION_NS::error_condition> e = 
    make_expected<std::string, ERROR_CONDITION_NS::error_condition>("emplace");
  BOOST_REQUIRE_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), "emplace");
  BOOST_CHECK_EQUAL(*e, "emplace");
  BOOST_CHECK(e.valid());
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK(static_cast<bool>(e));
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_exception_catch)
{
  // From catch block
  try
  {
    throw test_exception();
  }
  catch(...)
  {
    expected<int> e = make_exceptional_expected<int>();

    BOOST_REQUIRE_THROW(e.get(), std::exception);
    BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
    BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif
  }
}


#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_from_error_catch_exception)
{ 
  // From catch block
  try
  {
    throw test_exception();
  }
  catch(...)
  {
    auto throw_lambda = [](){ make_exceptional_expected<int, ERROR_CONDITION_NS::error_condition>();};

    BOOST_CHECK_THROW(throw_lambda(), test_exception);
  }
}
#endif

BOOST_AUTO_TEST_CASE(expected_from_exception_ptr)
{
  // From exception_ptr constructor.
  boost::expected<int> e = make_exceptional_expected<int>(MAKE_EXCEPTION_PTR(test_exception()));
  BOOST_CHECK_THROW(e.get(), test_exception);
  BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_noexcept_fun)
{
  BOOST_CHECK_NO_THROW(make_noexcept_expected(throwing_fun));
  expected<int> e = make_noexcept_expected(throwing_fun);
  BOOST_CHECK_THROW(e.get(), std::exception);
  BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif

  e = make_noexcept_expected(nothrowing_fun);
  BOOST_CHECK_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.get(), 4);
  BOOST_CHECK_EQUAL(*e, 4);
  BOOST_CHECK_EQUAL(e.valid(), true);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), true);
#endif

  BOOST_CHECK_THROW(make_noexcept_expected<ERROR_CONDITION_NS::error_condition>(throwing_fun), test_exception);

  BOOST_CHECK_NO_THROW(make_noexcept_expected<ERROR_CONDITION_NS::error_condition>(nothrowing_fun));
  expected<int, ERROR_CONDITION_NS::error_condition> e2 = make_noexcept_expected<ERROR_CONDITION_NS::error_condition>(nothrowing_fun);
  BOOST_CHECK_NO_THROW(e2.get());
  BOOST_CHECK_EQUAL(e2.get(), 4);
  BOOST_CHECK_EQUAL(*e2, 4);
  BOOST_CHECK_EQUAL(e2.valid(), true);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e2), true);
#endif
}

BOOST_AUTO_TEST_CASE(expected_from_noexcept_void_fun)
{
  BOOST_CHECK_NO_THROW(make_noexcept_expected(void_throwing_fun));
  expected<void> e = make_noexcept_expected(void_throwing_fun);
  BOOST_CHECK_THROW(e.get(), std::exception);
  BOOST_CHECK_EQUAL(e.valid(), false);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), false);
#endif

  e = make_noexcept_expected(do_nothing_fun);
  BOOST_CHECK_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(e.valid(), true);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e), true);
#endif

  BOOST_CHECK_THROW(make_noexcept_expected<ERROR_CONDITION_NS::error_condition>(void_throwing_fun), test_exception);

  BOOST_CHECK_NO_THROW(make_noexcept_expected<ERROR_CONDITION_NS::error_condition>(do_nothing_fun));
  expected<void, ERROR_CONDITION_NS::error_condition> e2 = make_noexcept_expected<ERROR_CONDITION_NS::error_condition>(do_nothing_fun);
  BOOST_CHECK_NO_THROW(e2.get());
  BOOST_CHECK_EQUAL(e2.valid(), true);
#ifdef EXPECTED_CPP11_TESTS
  BOOST_CHECK_EQUAL(static_cast<bool>(e2), true);
#endif
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(expected_swap)

BOOST_AUTO_TEST_CASE(expected_swap_value)
{
  // From value constructor.
  expected<int> e(5);
  expected<int> e2(8);

  e.swap(e2);

  BOOST_CHECK_EQUAL(e.get(), 8);
  BOOST_CHECK_EQUAL(e2.get(), 5);

  e2.swap(e);

  BOOST_CHECK_EQUAL(e.get(), 5);
  BOOST_CHECK_EQUAL(e2.get(), 8);
}

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_swap_exception)
{
  // From value constructor.
  expected<int> e = make_exceptional_expected<int>(MAKE_EXCEPTION_PTR(std::invalid_argument("e")));
  expected<int> e2 = make_exceptional_expected<int>(MAKE_EXCEPTION_PTR(std::invalid_argument("e2")));

  e.swap(e2);

  auto equal_to_e = [](const std::invalid_argument& except) { return std::string(except.what()) == "e"; };
  auto equal_to_e2 = [](const std::invalid_argument& except) { return std::string(except.what()) == "e2"; };

  BOOST_CHECK_EXCEPTION(e.get(), std::invalid_argument, equal_to_e2);
  BOOST_CHECK_EXCEPTION(e2.get(), std::invalid_argument, equal_to_e);

  e2.swap(e);

  BOOST_CHECK_EXCEPTION(e.get(), std::invalid_argument, equal_to_e);
  BOOST_CHECK_EXCEPTION(e2.get(), std::invalid_argument, equal_to_e2);
}
#endif

BOOST_AUTO_TEST_CASE(expected_swap_function_value)
{
  // From value constructor.
  expected<int> e(5);
  expected<int> e2(8);

  swap(e, e2);

  BOOST_CHECK_EQUAL(e.get(), 8);
  BOOST_CHECK_EQUAL(e2.get(), 5);

  swap(e, e2);

  BOOST_CHECK_EQUAL(e.get(), 5);
  BOOST_CHECK_EQUAL(e2.get(), 8);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(expected_then)

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_then)
{
  auto fun = [](bool b)
  { 
    if(b) 
      return expected<int>(5);
    else
      return make_exceptional_expected<int>(MAKE_EXCEPTION_PTR(test_exception()));
  };

  auto add_five = [](int sum)
  {
    return sum + 5;
  };

  expected<int> e = fun(true).then(add_five);
  BOOST_CHECK_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(*e, 10);

  e = fun(true).then(add_five).then(add_five);
  BOOST_CHECK_NO_THROW(e.get());
  BOOST_CHECK_EQUAL(*e, 15);

  e = fun(false).then(add_five).then(add_five);
  BOOST_CHECK_THROW(e.get(), test_exception);
}

BOOST_AUTO_TEST_CASE(expected_void_then)
{
  auto fun = [](bool b)
  { 
    if(b) 
      return expected<void>();
    else
      return make_exceptional_expected<void>(MAKE_EXCEPTION_PTR(test_exception()));
  };

  auto launch_except = []()
  {
    throw test_exception();
  };

  auto do_nothing = [](){};

  expected<void> e = fun(true).then(do_nothing);
  BOOST_CHECK_NO_THROW(e.get());

  e = fun(false).then(do_nothing);
  BOOST_CHECK_THROW(e.get(), test_exception);

  e = fun(true).then(launch_except);
  BOOST_CHECK_THROW(e.get(), std::exception);
}
#endif

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(expected_recover)

#ifdef EXPECTED_CPP11_TESTS
BOOST_AUTO_TEST_CASE(expected_recover)
{
  auto fun = [](bool b)
  { 
    if(b) 
      return expected<int>(5);
    else
      return make_exceptional_expected<int>(MAKE_EXCEPTION_PTR(test_exception()));
  };
  
  auto then_launch = [](int) -> int
  {
    throw test_exception();
  };

  auto add_five = [](int sum)
  {
    return sum + 5;
  };

  auto recover_error = [](exception_ptr_type p)
  {
    return 0;
  };

  auto recover_error_silent_failure = [](exception_ptr_type p)
  {
    return make_exceptional_expected<int>(p);
  };

  auto recover_error_failure = [](exception_ptr_type p) -> expected<int>
  {
    throw test_exception();
  };

  BOOST_CHECK_EQUAL(fun(false).recover(recover_error).get(), 0);
  BOOST_CHECK_EQUAL(fun(true).recover(recover_error).get(), 5);
  BOOST_CHECK_EQUAL(fun(false).recover(recover_error_silent_failure).valid(), false);
  BOOST_CHECK_EQUAL(fun(false).recover(recover_error_failure).valid(), false);

  BOOST_CHECK_EQUAL(fun(true).then(add_five).recover(recover_error).get(), 10);
  BOOST_CHECK_EQUAL(fun(true).then(add_five).recover(recover_error_silent_failure).get(), 10);
  BOOST_CHECK_EQUAL(fun(true).then(add_five).recover(recover_error_failure).get(), 10);

  BOOST_CHECK_EQUAL(fun(false).recover(recover_error).then(add_five).get(), 5);
  BOOST_CHECK_EQUAL(fun(false).recover(recover_error).then(add_five).then(add_five).get(), 10);
  BOOST_CHECK_EQUAL(fun(false).recover(recover_error_failure).then(add_five).valid(), false);
  BOOST_CHECK_EQUAL(fun(false).then(add_five).recover(recover_error_failure).then(add_five).valid(), false);
  BOOST_CHECK_EQUAL(fun(false).then(add_five).recover(recover_error_silent_failure).then(add_five).valid(), false);

  BOOST_CHECK_EQUAL(fun(true).then(then_launch).recover(recover_error).get(), 0);
  BOOST_CHECK_EQUAL(fun(true).then(then_launch).recover(recover_error).then(add_five).get(), 5);
  BOOST_CHECK_EQUAL(fun(true).then(then_launch).recover(recover_error_failure).valid(), false);
}

BOOST_AUTO_TEST_CASE(expected_void_recover)
{
  auto fun = [](bool b)
  { 
    if(b) 
      return expected<void>();
    else
      return make_exceptional_expected<void>(MAKE_EXCEPTION_PTR(test_exception()));
  };

  auto then_launch = []() -> void
  {
    throw test_exception();
  };

  auto do_nothing = [](){};

  auto recover_error = [](exception_ptr_type p)
  {
    return expected<void>();
  };

  auto recover_error_silent_failure = [](exception_ptr_type p)
  {
    return make_exceptional_expected<void>(p);
  };

  auto recover_error_failure = [](exception_ptr_type p) -> expected<void>
  {
    throw test_exception();
  };

  // The recover doesn't alter the expected if it's valid.
  BOOST_CHECK_EQUAL(fun(true).recover(recover_error_failure).valid(), true);

  // Simple recover tests.
  BOOST_CHECK_EQUAL(fun(false).recover(recover_error).valid(), true);
  BOOST_CHECK_EQUAL(fun(false).recover(recover_error_failure).valid(), false);
  BOOST_CHECK_EQUAL(fun(false).recover(recover_error_silent_failure).valid(), false);

  // With a then between.
  BOOST_CHECK_EQUAL(fun(false).then(do_nothing).recover(recover_error_failure).valid(), false);

  BOOST_CHECK_NO_THROW(fun(false).then(then_launch).recover(recover_error).get());
}
#endif

BOOST_AUTO_TEST_SUITE_END()