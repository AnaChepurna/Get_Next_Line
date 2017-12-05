/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achepurn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 16:25:45 by achepurn          #+#    #+#             */
/*   Updated: 2017/11/24 20:43:39 by achepurn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int		ft_strclen(char *str, char c)
{
	int			i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static t_list	*get_current_file(t_list **alst, int fd)
{
	t_list	*current;

	current = *alst;
	while (current)
	{
		if (current->content_size == fd)
			return (current);
		current = current->next;
	}
	current  = ft_lstnew(ft_strnew(1), fd);
	return (current);
}

static int		get_next_line_perform(const int fd, char **line, char **content)
{
	char		buf[BUFF_SIZE + 1];
	char		*tmp;
	int			lnlen;
	int			ret;

	while (!ft_strchr(*content, '\n') && (ret = read(fd, buf, BUFF_SIZE)))
	{
		if (ret == -1)
			return (-1);
		buf[ret] = '\0';
		tmp = *content;
		*content = ft_strjoin(*content, buf);
		free(tmp);
	}
	if (!ret && !ft_strlen(*content))
		return (0);
	lnlen = ft_strclen(*content, '\n') + 1;
	*line = ft_strnew(lnlen);
	ft_strncpy(*line, *content, lnlen - 1);
	tmp = *content;
	*content = ft_strdup(*content + lnlen);
	free(tmp);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*file = NULL;
	t_list			*current_file;

	if (fd < 0 || !line)
		return (-1);
	current_file = get_current_file(&file, fd);
	return (get_next_line_perform(fd, line, (char **)&current_file->content));
}
