#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <stdexcept>
#include <thread>

#include "TimeHandling.hpp"

class TestTimeHandling : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE(TestTimeHandling);
  CPPUNIT_TEST(targetFrameDuration);
  CPPUNIT_TEST(frameDurationAlign);
  CPPUNIT_TEST_SUITE_END();

public:
  TestTimeHandling()
  {};
  virtual ~TestTimeHandling() {};

  void setUp()
  {};

  void tearDown()
  {};

protected:

  void targetFrameDuration()
  {
    auto target = std::chrono::milliseconds(16);
    decltype(target) res;

    TimeHandling time(target);
    res = time.GetTargetFrameDuration();
    CPPUNIT_ASSERT(target == res);
  }

  template<class func>
  std::chrono::nanoseconds benchmark_func(func f)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();
    f();
    end = std::chrono::high_resolution_clock::now();
    return end - start;
  }

  template<class func, typename Args>
  std::chrono::nanoseconds benchmark_func(func f, Args&& args)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();
    f(std::forward<Args>(args));
    end = std::chrono::high_resolution_clock::now();
    return end - start;
  }

  void frameDurationAlign()
  {
    std::chrono::milliseconds target(8);

    TimeHandling time(target);
    std::chrono::nanoseconds res = benchmark_func([&time] {
      time.start();
      time.endFrame();
    });

    decltype(target) resTar = std::chrono::duration_cast<decltype(target)>(res);
    CPPUNIT_ASSERT(resTar == target);
  }

private:
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTimeHandling);
