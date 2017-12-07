/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achepurn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 16:25:45 by achepurn          #+#    #+#             */
/*   Updated: 2017/12/07 20:41:12 by achepurn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_list	*get_current_file(t_list **alst, int fd)
{
	t_list	*current;

	current = *alst;
	while (current)
	{
		if ((int)current->content_size == fd)
			return (current);
		current = current->next;
	}
	ft_lstadd(alst, ft_lstnew("", 1));
	(*alst)->content_size = fd;
	return (*alst);
}

static int		perform(const int fd, char **line, char **content, char *buf)
{
	char			*tmp;
	size_t			len;
	ssize_t			ret;
	char			*ln;

	while (!(ln = ft_strchr(*content, '\n')) &&
			(ret = read(fd, buf, BUFF_SIZE)))
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
	len = ft_strlen(*content);
	*line = ft_strnew(ln ? ln - *content : len);
	ft_strncpy(*line, *content, ln ? ln - *content : len);
	tmp = *content;
	*content = ft_strdup(ln ? ln + 1 : "");
	free(tmp);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*file;
	t_list			*current_file;
	char			*buf;
	int				status;

	if (fd < 0 || !line || BUFF_SIZE == 0 ||
			!(buf = (char*)malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (-1);
	current_file = get_current_file(&file, fd);
	status = perform(fd, line, (char **)&current_file->content, buf);
	free(buf);
	return (status);
}
