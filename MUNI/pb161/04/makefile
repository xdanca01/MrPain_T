SRC_D = d1_art.cpp d2_numbers.cpp d3_refs.cpp d4_pool.cpp
SRC_E = e1_diameter.cpp e2_circle.cpp e3_index.cpp
SRC_P = p1_format.cpp p2_least.cpp p3_area.cpp p4_zipper.cpp p5_rpn.cpp p6_eval.cpp
SRC_R = r1_complex.cpp r3_search.cpp
SRC_T =

CXX      = /packages/run.64/gcc-10.2/bin/c++
TIDY     = /packages/run.64/llvm-10.0.1/bin/clang-tidy
VALGRIND = valgrind
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -g -MD -MP

BIN_T    = ${SRC_T:.cpp=}
BIN_D    = ${SRC_D:.cpp=}
BIN_E    = ${SRC_E:.cpp=}
BIN_P    = ${SRC_P:.cpp=}
BIN_R    = ${SRC_R:.cpp=}

BIN      = $(BIN_T) $(BIN_D) $(BIN_E) $(BIN_P) $(BIN_R)
SRC      = $(SRC_T) $(SRC_D) $(SRC_E) $(SRC_P) $(SRC_R)
DEP      = ${BIN_D:%=.%.d} ${BIN_E:%=.%.d} ${BIN_T:%=.%.d} ${BIN_E:%=.sol.%.d}

_ENV   = CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" TIDY="$(TIDY)" VALGRIND="$(VALGRIND)"

all: $(BIN)

clean:
	@rm -f $(BIN) $(DEP)
	@rm -f *.core core *~ a.out .check.*.out valgrind.out a.out

.cpp: makefile .helper.sh
	$(CXX) -o $@ $(CXXFLAGS) -MD -MP -MF ${@:%=.%.d} $<
	@env $(_ENV) sh .helper.sh tidy   $<
	@env $(_ENV) sh .helper.sh grind ./$@

$(BIN_T): makefile .helper.sh
	@echo building and testing $@
	@env $(_ENV) sh .helper.sh cxx    $@.cpp -o $@
	@env $(_ENV) sh .helper.sh tidy   $@.cpp
	@env $(_ENV) sh .helper.sh grind ./$@

-include $(DEP)

.PHONY: all clean
