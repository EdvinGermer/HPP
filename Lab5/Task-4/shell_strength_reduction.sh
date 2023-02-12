echo
echo No optimisation
gcc -o strength_reduction strength_reduction.c
time ./strength_reduction

echo
echo O1 optimisation
gcc -O1 -o strength_reduction strength_reduction.c
time ./strength_reduction

echo
echo O2 optimisation
gcc -O2 -o strength_reduction strength_reduction.c
time ./strength_reduction

echo
echo O3 optimisation
gcc -O3 -o strength_reduction strength_reduction.c
time ./strength_reduction


echo echo