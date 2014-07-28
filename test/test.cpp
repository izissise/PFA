#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/necessary/CppUnitIncludes/>

int main()
{
  CppUnit::TestResult controller;
  CppUnit::TestResultCollector result;
  controller.addListener(&result);

  CppUnit::TextUi::TestRunner runner(controller);
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(registry.makeTest());
  bool wasSuccessful = runner.run("", false);

// output result in xml format
  std::ofstream xmlFileOut("cpptestresults.xml");
  CppUnit::XmlOutputter xmlOut(&result, xmlFileOut);
  xmlOut.write();

  return wasSuccessful;
}
