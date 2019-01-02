#ifndef ARRAY_LIST_H
# define ARRAY_LIST_H

typedef struct s_array_list{
    void        **arr;
    int         cap;
    int         len;
}               t_array_list;

# define ARRAY_LIST(funct) array_list_##funct

void    array_list_init(t_array_list **al);
void    array_list_push(t_array_list *al, void *data);
void    array_list_destroy(t_array_list **al, void (*destroyer)(void **));
#endif