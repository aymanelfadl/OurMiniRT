#include "parser.h"

int array_len_elements(char **s)
{
    int i = 0;
    if (!s)
        return 0;
    while (s[i])
        i++;
    return i;
}

int array_len(char **s)
{
    int len = 0;
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
    {
        len += ft_strlen(s[i]);
        if (s[i + 1])
            len += 1;
        ++i;
    }
    return (len);
}

char *ft_strjoin_array(char const **arr, char const *sep)
{
    char   *out;
    size_t i;
    size_t pos;
    size_t sep_len;

    if (!arr || !sep)
        return (NULL);
    out = malloc(array_len((char **)arr) + 1);
    pos = 0;
    sep_len = ft_strlen(sep);
    i = 0;
    while (arr[i])
    {
        ft_memcpy(out + pos, arr[i], ft_strlen(arr[i]));
        pos += ft_strlen(arr[i]);
        if (arr[i + 1])
        {
            ft_memcpy(out + pos, sep, sep_len);
            pos += sep_len;
        }
        i++;
    }
    out[pos] = '\0';
    return (out);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void free_token(void *ptr)
{
    t_token *t = (t_token *)ptr;
    free(t->id);
    free(t->args);
    free(t);
}

void ft_free_split(char **tab)
{
    size_t i;

    if (!tab)
        return;
    i = 0;
    while (tab[i])
        free(tab[i++]);
    free(tab);
}

int ft_isspace(int c)
{
    return (c == ' '  || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

double ft_atof(const char *str)
{
    int sign = 1;
    double result = 0.0;
    double fraction = 0.0;
    double divisor = 10.0;

    while (*str && ft_isspace((unsigned char)*str))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (ft_isdigit((unsigned char)*str))
    {
        result = result * 10.0 + (*str - '0');
        str++;
    }
    if (*str == '.')
    {
        str++;
        while (ft_isdigit((unsigned char)*str))
        {
            fraction += (*str - '0') / divisor;
            divisor *= 10.0;
            str++;
        }
    }
    return sign * (result + fraction);
}
