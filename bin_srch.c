#include <stdio.h>
#include <float.h>
#include <math.h>

int compare_int( void *a, void *b)
{
    int x = *(int*)a;
    int y = *(int*)b;

    if( x == y)
        return 0;
    if( x > y )
        return 1;
    return -1;      //x < y
}

int compare_char( void *a, void *b)
{
    char x = *(char*)a;
    char y = *(char*)b;

    if( x == y)
        return 0;
    if( x > y )
        return 1;
    return -1;      //x < y
}
/* compare_float - compare 2 floating point numbers. Floating point numbers can not be expressed accurately in binary.
 * There will always be rounding errors. So I consider them equal if their diff is less than a fixed "epsilon" value.
 * This is not the best way to get accuracy, but we are aiming for consistancy and not accuracy, so this will do.
 * Input 
 * @a and @b: pointers to valid floating point values
 * Return 
 * 0 if *a == *b (within the range of epsilon)
 * 1 if *a > *b
 * -1 if *a < *b 
*/ 
int compare_float( void *a, void *b)
{
    float x = *(float*)a;
    float y = *(float*)b;
    printf("diff %f\n", fabs(x - y));
    if( fabs(x - y) <= FLT_EPSILON ){
        printf("%f == %f\n", x, y);		//diff is smallar than epsilon so consider them equal
        return 0;
    }
    if( x > y ){
        printf("%f > %f\n", x, y);
        return 1;
    }
    printf("%f <  %f\n", x, y);
    return -1;      					//x < y
}


/* bin_srch - does binary search, not to be called directly, usr wraper function binary_search()
 * parameters and return same as binary_search()	 
 */ 
static int bin_srch( void *arr, int d_size, int s, int e, void *key, int (*compare_fn)(void*, void*))
{
    int mid = 0, cmp = 0;
    if(s > e){
        return -1;          						//key not found
    }
    mid = (s + e) / 2;								//get mid point of array
    cmp = compare_fn(arr + (mid * d_size), key);	//do not use == to compare since it might not work for some datatypes like float
    if (cmp == 0)      
        return mid;         						//key found in arr[mid]
    if (cmp > 0)    								
        return bin_srch(arr, d_size, s, mid - 1, key, compare_fn);	//arr[mid] > key, key is in left half
    return bin_srch(arr, d_size, mid + 1, e, key, compare_fn); 		//arr[mid] < key, key is in right half
}

/* binary_search - search for key in an array of any data type using binary search logic
Inputs
 * @array: pointer to first byte of data array
 * @arr_size: number of bytes in array
 * @data_size: size of each element (in bytes)
 * @key: pointer to key to be search. It's size is assumed to be same as data_size
 * @compare_fn: compare function for this data type. This takes 2 pointers to data to be compared, returns 0 if equal, 1 if 1st > 2nd, -1 if 1st < 2nd
Return
 * index of key in array if found(This is NOT byte index), -1 if not found, -2 if error
 */
int binary_search(void *array, int arr_size, int data_size, void *key, int (*compare_fn)(void*, void*))
{   
    int n = 0;	// no. of elements in array
    if(array == NULL || arr_size <= 0 || data_size <=0 || key == NULL || compare_fn == NULL){
        return -2;							//some sanity checking
    }
    n = (arr_size / data_size); 
    return bin_srch(array, data_size, 0, n - 1 , key, compare_fn); //call helper function
}

int test_int(void)
{
	int i_a[] = {0, 4, 8, 9, 22};
    int i_key = 22;
	printf("res = %d\n, expected 4", binary_search(i_a, sizeof(i_a), sizeof(int), &i_key, &compare_int));
}
int main(void)
{
    int i_a[] = {0, 4, 8, 9, 22};
    int i_key = 5;
    char c_a[] = { 'a', 'c', 'd', 'g', 'y', 'z'};
    char c_key = 'y';
    float f[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22f};
    float key2 = 1.9f;
    
    //printf("res = %d\n", binary_search(b, sizeof(b), sizeof(char), &key1, &compare_char));
    //printf("res = %d\n", binary_search(f, sizeof(f), sizeof(float), &key2, &compare_float));
	test_int();
    return 0;
}


