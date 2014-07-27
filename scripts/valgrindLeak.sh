#!/bin/bash

valgrind --xml=yes --xml-file=valgrind.xml Release/UnitTester
