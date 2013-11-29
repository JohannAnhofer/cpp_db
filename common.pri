include(g++.pri)

CONFIG += c++11
CONFIG -= qt

# BOOST_ANY_REQUEST=1

# Check boost library
_BOOST_ROOT=$$(BOOST_ROOT)
isEmpty(_BOOST_ROOT) {
    message(\"BOOST libraries\" not detected...)
}
else {
   message(\"BOOST libraries\" detected in BOOST_ROOT = \"$$_BOOST_ROOT\")
   INCLUDEPATH+=$$_BOOST_ROOT
!isEmpty(BOOST_ANY_REQUEST) : DEFINES += USE_BOOST_ANY
}

