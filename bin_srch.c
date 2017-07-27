#include <stdio.h>
#include <float.h>
#include <math.h>

int compare_int(void *a, void *b)
{
    int x = *(int*)a;
    int y = *(int*)b;

    if (x == y)
        return 0;
    if (x > y)
        return 1;
    return -1;      //x < y
}

int compare_char(void *a, void *b)
{
    char x = *(char*)a;
    char y = *(char*)b;

    if (x == y)
        return 0;
    if (x > y)
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
int compare_float(void *a, void *b)
{
    float x = *(float*)a;
    float y = *(float*)b;
    if (fabs(x - y) <= FLT_EPSILON){
        return 0;						//diff is smallar than epsilon so consider them equal

    }
    if (x > y){
        return 1;
    }
    return -1;      	//x < y
}


/* bin_srch - does binary search, not to be called directly, usr wraper function binary_search()
 * parameters and return same as binary_search()	 
 */ 
static int bin_srch(void *arr, int d_size, int s, int e, void *key, int (*compare_fn)(void*, void*))
{
    int mid = 0, cmp = 0;
    if (s > e){
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

/* binary_search - search for key in an array  of any data type using binary search logic. 
 * Array needs to be sorted in non decreasing order
 * Inputs - 
 * @array: pointer to first byte of data array
 * @arr_size: number of bytes in array
 * @data_size: size of each element (in bytes)
 * @key: pointer to key to be searched. It's size is assumed to be same as @data_size
 * @compare_fn: compare function for this data type. This takes 2 pointers to data to be compared,
 * returns 0 if equal, 1 if 1st > 2nd, -1 if 1st < 2nd
 * 
 * Retuns index of key in array if found(This is NOT byte index), -1 if not found, -2 if error
 */
int binary_search(void *array, int arr_size, int data_size, void *key, int (*compare_fn)(void*, void*))
{   
    int n = 0;	// no. of elements in array
    if (array == NULL || arr_size <= 0 || data_size <=0 || key == NULL || compare_fn == NULL){
        return -2;							//some sanity checking
    }
    n = (arr_size / data_size); 
    return bin_srch(array, data_size, 0, n - 1 , key, compare_fn); //call helper function
}

/* test cases and main function */
int test_int(void)
{
	int i_a[] = {0, 4, 8, 9, 22};
    int i_key = 22;
	printf("expected = 4, result = %d\n", binary_search(i_a, sizeof(i_a), sizeof(int), &i_key, &compare_int));
}

int test_float(void)
{
	//test odd elements
    float f1[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22f};
    float key1 = 1.9f;
	printf("expected = 3, result = %d\n", binary_search(f1, sizeof(f1), sizeof(float), &key1, &compare_float));
	
	//test even elements
	float f2[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22, 2.40};
    float key2 = 0.2f;
	printf("expected = 0, result = %d\n", binary_search(f2, sizeof(f2), sizeof(float), &key2, &compare_float));	

	//test 0 elements
	float f3[] = {};
    float key3 = 0.2f;
	printf("expected = -2, result = %d\n", binary_search(f3, sizeof(f3), sizeof(float), &key3, &compare_float));
	
	//test key not present
	float f4[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22, 2.40};
    float key4 = 0.9f;
	printf("expected = -1, result = %d\n", binary_search(f4, sizeof(f4), sizeof(float), &key4, &compare_float));
	
	//test small diff numbers
	//even though the key is not in array, due to epsilon comparision, it picks index 2 as key.
	float f5[] = {0.20000001f, 0.20000004f, 0.20000006f, 0.20000008f, 2.22, 2.40};
    float key5 = 0.20000005f;
	printf("expected = -1, result = %d\n", binary_search(f5, sizeof(f5), sizeof(float), &key5, &compare_float));
	
	return 0;
}

int main(void)
{
	test_int();
	test_float();
    return 0;
}


