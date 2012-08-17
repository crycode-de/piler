/*
 * decoder.c, SJ
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "decoder.h"
#include "htmlentities.h"
#include "config.h"


static int b64[] = {

     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255,  62,  255, 255, 255,  63,
      52,  53,  54,  55,    56,  57,  58,  59,    60,  61, 255, 255,  255,   0, 255, 255,

     255,   0,   1,   2,     3,   4,   5,   6,     7,   8,   9,  10,   11,  12,  13,  14,
      15,  16,  17,  18,    19,  20,  21,  22,    23,  24,  25, 255,  255, 255, 255, 255,
     255,  26,  27,  28,    29,  30,  31,  32,    33,  34,  35,  36,   37,  38,  39,  40,
      41,  42,  43,  44,    45,  46,  47,  48,    49,  50,  51, 255,  255, 255, 255, 255,

     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,

     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255,
     255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 255, 255,  255, 255, 255, 255

};


static char hex_table[] = {
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  1,  2,  3,   4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,

   0, 10, 11, 12,  13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0, 10, 11, 12,  13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


static int compmi(const void *m1, const void *m2){
   struct mi *mi1 = (struct mi *) m1;
   struct mi *mi2 = (struct mi *) m2;
   return strcmp(mi1->entity, mi2->entity);
}


void sanitiseBase64(char *s){
   char *p1;

   if(s == NULL) return;

   for(; *s; s++){
      if(b64[(unsigned int)(*s & 0xFF)] == 255){
         for(p1 = s; p1[0] != '\0'; p1++)
            p1[0] = p1[1];
      }
   }
}


int decodeBase64(char *p){
   int i, j, n[4], k1, k2, len=0;
   char s[5], s2[3], puf[MAXBUFSIZE];

   if(strlen(p) < 4 || strlen(p) > MAXBUFSIZE/2)
      return 0;

   for(i=0; i<strlen(p); i++){
      memcpy(s, p+i, 4);
      s[4] = '\0';

      i += 3;

      /* safety check against abnormally long lines */

      if(len + 3 > sizeof(puf)-1) break;

      if(strlen(s) == 4){
         memset(s2, 0, 3);

         for(j=0; j<4; j++){
            k1 = s[j];
            n[j] = b64[k1];
         }

         k1 = n[0]; k1 = k1 << 2;
         k2 = n[1]; k2 = k2 >> 4;

         s2[0] = k1 | k2;

         k1 = (n[1] & 0x0F) << 4;
         k2 = n[2]; k2 = k2 >> 2;

         s2[1] = k1 | k2;

         k1 = n[2] << 6;
         k2 = n[3] >> 0;


         s2[2] = k1 | k2;

         memcpy(puf+len, s2, 3);

         len += 3;
      }

   }

   *(puf+len) = '\0';

   snprintf(p, MAXBUFSIZE-1, "%s", puf);

   return len;

}


void decodeQP(char *p){
   int i, k=0, a, b;
   char c;

   if(p == NULL) return;

   for(i=0; i<strlen((char*)p); i++){
      c = p[i];

      if(p[i] == '=' && isxdigit(p[i+1]) && isxdigit(p[i+2])){
         a = p[i+1];
         b = p[i+2];

         c = 16 * hex_table[a] + hex_table[b];

         i += 2;
      }

      p[k] = c;
      k++;
   }

   p[k] = '\0';
}


void decodeHTML(char *p){
   unsigned char buf[MAXBUFSIZE], __u[8];
   char *s, *q;
   int count=0, len, c;
   struct mi key, *res;

   if(p == NULL || strlen(p) == 0) return;

   s = p;

   memset(buf, 0, sizeof(buf));

   for(; *s; s++){
      if(*s == '&'){
         q = strchr(s, ';');
         if(q){
            *q = '\0';

            if(*(s+1) == '#'){
               c = atoi(s+2);
               if(c == 0) c = 'q';

               buf[count] = (unsigned char)c;
               count++;
            }
            else {
               key.entity = s;
               res = bsearch(&key, htmlentities, NUM_OF_HTML_ENTITIES, sizeof(struct mi), compmi);

               if(res && res->val <= 255){
                  utf8_encode_char(res->val, &__u[0], sizeof(__u), &len);
                  memcpy(&buf[count], &__u[0], len);
                  count += len;
               }
               else {
                  buf[count] = 'q';
                  count++;
               }
            }

            s = q;
         }

      }
      else {
         buf[count] = *s;
         count++;
      }
   }

   buf[count] = '\0'; count++;

   memcpy(p, buf, count);
}


void decodeURL(char *p){
   int i, c, k=0, a, b;

   if(p == NULL) return;

   for(i=0; i<strlen(p); i++){
      switch(p[i]){
         case '+':
            c = ' ';
            break;

         case '%':
            if(isxdigit(p[i+1]) && isxdigit(p[i+2])){
               a = p[i+1];
               b = p[i+2];

               c = 16 * hex_table[a] + hex_table[b];

               i += 2;
            }
            else
               c = p[i];

            break;

        default:
            c = p[i];
            break;
      }

      p[k] = c;
      k++;

   }

   p[k] = '\0';
}


inline void utf8_encode_char(unsigned char c, unsigned char *buf, int buflen, int *len){
   int count=0;

   memset(buf, 0, buflen);

      /*
       * Code point          1st byte    2nd byte    3rd byte    4th byte
       * ----------          --------    --------    --------    --------
       * U+0000..U+007F      00..7F
       * U+0080..U+07FF      C2..DF      80..BF
       * U+0800..U+0FFF      E0          A0..BF      80..BF
       */

      if(c <= 0x7F){
         *(buf+count) = c;
         count++;
      }

      else if(c <= 0x7FF){
         *(buf+count) = ( 0xC0 | (c >> 6) );
         count++;
         *(buf+count) = ( 0x80 | (c & 0x3F) );
         count++;
      }


      else if (c <= 0xFFFF){
         *(buf+count) = ( 0xE0 | (c >> 12) );
         count++;
         *(buf+count) = ( 0x80 | ((c >> 6) & 0x3F) );
         count++;
         *(buf+count) = ( 0x80 | (c & 0x3F) );
         count++;
      }

   *len = count;
}


void utf8_encode(unsigned char *p){
   int count=0, len;
   unsigned char *u, *s, utf8[MAXBUFSIZE], __u[8];

   if(p == NULL || strlen((char *)p) == 0) return;

   memset(utf8, 0, MAXBUFSIZE);
   u = &utf8[0];
   s = p;

   for(; *s; s++){

      utf8_encode_char(*s, &__u[0], sizeof(__u), &len);

      /*
       * this condition should never happen, as according to the RFCs:
       *
       * "Each line of characters MUST be no more than 998 characters, and
       * SHOULD be no more than 78 characters, excluding the CRLF."
       *
       */

      if(count+len > sizeof(utf8)-1) break;

      //printf("%s", __u);
      memcpy(u+count, &__u[0], len);

      count += len;
   }

   *(u+count) = '\0'; count++;
   memcpy(p, u, count);
}

