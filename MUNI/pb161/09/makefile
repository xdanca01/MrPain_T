SRC_D = d1_zipper.cpp d2_expr.cpp d3_fold.cpp d4_rel.cpp
SRC_E = e1_iota.cpp e2_quot.cpp e3_split.cpp
SRC_P = p1_circular.cpp p2_buffer.cpp p3_stats.cpp p4_sparse.cpp p5_bbox.cpp p6_visit.cpp
SRC_R = r1_tfold.cpp r2_tmap.cpp r3_monoid.cpp
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
