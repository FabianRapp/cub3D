/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 17:12:36 by frapp             #+#    #+#             */
/*   Updated: 2024/04/24 10:14:42 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);

typedef struct s_file	t_file;
typedef struct s_buffer	t_buffer;

struct s_file
{
	char	buffer[BUFFER_SIZE + 5];
	char	buffer_copy[BUFFER_SIZE + 5];
	char	*output;
	int		fd;
	int		buffer1_chars;
	int		output_size;
	t_file	*first_file;
	t_file	*next_file;
	int		cur_all_c;
	int		buffer1_i;
	int		return_now;
};

char	*cleanup(t_file *current_file, t_file *first_file);
t_file	*get_current_file(int fd, t_file *first_file);
int		my_str_cpy(char *dest, char *src, char *src2);
void	sort_buffer(t_file *current_file);
char	*append_buffer(t_file *current_file);
char	*reading(t_file	*current_file);
char	*shrink_out(int fd, t_file *first_file);
char	*get_next_line(int fd);

#endif