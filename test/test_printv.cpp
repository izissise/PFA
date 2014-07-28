#include "test_printv.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPrintv);

void TestPrintv::setUp()
{
  fixture = new std::stringstream();
}

void TestPrintv::tearDown()
{
  delete fixture;
  fixture = NULL;
}

void TestPrintv::randTest()
{
  printv(fixture, "Hello i'm testing %% printv with up to % arg like: % % %",
         5, "wor%ld", 854L, "a");
  CPPUNIT_ASSERT(fixture.str() == "Hello i'm testing % printv with up to 5 arg like: wor%ld 854 a");
}

void TestPrintv::condTest()
{
  printv(fixture, "%%%%%", 5);
  CPPUNIT_ASSERT(fixture.str() == "%%5");
}

void TestPrintv::lessArgTest()
{
  try
    {
      printv(fixture, "There is no arg but a %");
    }
  catch (std::exception& e)
    {

    }
  CPPUNIT_ASSERT(true);
}

void TestPrintv::moreArgTest()
{
  try
    {
      printv(fixture, "There is no arg but passing some anyways.", 4563);
    }
  catch (std::exception& e)
    {

    }
  CPPUNIT_ASSERT(true);
}
