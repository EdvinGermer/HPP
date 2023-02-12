echo
echo No optimisation
gcc -o math_functions math_functions.c -lm
time ./math_functions

echo
echo O1 optimisation
gcc -O1 -o math_functions math_functions.c -lm
time ./math_functions

echo
echo O2 optimisation
gcc -O2 -o math_functions math_functions.c -lm
time ./math_functions

echo
echo O3 optimisation
gcc -O3 -o math_functions math_functions.c -lm
time ./math_functions


echo echo