#include	<stdio.h>
#include	<stdlib.h>
#include	<stdint.h>
#include	<stdarg.h>
#include	<Windows.h>

#define MAX_NB_FILES	128
// #define	MAX_BUFF_SIZE	4096
#define	MAX_FN_SIZE	MAX_PATH

typedef struct	s_entry
{
  uint32_t	fn_size;
  char		filename[MAX_FN_SIZE]; // Note : doesn't end with a \0.
  uint32_t	x; // cuts on the left (it seems).
  uint32_t	y; // changes the y pos. When too big, it seems it cuts the picture.
  uint32_t	w; // width
  uint32_t	h; // height. Doesn't seem to be exactly in pixels (maybe it's in pixels, but minus the y field)
}		t_entry;

typedef struct	s_file
{
  uint32_t	magic;
  uint32_t	nb_files;
  t_entry	files[MAX_NB_FILES];
  // char		buff[MAX_BUFF_SIZE];
}		t_file;

t_file	file; // Global, because I want to be sure it won't overflow from the stack

int WINAPI	MessageBoxF(LPCTSTR format, ...)
{
  TCHAR		buff[4096];
  va_list	args;

  memset(buff, '\0', 4096 * sizeof(TCHAR));
  va_start(args, format);
  _vsnprintf(buff, 4095, format, args); // Note : doesn't work with unicode (this function hasn't a tchar variant)
  return (MessageBox(NULL, buff, NULL, MB_OK));
}

void	repack_pic(t_entry* entry, FILE* fd, char* dat_filename)
{
  char	path[MAX_PATH];

  fread(&entry->fn_size, 4, 1, fd);
  fread(entry->filename, entry->fn_size, 1, fd);
  entry->filename[entry->fn_size] = '\0';

  if (strchr(dat_filename, '\\') || strchr(dat_filename, '/'))
    path[0] = '\0';
  else
    strcpy(path, ".\\");
  strcat(path, dat_filename);
  *strrchr(path, '.') = '\0';
  strcat(path, "_");
  strcat(path, entry->filename);
  strcat(path, ".ini");

  entry->x = GetPrivateProfileInt("img", "x", 0, path);
  entry->y = GetPrivateProfileInt("img", "y", 0, path);
  entry->w = GetPrivateProfileInt("img", "w", 0, path);
  entry->h = GetPrivateProfileInt("img", "h", 0, path);

  fseek(fd, 0, SEEK_CUR);
  fwrite(&entry->x, 4, 1, fd);
  fwrite(&entry->y, 4, 1, fd);
  fwrite(&entry->w, 4, 1, fd);
  fwrite(&entry->h, 4, 1, fd);
  fseek(fd, 0, SEEK_CUR);
}

void		repack_file(char* filename)
{
  FILE* 	fd;
  unsigned int	i;

  fd = fopen(filename, "r+b");
  if (fd == NULL)
    {
      MessageBoxF("Echec de l'ouverture du fichier %s.", filename);
      return ;
    }
  fread(&file.magic, 4, 1, fd);
  if (file.magic != 4)
    {
      MessageBoxF("Le fichier %s n'est pas un fichier .dat valide.", filename);
      return ;
    }
  fread(&file.nb_files, 4, 1, fd);
  if (file.nb_files > MAX_NB_FILES)
    {
      MessageBoxF("Le fichier %s contient trop d'images.\nVeuillez contacter le developpeur de ce programme : brliron@hotmail.fr", filename);
      return ;
    }
  for (i = 0; i < file.nb_files; i++)
    repack_pic(&file.files[i], fd, filename);
  fclose(fd);
}

int	main(int ac, char **av)
{
  int	i;

  if (ac < 2)
    {
      MessageBox(NULL, "Pour extraire des fichiers, deposez-lez sur ce programme.", NULL, MB_OK);
      return 1;
    }
  for (i = 1; i < ac; i++)
    repack_file(av[i]);
  return 0;
}
