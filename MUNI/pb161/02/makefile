SRC_D = d1_stats.cpp d2_primes.cpp d3_iterate.cpp d4_newton.cpp
SRC_E = e1_fibonacci.cpp e2_normalize.cpp e3_accumulate.cpp
SRC_P = p1_rewrap.cpp p2_golden.cpp p3_divisors.cpp p4_midpoints.cpp p5_higher.cpp p6_fixpoint.cpp
SRC_R = r1_euler.cpp r2_approx.cpp r3_solve.cpp r4_sort.cpp r5_permute.cpp r6_bsearch.cpp
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
