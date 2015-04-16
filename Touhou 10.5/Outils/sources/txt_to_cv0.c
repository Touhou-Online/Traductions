#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

char*			unicode_to_sjis(const wchar_t* src);

static const wchar_t	tab[] =
{
  L'あ', L'à',
  L'い', L'ç',
  L'う', L'è',
  L'え', L'é',
  L'お', L'ê',
  L'が', L'ù',
  L'\0'
};

wchar_t	reset_char(wchar_t c)
{
  int	i;

  i = 0;
  while (tab[i] != L'\0')
    {
      if (tab[i + 1] == c)
	return tab[i];
      i += 2;
    }
  return c;
}

char*	read_from_cv(FILE* fd, int reuse_last)
{
  static char	buff[2048] = "";

  if (buff[0] && reuse_last)
    return buff;
  if (fgets(buff, 2048, fd) == NULL)
    return NULL;
  return buff;
}

int	read_until__cmp(char* buff, char* txt)
{
  if (buff[0] == '\n')
    return 0;
  if (txt)
    {
      if (strncmp(buff, txt, strlen(txt)) == 0)
	return 0;
      else
	return 1;
    }
  else
    {
      if (buff[0] == '#')
	return 0;
      if (strchr(buff, ':') != NULL)
	return 0;
      else
	return 1;
    }
}

void	read_actions(FILE* in_cv, FILE* out)
{
  char*	buff;
  int	reuse_last;

  reuse_last = 1;
  while ((buff = read_from_cv(in_cv, reuse_last)) && (buff[0] == '\n' || buff[0] == '#' || strchr(buff, ':') == NULL))
    reuse_last = 0;
  do
    {
      if (buff[0] != '\n' && buff[0] != '#' && strchr(buff, ':') == NULL)
	return ;
      if (strncmp(buff, "Face:", 5) == 0)
	return ;
      fputs(buff, out);
    } while ((buff = read_from_cv(in_cv, 0)));
}

void	read_face(FILE* in_cv, FILE* out)
{
  char*	buff;
  int	reuse_last;

  reuse_last = 1;
  while ((buff = read_from_cv(in_cv, reuse_last)) && (buff[0] == '#' || strncmp(buff, "Face:", 5) != 0))
    reuse_last = 0;
  fputs(buff, out);
  read_from_cv(in_cv, 0); // Pour lire une nouvelle chaîne au prochain appel avec reuse_last=1
}

void	read_text(wchar_t* buff, FILE* out)
{
  wchar_t	new_buff[2048];
  char*		out_buff;
  int		i;
  int		j;

  i = 0;
  j = 0;
  while (buff[i])
    {
      if (buff[i] == L',')
	{
	  new_buff[j] = L'¥';
	  j++;
	}
      if (wcsncmp(buff + i, L"<break>", 7) == 0)
	{
	  new_buff[j] = L'@';
	  j++;
	  i += 7;
	}
      if (buff[i] == L'\n')
        buff[i] = L'\0';

      new_buff[j] = reset_char(buff[i]);
      i++;
      j++;
    }
  new_buff[j] = L'\0';
  out_buff = unicode_to_sjis(new_buff);
  fputs(out_buff, out);
  free(out_buff);
}

FILE*	move_and_open_cv(char* src, char* dst)
{
  FILE*	fd;

  if (errno != ENOENT)
    {
      perror(dst);
      return NULL;
    }
  if (rename(src, dst) != 0)
    {
      fprintf(stderr, "Could not rename %s to %s: %s\n", src, dst, strerror(errno));
      return NULL;
    }
  fd = fopen(dst, "r");
  if (fd == NULL)
    {
      perror(dst);
      return NULL;
    }
  return fd;
}

int		main(int ac, char** av)
{
  FILE*		in_cv;
  FILE*		in_txt;
  FILE*		out;
  char		in_cv_name[PATH_MAX];
  char		out_name[PATH_MAX];
  wchar_t	buff[2048];

  if (ac != 2 || strcmp(av[1] + strlen(av[1]) - 4, ".txt") != 0)
    {
      printf("Usage: %s file.txt\n", av[0]);
      return 0;
    }
  strcpy(out_name, av[1]);
  strcpy(out_name + strlen(out_name) - 4, ".cv0");
  strcpy(in_cv_name, av[1]);
  strcpy(in_cv_name + strlen(in_cv_name) - 4, "_orig.cv0");
  in_cv = fopen(in_cv_name, "r");
  if (in_cv == NULL && (in_cv = move_and_open_cv(out_name, in_cv_name)) == NULL)
    return 1;
  in_txt = fopen(av[1], "r, ccs=UNICODE");
  if (in_txt == NULL)
    {
      perror(av[1]);
      return 1;
    }
  out = fopen(out_name, "w");
  if (out == NULL)
    {
      perror(out_name);
      return 1;
    }
  unicode_to_sjis(NULL);

  int	last_is_text = 0;
  while (fgetws(buff, 2048, in_txt) != NULL)
    {
      if (last_is_text)
	{
	  if (buff[0] == L'\n')
	    fputc('\\', out);
	  fputc('\n', out);
	  last_is_text = 0;
	}
      if (wcsncmp(buff, L"<action>", 8) == 0)
	read_actions(in_cv, out);
      else if (wcsncmp(buff, L"<face:", 6) == 0)
	read_face(in_cv, out);
      else if (buff[0] && buff[0] != L'\n')
	{
	  read_text(buff, out);
	  last_is_text = 1;
	}
    }
  fclose(in_cv);
  fclose(in_txt);
  fclose(out);
  return 0;
}
