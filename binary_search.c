#include <stdio.h>
#include <float.h>
#include <math.h>

//compare_int - compares 2 integers in normal way 
int compare_int(void *a, void *b)
{
    int x = *(int*)a;
    int y = *(int*)b;

    if (x == y)
        return 0;
    return (x > y)? 1: -1;
}

//compare_char - compares 2 chars  in normal way 
int compare_char(void *a, void *b)
{
    char x = *(char*)a;
    char y = *(char*)b;

    if (x == y)
        return 0;
    return (x > y)? 1: -1;
}

/* compare_float - compare 2 floating point numbers. Floating point numbers can not be expressed accurately in binary.
 * There will always be rounding errors. So I consider them equal if their difference is less than a fixed "epsilon" value.
 * This is not the best way to get accuracy, but we are prioritizing consistancy over accuracy, so this will do.
 *
 * Inputs
 * @a and @b: pointers to valid floating point values
 * Returns
 * 0 if *a == *b (within the range of epsilon)
 * 1 if *a > *b
 * -1 if *a < *b 
*/ 
int compare_float(void *a, void *b)
{
    float x = *(float*)a;
    float y = *(float*)b;

    if (fabs(x - y) < FLT_EPSILON)
        return 0;                        //diff is smallar than epsilon so consider them equal
    else if (x > y)
        return 1;
    else
        return -1;            //x < y
}

/* bin_srch - does binary search iteratively. Not to be called directly, use wraper function binary_search()
 * parameters and return same as binary_search()     
 */ 
static int bin_srch(void *arr, int d_size, int s, int e, void *key, int (*compare_fn)(void*, void*))
{
    int mid = 0, cmp = 0;
    while (s <= e)    {
        mid = (s + e) / 2;                                //get middle element
        cmp = compare_fn(arr + (mid * d_size), key);    //do not use == to compare since it might not work for some datatypes like float    
        if (cmp == 0)      
            return mid;                                    //key found in arr[mid]
         else if (cmp > 0)
            e = mid - 1;            //arr[mid] > key, key is in left half
        else
            s = mid + 1;            //arr[mid] < key, key is in right half
    }
    return -1;        // key not found        
}


/* binary_search - search for key in an array of any data type using binary search logic. 
 * Array needs to be sorted in non decreasing order
 * Inputs - 
 * @array: pointer to first byte of data array
 * @arr_size: number of bytes in array
 * @data_size: size of each element (in bytes)
 * @key: pointer to key to be searched. It's size is assumed to be same as @data_size
 * @compare_fn: pointer to compare function for this data type. This function takes 2 pointers to data to be compared,
 * and returns 0 if equal, 1 if 1st > 2nd, -1 if 1st < 2nd
 * 
 * Retuns index of key in array if found(This is NOT byte index), -1 if not found, -2 if error
 */
int binary_search(void *array, int arr_size, int data_size, void *key, int (*compare_fn)(void*, void*))
{   
    int n = 0;    // no. of elements in array
    if (array == NULL || arr_size <= 0 || data_size <= 0 || key == NULL || compare_fn == NULL){
        return -2;                            //some sanity checking
    }
    n = (arr_size / data_size); 
    return bin_srch(array, data_size, 0, n - 1 , key, compare_fn); //call helper function
}

/* test cases and main function */
int test_int(void)
{
    int fail = 0;
    
	//simple test
    int i_a[] = {0, 4, 8, 9, 22};
    int i_key = 22;
    int expected = 4;
    int result = binary_search(i_a, sizeof(i_a), sizeof(i_key), &i_key, &compare_int); 
    if (expected != result )    {
        printf("expected = %d, result = %d\n", expected, result);
        fail = 1;
    }
    return fail;
}

int test_float(void)
{
    int fail = 0;
    int expected = 0, result = 0;
    
    //test odd elements
    float f1[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22f};
    float key1 = 1.9f;
    expected = 3;
    result = binary_search(f1, sizeof(f1), sizeof(key1), &key1, &compare_float);
    if (expected != result )    {
        printf("expected = %d, result = %d\n", expected, result);
        fail = 1;
    }    
    
    //test even elements
    float f2[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22, 2.40};
    float key2 = 0.2f;
    expected = 0;
    result = binary_search(f2, sizeof(f2), sizeof(key2), &key2, &compare_float);
    if (expected != result )    {
        printf("expected = %d, result = %d\n", expected, result );
        fail = 1;
    }    
    
    //test 0 elements
    float f3[] = {};
    float key3 = 0.2f;
    expected = -2;
    result = binary_search(f3, sizeof(f3), sizeof(key3), &key3, &compare_float);
    if (expected != result )    {
        printf("expected = %d, result = %d\n", expected, result );
        fail = 1;
    }    
    
    //test key not present
    float f4[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22, 2.40};
    float key4 = 0.9f;
    expected = -1;
    result = binary_search(f4, sizeof(f4), sizeof(key4), &key4, &compare_float);
    if (expected != result )    {
        printf("expected = %d, result = %d\n", expected, result );
        fail = 1;
    }    
    
    //test small diff numbers
    //even though the key is not in array, due to epsilon comparision, it picks index 2 as key.
    float f5[] = {0.20000001f, 0.20000004f, 0.20000006f, 0.20000008f, 2.22, 2.40};
    float key5 = 0.20000005f;
    expected = -1;
    result = binary_search(f5, sizeof(f5), sizeof(key5), &key5, &compare_float);
    if (expected != result )    {
        printf("expected = %d, result = %d\n", expected, result );
        fail = 1;
    }    
    
    return fail;
}

int main(void)
{
    //intiger tests
    if (test_int() != 0)
        printf("int test fail\n");
    else 
        printf("int test pass\n");
    //float tests
    if (test_float() != 0)
        printf("float test fails\n");
    else 
        printf("int test pass\n");
    return 0;
}
