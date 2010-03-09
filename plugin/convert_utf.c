/* unicode.c
 * Copyright (c) 2005 Mohammed Sameer.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * To compile: gcc -o unicode unicode.c
 * ./unicode ~/projects/katoob/misc/tests/utf
 */

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define ERR(x) {fprintf(stderr,"%s\n",x); exit(1);}

int validate(unsigned char *buff, int len);
void dump_unicode(unsigned char *buff, int len);
unsigned long *to_unicode(unsigned char *utf8, int len);
unsigned char *to_utf8(unsigned long *unicode);
void print_char(int pos, int len, unsigned long ch);
void dump_unicode_string(unsigned long *str);
int get_length(unsigned char ch);

int main(int argc, char *argv[])
{
  FILE *fp;
  struct stat buf;
  int x;
  unsigned char *buff;
  unsigned long *unicode = NULL;

  if (argc != 2)
    {
      ERR("Usage unicode <file>");
    }

  x = stat(argv[1], &buf);
  if (x != 0)
    {
      perror("stat");
      exit(1);
    }
  buff = (unsigned char *)malloc(buf.st_size +1);
  if (!buff)
    ERR("Not enough memory!");

  fp = fopen(argv[1], "r");
  if (!fp)
    {
      perror("fopen");
      exit(1);
    }
  x = fread(buff, 1, buf.st_size, fp);
  if (x != buf.st_size)
    {
      perror("fread");
      exit(1);
    }
  buff[x] = '\0';
  fclose(fp);
  if (validate(buff, x))
    {
      unicode = to_unicode(buff, x);
      dump_unicode_string(unicode);
      //      dump_unicode(buff, x);
    }
  else {
    ERR("Invalid UTF-8 strings.");
  }
  //  printf("%s",buff);
  free(buff);
  buff = to_utf8(unicode);
  if (unicode)
    free(unicode);
  printf("%s\n", buff);
  free(buff);
  return 0;
}

int validate(unsigned char *buff, int len)
{
  int x;
  for (x = 0; x < len; x++)
    {
      if (buff[x] > 0xfd)
{
  printf("Byte %i is invalid\n", x);
  return 0;
}
    }
  return 1;
}

int get_length(unsigned char ch)
{
  int l;
  unsigned char c = ch;
  c >>= 3;
  // 6 => 0x7e
  // 5 => 0x3e
  if (c == 0x1e)
    {
      l = 4;
    }
      else {
c >>= 1;
if (c == 0xe)
  {
    l = 3;
  }
else {
  c >>= 1;
  if (c == 0x6)
    {
      l = 2;
    }
  else {
    l = 1;
  }
}
      }
  return l;
}

unsigned long *to_unicode(unsigned char *utf8, int len)
{
  unsigned char *p = utf8;
  unsigned long ch;
  int x = 0;
  int l;
  unsigned long *result = (unsigned long *)malloc(sizeof(unsigned long)*len);
  unsigned long *r = result;
  if (!result)
    {
      ERR("Ran out of memory!");
    }
  while (*p)
    {
      l = get_length(*p);

      switch (l)
{
case 4:
  ch = (*p ^ 0xf0);
  break;
case 3:
  ch = (*p ^ 0xe0);
  break;
case 2:
  ch = (*p ^ 0xc0);
  break;
case 1:
  ch = *p;
  break;
default:
  printf("Len: %i\n", l);
}
      ++p;
      int y;
      for (y = l; y > 1; y--)
{
  ch <<= 6;
  ch |= (*p ^ 0x80);
  ++p;
}
      print_char(x,l,ch);
      x += l;
      *r = ch;
      r++;
    }
  *r = 0x0;
  return result;
}

unsigned char *to_utf8(unsigned long *unicode)
{
  unsigned char *utf8 = NULL;
  unsigned long *s = unicode;
  unsigned char *u;
  unsigned long ch;
  int x = 0;
  while (*s)
    {
      ++s;
      ++x;
    }
  if (x == 0)
    {
      return NULL;
    }
  utf8 = (unsigned char *)malloc(x*4);
  if (!utf8)
    ERR("Out of memory");

  s = unicode;
  u = utf8;
  x = 0;

  while (*s)
    {
      ch = *s;
      if (*s < 0x80)
{
  x = 1;
  *u = *s;
  u++;
}
      else if (*s < 0x800)
{
  x = 2;
  *u = 0xc0 | (ch >> 6);
  u++;
}
      else if (*s < 0x10000)
{
  x = 3;
  *u = 0xe0 | (ch >> 12);
  u++;
}
      else if (*s < 0x200000)
{
  x = 4;
  *u = 0xf0 | (ch >> 18);
  u++;
}
      if (x > 1)
{
  int y;
  for (y = x; y > 1; y--)
    {
      /*
      unsigned long mask = 0x3f << ((y-2)*6);
      *u = 0x80 | (ch & mask);
      */
      *u = 0x80 | (ch & (0x3f << ((y-2)*6)));
      ++u;
    }
}
      ++s;
    }
  return utf8;
}

void dump_unicode(unsigned char *buff, int len)
{
  unsigned long *result = to_unicode(buff, len);
  dump_unicode_string(result);
  free(result);
}

void dump_unicode_string(unsigned long *str)
{
  unsigned long *s = str;
  while (*s)
    {
      printf("%li %lx\n", *s, *s);
      s++;
    }
  printf("\n");
}

void print_char(int pos, int len, unsigned long ch)
{
  printf("Character: %i\tLength: %i\tUTF-32(hex): %lx\tUTF-32(dec): %li\n", pos, len, ch, ch);
}
