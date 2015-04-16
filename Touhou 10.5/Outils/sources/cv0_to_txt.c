#include <stdio.h>
#include <stdlib.h>
#include <string.h>

wchar_t*		sjis_to_unicode(const char *src);
char*			unicode_to_utf8(const wchar_t* src);

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

wchar_t	change_char(wchar_t c)
{
  int	i;

  i = 0;
  while (tab[i] != L'\0')
    {
      if (tab[i] == c)
	return tab[i + 1];
      i += 2;
    }
  return c;
}

void	print_line(FILE* out, wchar_t* buff)
{
  wchar_t	new_buff[2048];
  char*	out_buff;
  int	i;
  int	j;

  i = 0;
  j = 0;
  while (buff[i])
    {
      if (buff[i] == L'¥' && buff[i + 1] == L',')
	i++;
      if (buff[i] == L'\n')
	{
	  if (new_buff[j - 1] == L'¥')
	    new_buff[j - 1] = L'\n';
	  else if (new_buff[j - 1] == L'@')
	    {
	      wcscpy(new_buff + j - 1, L"<break>");
	      j += 6;
	    }
	}
      new_buff[j] = change_char(buff[i]);
      // new_buff[j] = buff[i];
      i++;
      j++;
    }
  new_buff[j] = L'\0';
  out_buff = unicode_to_utf8(new_buff);
  fputs(out_buff, out);
  free(buff);
  free(out_buff);
}

int	main(int ac, char** av)
{
  FILE*	in;
  FILE*	out;
  char	out_filename[255];
  char	buff[2048];

  if (ac != 2)
    {
      printf("Usage: %s file\n", av[0]);
      return 0;
    }
  in = fopen(av[1], "r");
  if (in == NULL)
    {
      perror(av[1]);
      return 1;
    }
  strcpy(out_filename, av[1]);
  strcpy(strchr(out_filename, '.'), ".txt");
  out = fopen(out_filename, "w");
  if (out == NULL)
    {
      perror(out_filename);
      return 1;
    }
  fprintf(out, "\xEF\xBB\xBF");
  sjis_to_unicode(NULL);
  unicode_to_utf8(NULL);

  int	last_is_action = 0;
  while (fgets(buff, 2048, in) != NULL)
    {
      if (buff[0] == '#' || buff[0] == '\n' || buff[0] == '\0')
	continue ;
      if (strncmp(buff, "Face:", 5) == 0)
	{
	  *strchr(buff, ',') = '\0';
	  fprintf(out, "<face:%s>\n", buff + 5);
	  last_is_action = 0;
	}
      else if (strchr(buff, ':') != NULL)
	{
	  if (last_is_action == 0)
	    fprintf(out, "<action>\n");
	  last_is_action = 1;
	}
      else
	{
	  print_line(out, sjis_to_unicode(buff));
	  last_is_action = 0;
	}
    }
  fclose(in);
  fclose(out);
  return 0;
}
