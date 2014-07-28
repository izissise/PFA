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
         5, "wor%ld", 854L, "");
  std::cout << fixture.str() << std::endl;
  CPPUNIT_ASSERT(true);
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
