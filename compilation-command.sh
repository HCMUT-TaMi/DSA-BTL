# Compile
INCLUDE1="./include"
INCLUDE2="./include/tensor"
INCLUDE3="./include/sformat"
INCLUDE4="./include/ann"
INCLUDE5="./demo"
SRC1="./src/ann/"
SRC2="./src/tensor/"
# MAIN="./src/program.cpp"
MAIN="./main.cpp"

#g++ -std=c++17 -I "$INCLUDE1" -I "$INCLUDE2" -I "$INCLUDE3" -I "$INCLUDE4" -I "$INCLUDE5" $(find $SRC1 -type f -iregex ".*\.cpp") "$SRC2"/*.cpp "$MAIN"  -o program
g++ -std=c++17 -I "$INCLUDE1" -I "$INCLUDE5" -I  "$MAIN"  -o main