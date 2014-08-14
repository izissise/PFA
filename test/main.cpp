#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <fstream>

using namespace CppUnit;

void print_all_tests(Test *t, const char *spacer, std::ostream& os)
{
  os << t->getName() << spacer;
  for (int i = 0; i < t->getChildTestCount(); i++)
    print_all_tests(t->getChildTestAt(i), spacer, os);
}

int main(int argc, char* argv[])
{
  TestResult controller;
  TestResultCollector result;
  controller.addListener(&result);

  TextUi::TestRunner runner;
  TestFactoryRegistry& registry = TestFactoryRegistry::getRegistry();
  Test* testToRun = registry.makeTest();

  if(argc > 1)
    {
      try {
          testToRun = testToRun->findTest(argv[1]);
        }
      catch (...) {
          std::cerr << argv[1] << " isn't a valid test name." << std::endl;
          std::cerr << "Valid tests are: " << std::endl;
          print_all_tests(testToRun, "\n", std::cerr);
          return 1;
          throw;
        }
    }

  runner.addTest(testToRun);
  runner.run(controller);

  bool success = result.wasSuccessful();

//  std::ofstream xmlFileOut("cpptestresults.xml");
//  XmlOutputter xmlOut(&result, xmlFileOut);
//  xmlOut.write();
//  xmlFileOut.close();

  if (!success)
    {
      TextOutputter textOutput(&result, std::cout);
      textOutput.write();
    }

  return success ? 0 : 1;
}

