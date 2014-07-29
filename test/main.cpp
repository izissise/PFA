#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <fstream>

#include <cstring>

int main(int argc, char* argv[]) {
  using namespace CppUnit;

  TestResult controller;
  TestResultCollector result;
  controller.addListener(&result);

  TextUi::TestRunner runner;
  TestFactoryRegistry& registry = TestFactoryRegistry::getRegistry();
  Test* testToRun = registry.makeTest();

  if(argc > 1)
    testToRun = testToRun->findTest(argv[1]);

  runner.addTest(testToRun);
  runner.run(controller);

  std::ofstream xmlFileOut("cpptestresults.xml");
  XmlOutputter xmlOut(&result, xmlFileOut);
  xmlOut.write();
  xmlFileOut.close();

  TextOutputter textOutput(&result, std::cout);
  textOutput.write();

  return 0;
}

