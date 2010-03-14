/*
 * convert_utf.c - UTF-8 to UTF-32 converter.
 *                 Based on M. Sameer UTF-32 <-> UTF-8 conversion code.
 */

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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "convert_utf.h"
#include "conetserv.h"

static int get_length(uint8_t c)
{
   int len;
   c >>= 3;
   // 6 => 0x7e
   // 5 => 0x3e
   if (c == 0x1e) {
      len = 4;
   } else {
      c >>= 1;
      if (c == 0xe) {
         len = 3;
      }
      else {
         c >>= 1;
         if (c == 0x6) {
            len = 2;
         } else {
            len = 1;
         }
      }
   }
   return len;
}

uint32_t *utf8_to_utf32(uint8_t *utf8, int len)
{
   uint8_t *p = utf8;
   uint32_t ch;
   uint32_t *result = (uint32_t *)malloc(sizeof(uint32_t) * len);
   uint32_t *r = result;
   int y,x = 0;
   int length;

   if (!result) {
      logmsg("Ran out of memory!");
   }

   while (*p) {

      length = get_length(*p);

      switch (length)
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
         printf("length: %i\n", length);
      }

      ++p;

      for (y = length; y > 1; y--) {
         ch <<= 6;
         ch |= (*p ^ 0x80);
         ++p;
      }

      x += length;
      *r = ch;
      r++;
   }

   *r = 0x0;

   return result;
}
