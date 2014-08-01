#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <sstream>
#include <stdexcept>

#include "printv.hpp"

class TestPrintv : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE(TestPrintv);
  CPPUNIT_TEST(randTest);
  CPPUNIT_TEST(condTest);
  CPPUNIT_TEST(lessArgTest);
  CPPUNIT_TEST(moreArgTest);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
    fixture = new std::stringstream();
  };

  void tearDown()
  {
    delete fixture;
    fixture = NULL;
  };

protected:
  void randTest()
  {
    printv(*fixture, "Hello i'm testing %% printv with up to % arg like: % % %",
           5, "wor%ld", 854L, "a");
    CPPUNIT_ASSERT(fixture->str() == "Hello i'm testing % printv with up to 5 arg like: wor%ld 854 a");
  };

  void condTest()
  {
    printv(*fixture, "%%%%%", 5);
    CPPUNIT_ASSERT(fixture->str() == "%%5");
    fixture->str("");
    printv(*fixture, "%%%%");
    CPPUNIT_ASSERT(fixture->str() == "%%");
  };

  void lessArgTest()
  {
    try
      {
        printv(*fixture, "There is no arg but a %");
        CPPUNIT_ASSERT(false);
      }
    catch (std::runtime_error& e)
      {
        CPPUNIT_ASSERT(std::string("invalid format string: missing arguments") == e.what());
        return;
      }
    CPPUNIT_ASSERT(false);
  };

  void moreArgTest()
  {
    try
      {
        printv(*fixture, "There is no arg but passing some anyways.", 4563);
        CPPUNIT_ASSERT(false);
      }
    catch (std::logic_error& e)
      {
        CPPUNIT_ASSERT(std::string("extra arguments provided to printv") == e.what());
        return;
      }
    CPPUNIT_ASSERT(false);
  };

private:
  std::stringstream *fixture;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPrintv);
