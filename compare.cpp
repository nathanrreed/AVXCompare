#include <vector>
#include <iostream>

#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <sys/timeb.h>

int main(){
	std::cout << "Comparing the speed of Normal execution and using AVX2\nFor ceil(sqrt(((x * 100.0) - 7.0)) / 1.1)\nwhere x is a random number between 0 and 10000.\n";
	
	struct timeb startTime;
    struct timeb endTime;

	std::vector<std::vector<float>> one(6400 , std::vector<float> (12800));
	std::vector<std::vector<float>> two(6400 , std::vector<float> (12800));
	
	float temp;
	int x, y, i = 0, j, max = 3;
	for(x = 0; x < one.size(); x++){
		for(y = 0; y < one[0].size(); y++){
			temp = rand() % 10000;
			one[x][y] = temp;
			two[x][y] = temp;
		}
	}
	
	/*for(j = 0; j < 10; j++){
		std::cout << one[0][j] << " == " << two[0][j] << "\n";
	}*/

	
	double average = 0;
	
	std::cout << "Normal\n";
	while(i < max){
		ftime(&startTime);
		
		for(x = 0; x < one.size(); x++){
			for(y = 0; y < one[0].size(); y++){
				temp = ceil( sqrt(((one[x][y] * 100.0) - 7.0)) / 1.1);
				
				if(temp < 100 || isnan(temp)){
					one[x][y] = 0;
				}else{
					one[x][y] = temp;
				}
			}
		}

		ftime(&endTime);
		average += (double)((endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
		i++;
	}
	
	std::cout << "Average: " << average / max << "ms\n";
	
	average = 0;
	i = 0;
	__m256 a, b, c, d, e, f;
	__m256i index = _mm256_set_epi32(7,6,5,4,3,2,1,0);
	b = _mm256_set1_ps(100.0);
	c = _mm256_set1_ps(7.0);
	e = _mm256_set1_ps(1.1);
		
	std::cout << "AVX2\n";
	while(i < max){
		ftime(&startTime);
		for(x = 0; x < one.size(); x++){
			for(y = 0; y < one[0].size(); y+=8){
				a = _mm256_i32gather_ps(&two[x][y], index, 4);
				d = _mm256_mul_ps(a, b);
				d = _mm256_sub_ps(d, c);
				d = _mm256_sqrt_ps(d);
				d = _mm256_div_ps(d, e);
				d = _mm256_ceil_ps(d);

				f = _mm256_cmp_ps(d, b, _CMP_GE_OQ);
				d = _mm256_and_ps(f, d);
				
				_mm256_storeu_ps(&two[x][y], d);
			}
		}

		ftime(&endTime);
		average += (double)((endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
		i++;
	}
	
	std::cout << "Average: " << average / max << "ms\n";
	
	
	/*for(j = 0; j < 100; j++){
		std::cout << one[0][j] << " == " << two[0][j] << "\n";
	}*/
	return 0;
}

