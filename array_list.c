#include "array_list.h"
#include "libft/libft.h"

void    array_list_init(t_array_list **al)
{
    *al = malloc(sizeof(t_array_list));
    (*al)->arr = malloc(sizeof(void*) * 2);
    ft_bzero((*al)->arr, sizeof(void*) * 2);
    (*al)->len = 0;
    (*al)->cap = 2;
}

void    array_list_push(t_array_list *al, void *data)
{
    void    **new_arr;
    int     i;

    i = 0;
    if (al->len + 1 < al->cap)
    {
        al->arr[al->len] = data;
        al->len++;
    }
    else
    {
        new_arr = malloc(sizeof(void*) * (al->cap * 2));
        ft_bzero(new_arr, sizeof(void*) * (al->cap * 2));
        while (i < al->cap)
        {
            new_arr[i] = al->arr[i];
            i++;
        }
        new_arr[al->len] = data;
        al->len++;
        al->cap = al->cap * 2;
        free(al->arr);
        al->arr = new_arr;
    }
}

void    array_list_destroy(t_array_list **al, void (*destroyer)(void **))
{
    int        i;

    i = 0;
    while (i < (*al)->len)
    {
        if ((*al)->arr[i])
        {
            if (destroyer)
                destroyer(&(*al)->arr[i]);
        }
        i++;
    }
    if ((*al)->arr)
        free((*al)->arr);
    free(*al);
}