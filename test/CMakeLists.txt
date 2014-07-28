FILE(GLOB UnitTests_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
				"*Test.cpp")
MESSAGE("Found Test Files: ${UnitTests_SRCS}")
ADD_EXECUTABLE(UnitTester main.cpp ${UnitTests_SRCS})
TARGET_LINK_LIBRARIES(UnitTester cppunit dl)

include_directories("${PROJECT_SOURCE_DIR}/inc")

FOREACH(test ${UnitTests_SRCS})
	MESSAGE("  ### adding test ${test}")
	GET_FILENAME_COMPONENT(TestName ${test} NAME_WE)
    ADD_TEST(${TestName} UnitTester ${TestName})
ENDFOREACH(test)


