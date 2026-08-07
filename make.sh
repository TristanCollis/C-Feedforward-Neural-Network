# clang ./src/neuralnet.c -o ./target/out.exe

clang -g -O0 -Wall -Wextra ./src/matrix.c ./src/main.c -o ./target/out.exe