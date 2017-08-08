SRCDIR              = $(realpath $(dir $(lastword $(filter Makefile,$(MAKEFILE_LIST)))))

CPPFLAGS_EXTRA      = -Ipcre2++
CXXFLAGS_EXTRA      = -Wall -Wextra -Wno-unused-parameter -fvisibility=hidden -fvisibility-inlines-hidden -std=c++11
LDFLAGS_EXTRA       = -pthread

TESTER              = .lib/pcre2++-test

TESTER_CPPFLAGS     = -I$(SRCDIR)
TESTER_CPPFLAGS_PCH = -include .build/test/precompiled.h -Winvalid-pch
TESTER_LDFLAGS      =
TESTER_LDLIBS       = -lpcre2-8 -lpcre2-16 -lpcre2-32

TARGET              = $(TESTER)

TESTER_CXX_SOURCES  = \
	test/tester.cpp \
	test/basic_regex.cpp \
	test/regex_match.cpp \
	test/sub_match.cpp \
	test/regex_error.cpp

TESTER_GCH = .build/test/precompiled.h.gch
TESTER_PCH = test/precompiled.h

### Google Test
GTEST_DIR  = /usr/src/gtest
GTEST_SRCS = $(wildcard $(GTEST_DIR)/src/*.cc)
GTEST_HDRS = $(wildcard /usr/include/gtest/*.h) $(wildcard /usr/include/gtest/internal/*.h)
### END Google Test

ifeq ($(CXX),clang++)
# CXXFLAGS_EXTRA += -Weverything -Werror -Wno-c++98-compat -Wno-documentation -Wno-documentation-unknown-command -Wno-switch-enum -Wno-error=padded -Wno-error=exit-time-destructors -Wno-error=float-equal -Wno-error=global-constructors -Wno-error=double-promotion -Wno-error=weak-vtables -Wno-shadow
endif

ifeq ($(COVERAGE),1)
CXXFLAGS_EXTRA += -O0 -coverage
LDFLAGS_EXTRA  += -coverage
endif

ifeq ($(NOPCH),1)
TESTER_CPPFLAGS_PCH = 
endif

TESTER_CXX_OBJS    = $(patsubst %.cpp,.build/%.o,$(TESTER_CXX_SOURCES))

CXX_OBJS    = $(TESTER_CXX_OBJS)
OBJS        = $(CXX_OBJS)
DEPS        = $(patsubst %.o,%.d,$(OBJS)) $(patsubst %.gch,%.d,$(TESTER_GCH))
COV_GCDA    = $(patsubst %.o,%.gcda,$(OBJS))
COV_GCNO    = $(patsubst %.o,%.gcno,$(OBJS))

all: $(TARGET)

build_directory: .build .build/test
.build .build/test:
	mkdir -p "$@"

output_directory: .lib
.lib:
	mkdir -p "$@"

ifneq ($(NOPCH),1)
$(TESTER_GCH): $(TESTER_PCH) | build_directory
	$(CXX) $(CPPFLAGS) $(TESTER_CPPFLAGS) $(CPPFLAGS_EXTRA) $(CXXFLAGS) $(CXXFLAGS_EXTRA) -MMD -MP -MF"$(@:%.gch=%.d)" -MT"$(@:%.gch=%.d)" -MT"$@" -o "$@" "$<"

$(TESTER_CXX_OBJS): $(TESTER_GCH)
else
$(OBJS): | clean-pch
endif

$(TESTER): $(TESTER_CXX_OBJS) $(STATIC_LIBRARY) .build/gtest-all.o | output_directory
	$(CXX) $(LDFLAGS) $(TESTER_LDFLAGS) $(LDFLAGS_EXTRA) $^ $(TESTER_LDLIBS) $(LDLIBS) -o "$@"

ifeq ($(CXX),clang++)
.build/gtest-all.o: $(GTEST_SRCS) | output_directory
	$(CXX) -isystem $(GTEST_DIR) $(CXXFLAGS) -std=c++11 -g -O2 -c $(GTEST_DIR)/src/gtest-all.cc -o "$@"
else
.build/gtest-all.o: $(GTEST_SRCS) | output_directory
	$(CXX) -I$(GTEST_DIR) $(CXXFLAGS) -std=c++11 -g -O2 -c $(GTEST_DIR)/src/gtest-all.cc -o "$@"
endif

.build/test/%.o: test/%.cpp | build_directory
	$(CXX) $(CPPFLAGS) $(TESTER_CPPFLAGS) $(TESTER_CPPFLAGS_PCH) $(CPPFLAGS_EXTRA) $(CXXFLAGS) $(CXXFLAGS_EXTRA) -c "$<" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$@" -o "$@"

clean-deps:
	-rm -f $(DEPS)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
-include $(DEPS)
endif

clean:
	-rm -f $(TARGET)
	-rm -f $(OBJS)
	-rm -f $(DEPS)
	-rm -f $(COV_GCDA) $(COV_GCNO)
	-rm -f $(TESTER_GCH)

clean-coverage:
	-rm -rf .tracefile coverage $(COV_GCDA)

clean-pch:
	-rm -f $(TESTER_GCH)

test: $(TESTER)
	$(TESTER)

coverage: clean-coverage
	$(MAKE) $(TESTER) COVERAGE=1 MAKEFLAGS="$(MAKEFLAGS)"
	-$(TESTER)
	lcov -q -d .build -c -b . -o .tracefile
	lcov -q -r .tracefile "/usr/include/*" -o .tracefile
	genhtml -q --legend -o coverage .tracefile

.PHONY: build_directory output_directory clean coverage clean-coverage clean-pch
