/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 22:13:29 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/03 15:57:05 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <errno.h>
#include "bmp.h"

static void		paste_fileheader(t_bitmap *s, char *file_name)
{
	ft_printf("{green}Chargement de l'image %s:{eoc}\n", file_name);
	ft_printf("%s %c%c\n", "signature", s->fileheader.signature[0],
												s->fileheader.signature[1]);
	ft_printf("%s: %i\n", "filesize", s->fileheader.filesize);
	ft_printf("%s: %i\n", "offset", s->fileheader.fileoffset_to_pixelarray);
	ft_printf("%s: %i\n", "header_size", s->bitmapinfoheader.dibheadersize);
	ft_printf("%s: %i\n", "width", s->bitmapinfoheader.width);
	ft_printf("%s: %i\n", "height", s->bitmapinfoheader.height);
	ft_printf("%s: %i\n", "planes", s->bitmapinfoheader.planes);
	ft_printf("%s: %i\n", "bpp", s->bitmapinfoheader.bitsperpixel);
	ft_printf("%s: %i\n", "compression", s->bitmapinfoheader.compression);
	ft_printf("%s: %i\n", "imagesize", s->bitmapinfoheader.imagesize);
	ft_printf("%s: %i\n", "xpermeter", s->bitmapinfoheader.ypixelpermeter);
	ft_printf("%s: %i\n", "ypermeter", s->bitmapinfoheader.xpixelpermeter);
	ft_printf("%s: %i\n", "numcolorpal",
										s->bitmapinfoheader.numcolorspallette);
}

static void		fill_image(uint8_t *data, uint8_t *pixelbuffer, int width,
																int height)
{
	size_t	i;
	int		p;
	int		c;
	uint8_t *ptr;

	p = height - 1;
	ptr = pixelbuffer + (p * width * 3);
	c = 0;
	i = 0;
	while (p >= 0)
	{
		if ((i % 4) == 3)
			i++;
		data[i] = ptr[c++];
		if (c == (width * 3))
		{
			p--;
			ptr = pixelbuffer + (p * width * 3);
			c = 0;
		}
		i++;
	}
}

int				bmp_load(char *file_name, int *width, int *height, int **data)
{
	FILE			*file;
	char			*buff;
	struct stat		*stat;
	size_t			res;
	t_bitmap		*s;

	if (!(stat = (struct stat *)malloc(sizeof(struct stat))))
		exit(EXIT_FAILURE);
	if ((lstat(file_name, stat)) == -1 || (!(file = fopen(file_name, "rb"))))
	{
		ft_eprintf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (!(buff = (char *)malloc(stat->st_size)))
		exit(EXIT_FAILURE);
	res = fread(buff, stat->st_size, 1, file);
	s = (t_bitmap *)buff;
	paste_fileheader((t_bitmap *)buff, file_name);
	*width = s->bitmapinfoheader.width;
	*height = s->bitmapinfoheader.height;
	if (!(*data = (int *)ft_memalloc(sizeof(int) * (*width) * (*height))))
		return (0);
	fill_image((uint8_t *)*data, (uint8_t *)
			(buff + s->fileheader.fileoffset_to_pixelarray), *width, *height);
	return (1);
}
