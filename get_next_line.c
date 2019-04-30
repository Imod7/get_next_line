/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dsaripap <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/26 20:52:11 by dsaripap      #+#    #+#                 */
/*   Updated: 2019/04/30 19:53:29 by dsaripap      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "get_next_line.h"
#include <stdio.h>

int				contains_newline(char *line, int eof)
{
	int			i;
	int			len;

	len = ft_strlen(line);
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\n')
			return (i);
		i++;
	}
	if (eof != BUFF_SIZE)
		return (len);
	return (-1);
}

void			check_stored(char **stored, int *i, int *eof)
{
	if (*stored == 0)
	{
		*stored = ft_strnew(BUFF_SIZE);
		//ft_bzero(*stored, BUFF_SIZE + 1);
		*i = -1;
	}
	else
	{
		*eof = BUFF_SIZE;
		*i = contains_newline(*stored, *eof);
	}
}

int				get_next_line(const int fd, char **line)
{
	int			eof;
	int			i;
	char		buf[BUFF_SIZE + 1];
	static char *stored;
	char		*temp;

    //printf("      - stored='%s'\n", stored);
    //printf("      - i='%d'\n", i);
    //printf("      - len stored='%lu'\n", ft_strlen(stored));
    check_stored(&stored, &i, &eof);
	while (i == -1)
	{
		ft_bzero(buf, BUFF_SIZE + 1);
		eof = read(fd, buf, BUFF_SIZE);
		temp = ft_strjoin(stored, buf);
		free(stored);
		stored = temp;
		i = contains_newline(stored, eof);
        //printf("\n-----My Loop \n");
        //printf("     - buf = '%s'\n", buf);
        //printf("     - stored = '%s'\n", stored);
        //printf("     - i = '%d'\n", i);
	}
	*line = ft_strsub(stored, 0, i);
	//printf("PREVIOUS stored = '%c', next = '%c'", stored[i - 1], stored [i]);
	if (eof != 0)
	{
		//printf("EOF != 0 >> stored: \"%s\", i: %d\n", stored, i);
		if (stored[i] != '\0')
		{
			temp = ft_strdup((const char *)&stored[i + 1]);
			free(stored);
			stored = temp;
		}
		else
			return (0);
	}
	if ((eof != BUFF_SIZE && ft_strlen(stored) == 0) || (eof == 0))
	{
		free(temp);
		return (0);
	}
	else
		return (1);
}

