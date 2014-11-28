#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Range2.hpp"

class TestRange2 : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE(TestRange2);
  CPPUNIT_TEST(simple_foreach);
  CPPUNIT_TEST(wrong_order);
  CPPUNIT_TEST(empty_range);
  CPPUNIT_TEST_SUITE_END();

public:
  TestRange2(void) {}
  virtual ~TestRange2(void) = default;

protected:
  /*
  ** Checks that a small Range2 is correctly
  ** iterated over with Range2::iterator
  */
  void simple_foreach(void)
  {
    Range2i	five_by_five({0, 0}, {4, 4});
    int		x{0};
    int		y{0};
    int		xMax{4};

    for (auto cursor : five_by_five) {
      CPPUNIT_ASSERT(cursor.x == x);
      CPPUNIT_ASSERT(cursor.y == y);
      ++x;
      if (x > xMax) {
	x = 0;
	++y;
      }
    }
  }

  /*
  ** Range parameters should be passed in
  ** ascending order
  */
  void wrong_order(void)
  {
    CPPUNIT_ASSERT_THROW(
      Range2i({0, 0}, {-1, -1}),
      std::invalid_argument);
  }

  /*
  ** An empty range must be valid
  */
  void empty_range(void)
  {
    Range2i();
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestRange2);
