/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dsaripap <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/26 20:52:11 by dsaripap      #+#    #+#                 */
/*   Updated: 2019/05/05 20:58:09 by dsaripap      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"
#include "get_next_line.h"

static int		contains_newline(char *line, int bytes_read)
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
	if (bytes_read != BUFF_SIZE)
		return (len);
	return (-1);
}

static int		check_stored_start(char **stored, int *bytes_read)
{
	int			i;

	*bytes_read = BUFF_SIZE;
	if (*stored == 0)
	{
		*stored = ft_strnew(BUFF_SIZE);
		return (-1);
	}
	else
	{
		i = contains_newline(*stored, *bytes_read);
		return (i);
	}
}

static int		check_stored_end(char **stored, int i, char **temp)
{
	if ((*stored)[i] != '\0')
	{
		*temp = ft_strdup((const char *)&(*stored)[i + 1]);
		ft_strdel(stored);
		*stored = *temp;
		return (1);
	}
	else
	{
		ft_strdel(stored);
		return (0);
	}
}

static int		while_loop(int *bytes_read, char **stored, int fd, char **line)
{
	char		buf[BUFF_SIZE + 1];
	char		*temp;
	int			i;

	if (read(fd, buf, 0) < 0)
		return (-1);
	i = check_stored_start(stored, bytes_read);
	while (i == -1)
	{
		ft_bzero(buf, BUFF_SIZE + 1);
		*bytes_read = read(fd, buf, BUFF_SIZE);
		if (*bytes_read == -1)
			return (-1);
		ft_strreplace(stored, ft_strjoin(*stored, buf));
		i = contains_newline(*stored, *bytes_read);
	}
	*line = ft_strsub(*stored, 0, i);
	return (check_stored_end(stored, i, &temp));
}

int				get_next_line(const int fd, char **line)
{
	int			bytes_read;
	static char	*stored[10240];
	int			res;

	if (line == NULL || fd < 0)
		return (-1);
	bytes_read = BUFF_SIZE;
	res = while_loop(&bytes_read, &stored[fd], fd, line);
	if (res == -1)
		return (-1);
	else if (res == 0 && bytes_read == 0 && **line == '\0')
	{
		ft_strdel(line);
		return (0);
	}
	else
		return (1);
}
