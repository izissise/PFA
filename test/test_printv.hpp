#ifndef TEST_PRINTV_HPP_INCLUDED
# define TEST_PRINTV_HPP_INCLUDED

# include <cppunit/TestCase.h>
# include <cppunit/extensions/HelperMacros.h>

# include <sstream>
# include <stdexcept>

# include "printv.hpp"

class TestPrintv : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE(TestPrintv);
      CPPUNIT_TEST(randTest);
      CPPUNIT_TEST(lessArgTest);
      CPPUNIT_TEST(moreArgTest);
  CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void randTest();
    void lessArgTest();
    void moreArgTest();

private:
    std::stringstream *fixture;
};

#endif // TEST_PRINTV_HPP_INCLUDED
