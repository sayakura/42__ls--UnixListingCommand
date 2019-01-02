#include "ft_ls.h"

char            *get_path(char *path, char *filename)
{

    char            *path_;
    char            *full_path;

    path_ = NULL;
    if (ft_strcmp(path, "/") != 0)
        path_ = ft_strjoin(path, "/");
    else
        path_ = ft_strdup("/");
    full_path = ft_strjoin(path_, filename);
    // if (path)
    //     free(path_);
    return (full_path);
}

struct stat     *get_stat(char *path, char *filename)
{
    struct stat     *sb;
    char            *full_path;

    full_path = NULL;
    if (filename != NULL)
        full_path = get_path(path, filename);
    else
        full_path = ft_strdup(path);
    sb = malloc(sizeof(struct stat));
    if (lstat(full_path, sb) == -1)
        ring_buffer_op(5, "stat: ", filename, ": ", sys_errlist[errno], "\n");
    // if (full_path)
    //     free(full_path);
    return (sb);
}

void        acl_handler(t_dpsb *dpsb)
{
    acl_t			acl;

    acl = NULL;
    if (listxattr(dpsb->name, NULL, 0, XATTR_NOFOLLOW) > 0)
        dpsb->extended = '@';
    else
        dpsb->extended = ' ';
}
void            display_error_message(char *path)
{
    static  char buffer[300];
    int          i;
    char         *pointer;

    i = ft_strlen(path);
    pointer = path;
    while (path[i] != '/' && i--)
        ;
    if (i > 0)
    {
        ft_strcpy(buffer, path + i + 1);
        pointer = buffer;
    }
    if (errno == 9)
        ring_buffer_op(5, "\e[1m\e[31mls: ", pointer, ": ", "Not a directory", "\e[0m\n"); 
    else
        ring_buffer_op(5, "\e[1m\e[31mls: ", pointer, ": ", sys_errlist[errno], "\e[0m\n");  
}

t_array_list    *ft_save_dir(t_flags *flag_info, char *path)
{
    DIR             *dir;
    struct dirent   *dp;
    t_array_list    *al;
    t_dpsb          *dpsb;
    char            *temp;

    (void )flag_info;
    ARRAY_LIST(init)(&al);
    if ((dir = opendir(path)) == NULL)
    {
        display_error_message((char *)path);
        return (al);
    }
    while ((dp = readdir(dir)) != NULL)
    {
        dpsb = malloc(sizeof(t_dpsb));
        dpsb->sb = get_stat(path, dp->d_name);
        dpsb->name = ft_strdup(dp->d_name);
        acl_handler(dpsb);
        if (S_ISLNK(dpsb->sb->st_mode))
        {
            temp = get_path(path, dpsb->name);
            dpsb->name = get_sym_name(&dpsb->name, temp);
            // free(temp);
        }
        ARRAY_LIST(push)(al, dpsb);
    }
    closedir(dir);
    return (al);
}

void    ft_recursive(t_flags *flag_info, char *path)
{
    t_array_list    *al;
    int             i;
    char            *full_path;

    i = 0;
    if (flag_info->header == 1)
        ring_buffer_op(2, path, ":\n");
    flag_info->header = 1;
    al = ft_save_dir(flag_info, path);
    dpsb_sort(al, flag_info, &dpsb_cmper);
    dpsb_print(al, flag_info);
    while (i < al->len && flag_info->R == 1)
    {
        if (((t_dpsb *)(al->arr[i]))->name[0] != '.'
        && S_ISDIR((((t_dpsb *)(al->arr[i]))->sb)->st_mode))
        {
            full_path = get_path(path, ((t_dpsb *)(al->arr[i]))->name);
            ring_buffer_op(1, "\n");
            ft_recursive(flag_info, full_path);
            // free(full_path);
        }
        i++;
    }
    ARRAY_LIST(destroy)(&al, &dpsb_destroyer);
}

char    *replace_tilde_with_path(char *path)
{
    extern char **environ;
    int         i;
    char        *real_path;
    
    i = 0;
    real_path = NULL;
    if (ft_strchr(path, '~'))
    {
        while (environ[i])
        {
            if (ft_strstr(environ[i], "HOME") == NULL)
                i++;
            else
            {
                real_path = ft_strdup(environ[i] + 5);
                break ;
            }
        }
        return (real_path == NULL ? ft_strdup(path) : real_path);
    }
    else
        return (real_path = ft_strdup(path));
}

void    recursive_dirs(t_array_list  *dirs, t_flags *flag_info)
{
    int         i;
    t_dpsb      *dpsb;
    t_array_list *dir_sorted;

    i = 0;
    ARRAY_LIST(init)(&dir_sorted);
    while (i < dirs->len)
    {
        dpsb = ft_memalloc(sizeof(t_dpsb));
        dpsb->name = ft_strdup(dirs->arr[i]);
        dpsb->sb = get_stat(dpsb->name, NULL);
        acl_handler(dpsb);
        ARRAY_LIST(push)(dir_sorted, dpsb);
        i++;
    }
    dpsb_sort(dir_sorted, flag_info, &dpsb_cmper);
    i = 0;
    while (i < dir_sorted->len)
    {
        ft_recursive(flag_info, ((t_dpsb *)dir_sorted->arr[i])->name);
        i++;
        if (i < dir_sorted->len)
            ring_buffer_op(1, "\n");
    }
    ARRAY_LIST(destroy)(&dirs, &ft_memdel);
    ARRAY_LIST(destroy)(&dir_sorted, &dpsb_destroyer);
}

void    ls_av_op( t_flags *flag_info, char **av)
{
    char        *path;
    t_dpsb          *dpsb;
    t_array_list    *files;
    t_array_list    *dirs;
    struct stat     sb;
    
    ARRAY_LIST(init)(&files);
    ARRAY_LIST(init)(&dirs);
    while (*av)
    {
        path = replace_tilde_with_path(*av);
        if (lstat(path, &sb) != -1)
        {
            if (!S_ISDIR(sb.st_mode))
            {
                dpsb = ft_memalloc(sizeof(t_dpsb));
                dpsb->name = path;
                dpsb->sb = get_stat(dpsb->name, NULL);
                acl_handler(dpsb);
                if (S_ISLNK(sb.st_mode))
                    dpsb->name = get_sym_name(&path, *av);
                ARRAY_LIST(push)(files, dpsb);
            }
            else 
                ARRAY_LIST(push)(dirs, path);
        }
        else
            display_error_message(path);
        av++;
    }
    if (files->len > 0)
    {
        dpsb_sort(files, flag_info, &dpsb_cmper);
        dpsb_print(files, flag_info);
        if (dirs->len > 0)
            ring_buffer_op(1, "\n");  
        flag_info->header = 1;
    }
    ARRAY_LIST(destroy)(&files, &dpsb_destroyer);
    if (dirs->len > 0)
        recursive_dirs(dirs, flag_info);
    else
        ARRAY_LIST(destroy)(&dirs, &ft_memdel);
}

int		main(int ac, char **av)
{
    t_flags         flag_info;
    int             status;
    t_array_list    *al;
    
    al = NULL;
    ft_bzero(&flag_info, sizeof(t_flags));
    if ((status = ft_save_flags_info(ac, &av, &flag_info)) == 0)
        exit(EXIT_FAILURE);
    else if (*av == NULL)
        ft_recursive(&flag_info, ".");
    else
        ls_av_op(&flag_info, av);
    write(1, g_ring_buffer, g_ring_buffer_pointer);
    exit(EXIT_SUCCESS);
}
