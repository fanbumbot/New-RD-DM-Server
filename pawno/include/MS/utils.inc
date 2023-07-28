#if defined mdg_utils_inc
	#endinput
#endif
#define mdg_utils_inc

#include <a_samp>

#define quickSort quick_sort

stock quick_sort(array[], start, end)//end is end of array commonly
{
	static temp;
    new i = start;
    new j = end-1;
    new pivot = array[(i + j) / 2];
    while(i <= j)
    {
        while(array[i] < pivot)	i++;
        while(array[j] > pivot)	j--;
        if(i <= j)
        {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }
    if(j > start)	quickSort(array, start, j);
    if(i < end)		quickSort(array, i, end+1);
}