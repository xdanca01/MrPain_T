SRC_D = d1_queue.cpp d2_split.cpp d3_glob.cpp
SRC_E = e1_iota.cpp e2_view.cpp e3_skip.cpp
SRC_P = p1_seq.cpp p2_filter.cpp p3_zip.cpp p4_nibble.cpp p5_tree.cpp p6_scan.cpp
SRC_R = r1_map.cpp r2_range.cpp r3_permute.cpp
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
