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

int compare_float( void *a, void *b)
{
    float x = *(float*)a;
    float y = *(float*)b;
    printf("diff %f\n", fabs(x - y));
    if( fabs(x - y) <= FLT_EPSILON /*0.0001*/ ){
        printf("%f == %f\n", x, y);
        return 0;
    }
    if( x > y ){
        printf("%f > %f\n", x, y);
        return 1;
    }
    printf("%f <  %f\n", x, y);
    return -1;      //x < y
}



static int bin_srch( void *arr, int d_size, int s, int e, void *key, int (*compare_fn)(void*, void*))
{
    int mid = 0, cmp = 0;
    if(s > e){
        return -1;          //not found
    }
    mid = (s + e) / 2;
    cmp = compare_fn(arr + (mid * d_size), key);
    if (cmp == 0)      
        return mid;         //key found
    if (cmp > 0)    // key is in left half
        return bin_srch(arr, d_size, s, mid - 1, key, compare_fn);
    return bin_srch(arr, d_size, mid + 1, e, key, compare_fn); //key is in right half
}
/* Inputs
 * array: pointer to data array
 * arr_size: no. of bytes in array
 * data_size: size of each element in bytes
 * key: pointer to key to be search. its size is assumed to be same as data_size
 * compare_fn: compare function for this data type
 *
 * Return
 * index of key in array if found(This is NOT byte index), -1 if not found, -2 if error
 */
int binary_search(void *array, int arr_size, int data_size, void *key, int (*compare_fn)(void*, void*))
{   
    int n = 0;
    if( array == NULL || arr_size <= 0 || data_size <=0 || compare_fn == NULL){
        return -2;
    }
    n = (arr_size / data_size);
    return bin_srch( array, data_size, 0, n - 1 , key, compare_fn);
}

int main(void)
{
    int a[] = {0, 4, 8, 9, 22};
    int key = 5;
    char b[] = { 'a', 'c', 'd', 'g', 'y', 'z'};
    char key1 = 'y';
    float f[] = {0.2f, 0.4f, 0.8f, 1.9f, 2.22f};
    float key2 = 1.9f;
    
    //printf("res = %d\n", binary_search(a, sizeof(a), sizeof(int), &key, &compare_int));
    //printf("res = %d\n", binary_search(b, sizeof(b), sizeof(char), &key1, &compare_char));
    //printf("res = %d\n", binary_search(f, sizeof(f), sizeof(float), &key2, &compare_float));
    float f1 = 6.0000000001;
    float f2 = 6.0000000002;
    compare_float(&f1, &f2);
    return 0;
}

