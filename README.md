# AVXCompare
A comparision of the speed of AVX2 intrinsic functions to compute:
 
```math
ceil(sqrt(((x * 100.0) - 7.0)) / 1.1)
```
```
where x is a random number between 0 and 10000.
```

If that value is less than 100 it is set to 0.

This is computed on a 2d vector of size 6400 * 12800


Sample Output
```
Normal
Average: 1880.33ms
AVX2
Average: 448.333ms
```
