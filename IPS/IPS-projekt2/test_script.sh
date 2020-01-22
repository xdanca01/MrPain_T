#!/bin/bash

diff <(./pgrep 1 '.*wheel.*' 1 < sudoers) <(./my_grep 1 '.*wheel.*' 1 < sudoers) -s
diff <(./pgrep 0 '.*wheel.*' -1 < sudoers) <(./my_grep 0 '.*wheel.*' -1 < sudoers) -s
diff <(./pgrep 1 '.*wheel.*' -1 ".* ALL.*" 1 < sudoers) <(./my_grep 1 '.*wheel.*' -1 ".* ALL.*" 1 < sudoers) -s
diff <(./pgrep 1 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 < sudoers) <(./my_grep 1 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 < sudoers) -s
diff <(./pgrep 2 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 < sudoers) <(./my_grep 2 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 < sudoers) -s
diff <(./pgrep 2 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 ".*root.*" 1  < sudoers) <(./my_grep 2 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 ".*root.*" 1  < sudoers) -s
diff <(./pgrep 3 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 ".*root.*" 1  < sudoers) <(./my_grep 3 '.*wheel.*' -1 ".* ALL.*" 1 ".*cd.*" 1 ".*root.*" 1  < sudoers) -s
diff <(./pgrep 3 '.*wheel.*' 1 ".* ALL.*" 2 ".*cd.*" 1 ".*root.*" 1  < sudoers) <(./my_grep 3 '.*wheel.*' 1 ".* ALL.*" 2 ".*cd.*" 1 ".*root.*" 1  < sudoers) -s
diff <(./pgrep 3 '.*wheel.*' 1 ".* ALL.*" 2 ".*cd.*" 1 ".*root.*" 1  ".*NOPASSWD.*" -5 < sudoers) <(./my_grep 3 '.*wheel.*' 1 ".* ALL.*" 2 ".*cd.*" 1 ".*root.*" 1  ".*NOPASSWD.*" -5 < sudoers) -s
diff <(./pgrep 7 ".*NETWORKING,.*" 1 ".*SOFTWARE,.*" 1 ".*SERVICES,.*" 1 ".*STORAGE,.*" 1 ".*DELEGATING,.*" 1 "PROCESSES,.*" 1 ".*LOCATE,.*" 1 ".*DRIVERS.*" 1 < sudoers) <(./my_grep 7 ".*NETWORKING,.*" 1 ".*SOFTWARE,.*" 1 ".*SERVICES,.*" 1 ".*STORAGE,.*" 1 ".*DELEGATING,.*" 1 "PROCESSES,.*" 1 ".*LOCATE,.*" 1 ".*DRIVERS.*" 1 < sudoers)  -s

diff <(./pgrep 2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 <group) <(./my_grep 2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 <group) -s
diff <(./pgrep 1 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 <group) <(./my_grep 1 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 <group) -s
diff <(./pgrep 2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 ".*journal.*" -5 <group) <(./my_grep 2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 ".*journal.*" -5 <group) -s
diff <(./pgrep 2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 ".*journal.*" -5 "root.*" 5 <group) <(./my_grep 2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 ".*journal.*" -5 "root.*" 5 <group) -s
diff <(./pgrep 2 ".*pgrep.*" 1 ".*pthread" 1 < zadani.txt) <(./my_grep 2 ".*pgrep.*" 1 ".*pthread" 1 < zadani.txt) -s
diff <(./pgrep 2 ".*pgrep.*" 1 ".*pthread.*" 1 < zadani.txt) <(./my_grep 2 ".*pgrep.*" 1 ".*pthread.*" 1 < zadani.txt) -s
diff <(./pgrep 1 ".*pgrep.*" 1 ".*pthread.*" -1 < zadani.txt)  <(./my_grep 1 ".*pgrep.*" 1 ".*pthread.*" -1 < zadani.txt) -s
