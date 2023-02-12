echo
echo No optimization
gcc -o lookup lookup.c
time ./lookup

echo
echo O1 optimization
gcc -O1 -o lookup lookup.c
time ./lookup

echo
echo O2 optimization
gcc -O2 -o lookup lookup.c
time ./lookup

echo
echo O3 optimization
gcc -O3 -o lookup lookup.c
time ./lookup

echo
echo

