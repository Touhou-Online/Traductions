#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined(WIN32) || defined(_WIN32)
# include "iconv.h"
# define CAST (const char**)
#else
# include <iconv.h>
# define CAST (char**)
#endif

wchar_t*		sjis_to_unicode(const char *src)
{
  static iconv_t	cd = NULL;
  size_t		n_src;
  wchar_t*		dest;
  wchar_t*		dest_cpy;
  size_t		n_dest;

  if (src == NULL)
    {
      if (cd == NULL)
	cd = iconv_open("UTF-16LE", "SHIFT-JIS");
      else
	{
	  iconv_close(cd);
	  cd = NULL;
	}
      return (NULL);
    }
  n_src = strlen(src) + 1;
  n_dest = n_src * 2;
  dest = malloc(n_dest);
  dest_cpy = dest;
  iconv(cd, CAST &src, &n_src, (char **)&dest, &n_dest);
  return (dest_cpy);
}

wchar_t*		utf8_to_unicode(const char *src)
{
  static iconv_t	cd = NULL;
  size_t		n_src;
  wchar_t*		dest;
  wchar_t*		dest_cpy;
  size_t		n_dest;

  if (src == NULL)
    {
      if (cd == NULL)
	cd = iconv_open("UTF-16LE", "UTF-8");
      else
	{
	  iconv_close(cd);
	  cd = NULL;
	}
      return (NULL);
    }
  n_src = strlen(src) + 1;
  n_dest = n_src * 2;
  dest = malloc(n_dest);
  dest_cpy = dest;
  iconv(cd, CAST &src, &n_src, (char **)&dest, &n_dest);
  return (dest_cpy);
}

char*			unicode_to_utf8(const wchar_t* src)
{
  static iconv_t	cd = NULL;
  size_t		n_src;
  char*			dest;
  char*			dest_cpy;
  size_t		n_dest;

  if (src == NULL)
    {
      if (cd == NULL)
	cd = iconv_open("UTF-8", "UTF-16LE");
      else
	{
	  iconv_close(cd);
	  cd = NULL;
	}
      return (NULL);
    }
  n_src = (wcslen(src) + 1) * 2;
  n_dest = n_src * 2;
  dest = malloc(n_dest);
  dest_cpy = dest;
  iconv(cd, CAST &src, &n_src, &dest, &n_dest);
  return (dest_cpy);
}

char*			unicode_to_sjis(const wchar_t* src)
{
  static iconv_t	cd = NULL;
  size_t		n_src;
  char*			dest;
  char*			dest_cpy;
  size_t		n_dest;

  if (src == NULL)
    {
      if (cd == NULL)
	cd = iconv_open("SHIFT-JIS", "UTF-16LE");
      else
	{
	  iconv_close(cd);
	  cd = NULL;
	}
      return (NULL);
    }
  n_src = (wcslen(src) + 1) * 2;
  n_dest = n_src * 2;
  dest = malloc(n_dest);
  dest_cpy = dest;
  iconv(cd, CAST &src, &n_src, &dest, &n_dest);
  return (dest_cpy);
}
