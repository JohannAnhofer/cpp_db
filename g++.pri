*-g++*|clang {
  message(Applying extended compiler flags)

  NORMAL_CFLAGS = -Wno-long-long -ansi
  NORMAL_CXXFLAGS = \
    -Wnon-virtual-dtor -Wundef -Wcast-align \
    -Wchar-subscripts -Wpointer-arith \
    -Wwrite-strings -Wpacked -Wformat-security \
    -Wmissing-format-attribute -Woverloaded-virtual

# to treat warnings as errors
#  NORMAL_CXXFLAGS += -Werror

  NORMAL_CFLAGS += -pedantic

  USABLE_CXXFLAGS = -Wold-style-cast
  HARD_CXXFLAGS = -Weffc++ -Wshadow
  PITA_CXXFLAGS = -Wunreachable-code

  QMAKE_CFLAGS   += $$NORMAL_CFLAGS
  QMAKE_CXXFLAGS += $$NORMAL_CFLAGS $$NORMAL_CXXFLAGS

  #to enable profiling
  #QMAKE_CXXFLAGS_DEBUG += -pg
  #QMAKE_LFLAGS_DEBUG += -pg

  QMAKE_CXXFLAGS_WARN_ON += $$USABLE_CXXFLAGS
} else {
    message(Skipping extended compiler flags)
}

