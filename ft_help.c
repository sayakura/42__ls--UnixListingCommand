#include "ft_ls.h"

void    dpsb_destroyer(void **pointer)
{
    t_dpsb *dpsb;
    
    dpsb = (t_dpsb *)(*pointer);
    if (dpsb->name)
        free(dpsb->name);
    if (dpsb->sb)
        free(dpsb->sb);
    if (*pointer)
        free(*pointer);
}


int     dpsb_cmper(t_flags *flag_info, void *data, void *data2)
{
    t_dpsb  *dpsb;
    t_dpsb  *dpsb2;
    int     ret;

    dpsb = (t_dpsb *)data;
    dpsb2 = (t_dpsb *)data2;
    if (flag_info->t)
    {
        if (flag_info->u == 1)
            ret = (dpsb->sb)->st_atimespec.tv_sec - (dpsb2->sb)->st_atimespec.tv_sec;
        else
            ret = (dpsb->sb)->st_mtimespec.tv_sec - (dpsb2->sb)->st_mtimespec.tv_sec;
        if (ret != 0)
            return ((flag_info->r) ? ret : -ret);   
    }
    ret = (ft_strcmp(dpsb->name, dpsb2->name));
    return ((flag_info->r) ? -ret : ret);
}


void    dpsb_sort_g(t_dpsb **arr, int len, char *target, int to)
{
    void    *temp;
    int     i;
    int     index;

    i = -1;
    while (++i < len)
        if (ft_strcmp(arr[i]->name, target) == 0)
            index = i;
    temp = arr[index];
    while (index > to)
    {
        arr[index] = arr[index - 1];
        index--;
    }
    arr[to] = temp;
}
void quicksort(void **arr,int first ,int last, t_flags *flag_info, int (* cmp)(t_flags *, void *, void *)){
   int i, j, pivot;
   void *temp;

   if(first<last){
      pivot=first;
      i=first;
      j=last;
      while(i<j){
         while(cmp(flag_info, arr[i], arr[pivot]) <= 0
            &&i<last)
            i++;
         while(cmp(flag_info, arr[j], arr[pivot]) > 0)
            j--;
         if(i<j){
            temp = arr[i];
            arr[i]=arr[j];
            arr[j]=temp;
         }
      }
      temp=arr[pivot];
      arr[pivot]=arr[j];
      arr[j]=temp;
      quicksort(arr,first,j-1, flag_info, cmp);
      quicksort(arr,j+1,last, flag_info, cmp);
   }
}

void    dpsb_sort(t_array_list *al, t_flags *flag_info, int (* cmp)(t_flags *, void *, void *))
{
    int     i;

    i = 0;
    if (flag_info->f == 1)
    {
        dpsb_sort_g((t_dpsb **)al->arr, al->len, ".", 0);
        dpsb_sort_g((t_dpsb **)al->arr, al->len, "..", 1);
        return ;
    }
    quicksort(al->arr, 0, al->len - 1, flag_info, cmp); 
}
