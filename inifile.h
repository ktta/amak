#ifndef INIFILE_H_INCLUDED_cinarus_at_yahoo_dom_20240214
#define INIFILE_H_INCLUDED_cinarus_at_yahoo_dom_20240214

/**
Copyright 2024 Sukru Cinar

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

/**
  INI file parser. This library is not thread-safe. Use locks if you
  are loading/unloading INI files from multiple threads.
 **/

/**
  In all functions, the inifile_t argument is optional. If you provide
  NULL for those arguments, the last used/opened INI file will be used.
  
  This feature is meant to be used with simple programs which use only
  one INI file. Please use non-NULL arguments if you're dealing with
  multiple INI files, behaviour in error cases might be confusing 
  otherwise.
 **/
typedef struct inifile inifile_t;



 /** 
   Creates a new inifile_t object. 
  **/
inifile_t *ini_new();

 /**
   Loads a file given by (fmt, ...). 
   Return value is non-zero, an errno value. You may retrieve the
   error string by calling ini_get_error() below.
  **/
int ini_load_file(inifile_t *F, const char *fmt, ...);

/**
  In the following functions, section is optional. if not given, a key
  without a section will be returned. the section name =root= (with the
  equal signs) is reserved for this purpose.

  In case a given section/key pair isn't found in the inifile, the 
  given default value is returned. The *n() function returns -1 instead.
 **/
int   ini_getintn(inifile_t *F, char *section, char *key); 
int   ini_getint (inifile_t *F, char *section, char *key, int defval); 

 /**
  The returned string is owned by the library. You need to make a copy
  if you want it to survive across a call to ini_free().
  **/
char *ini_getstr (inifile_t *F,char *section, char *key, char *defval);
char *ini_getstrn(inifile_t *F,char *section, char *key);

void       ini_free (inifile_t *F);

char      *ini_get_error(inifile_t *F);
int        ini_get_errno(inifile_t *F);

/**
  An INI file consists of assignments, possibly grouped into sections.
  It's a line-based format, the first character in a line determines
  the meaning of the line.

  Sections are specified using a line of the form:

  [section_name]

  the section name should be a valid C identifier, starting with a
  letter or underscore. Digits may appear later in addition to these.

  Assignments have the form:

  identifier = value

  The identifier has the same syntax as section names. Whitespaces 
  at the beginning and end of the value are trimmed off.  

  This is the basic assignment operator. There are 3 more.

  The += operator appends the value to the previous value of the variable.
  It doesn't trim the whitespaces at the beginning of the value but trims
  the ones at the end.

  The <= operator assigns a multi-line value marked by the given identifer.
  For example:

   init_code <= END_CODE
     print("init");
     font= init_fonts();
   END_CODE

  The marker has to be present at the beginning of the line, with no 
  whitespace leading it.

  The last operator is <+. This one is a combination of <= and +=. It 
  appends the given multi-line value to the given variable.

  Comments can be written using the '#' or ';' characters as the leading
  characters in a line. These characters have no special meaning in other
  locations.

  You may include this file as a header. Or you can  define INIFILE_IMPLEMENT
  before including in order to compile the code.

  Additionally, you may also define INIFILE_TEST in order to compile a test
  program. See the end of this file for details about how to run it.
 **/

#endif

#ifdef INIFILE_IMPLEMENT
#include <stdio.h> 
#include <stdint.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <setjmp.h>
#include <limits.h>
#include <stdarg.h>

typedef struct buffer buffer_t;

#define _BUFFER_PROTO  static

_BUFFER_PROTO buffer_t *buffer_new(size_t blksize);
_BUFFER_PROTO void buffer_free(buffer_t *B, void **Rd, size_t *Rl);
_BUFFER_PROTO void buffer_collect(buffer_t *B, void **Rd, size_t *Rl);
_BUFFER_PROTO void buffer_write(buffer_t *B, void *d, size_t l);

struct buffer_node { 
  struct buffer_node *next;
  size_t len,size,start;
  unsigned char data[1];
};

struct buffer {
  struct buffer_node *first, *last;
  size_t blksize;
};


_BUFFER_PROTO buffer_t *buffer_new(size_t blksize)
{
  buffer_t *B;
  B= malloc(sizeof(*B));
  B->first= B->last= NULL;
  B->blksize= blksize;
  return B;
}


_BUFFER_PROTO void buffer_free(buffer_t *B, void **Rd, size_t *Rl)
{
  struct buffer_node *P,*N;
  if (Rd || Rl) buffer_collect(B, Rd, Rl);
  for(P=B->first;P;P=N)
  {
    N= P->next;
    free(P);
  }
  free(B);
}

_BUFFER_PROTO void buffer_collect(buffer_t *B, void **Rd, size_t *Rl)
{
  size_t S;
  struct buffer_node *P;
  unsigned char *R;

  if (!Rd && !Rl) return ;

  S= 0;
  for(P=B->first;P;P=P->next) S+= P->len;

  if (S==0) { if (Rd) *Rd= NULL; if (Rl) *Rl= 0; return ; }
  if (!Rd) { *Rl= S; return ; }

  R= malloc(S);
  S= 0;
  for(P=B->first;P;P=P->next)
  {
    memcpy(R+S, P->data+P->start, P->len);
    S+= P->len;
  }

  *Rd= (void*) R;
  if (Rl) *Rl= S;
}

_BUFFER_PROTO void buffer_write(buffer_t *B, void *d, size_t l)
{
  struct buffer_node *P;
  size_t cp;

  while(l)
  {
    P= B->last;
    if (!P || (P->start+P->len)==P->size)
    {
      P= malloc(sizeof(*P)+B->blksize-1);
      P->len= 0;
      P->next= NULL;
      P->size= B->blksize;
      P->start= 0;
      if (!B->last) { B->last= B->first= P; }
      else { B->last->next= P; B->last= P; }
    }

    cp= P->size-P->len-P->start;
    if (cp>l) cp= l; 

    memcpy(P->data+P->len+P->start, d, cp);
    P->len+= cp;
    l-= cp;
    d= (unsigned char*) d + cp;
  }
}

typedef struct {
  void *root;
} strmap_t;

static strmap_t *strmap_new();
static void *strmap_find(strmap_t *map, const char *key);
static void *strmap_insert(strmap_t *map, char *key,void *data);
static void *strmap_remove(strmap_t *map, char *key);
static void *strmap_destroy(strmap_t *map);

typedef struct
{
  void* child[2];
  unsigned internal:1, byte:23, mask:8;
} strmap_node_t;


static inline int strmap_dir
    (uint8_t *key,uint32_t len_key,strmap_node_t *node)
{
  uint8_t c;

  if (node->byte<len_key) c= key[node->byte];
                     else c= 0;
  return (1+(node->mask|c))>>8;
}

static void* strmap_find(strmap_t *map,const char *skey)
{
  uint8_t *key; unsigned int len_key;
  strmap_node_t *node;

  node= map->root;
  if (!node) return 0;

  key= (void*) skey;
  len_key= strlen(skey);

  while(node->internal)
     node= node->child[strmap_dir(key,len_key,node)];

  if (strcmp(skey, node->child[0])) return 0;
                               else return node->child[1];
}


static inline uint32_t strmap_make_mask
    (uint32_t A, uint32_t B)
{
  uint8_t diff;
  diff= A ^ B;
  diff|= diff>>1;
  diff|= diff>>2;
  diff|= diff>>4;
  return (diff&~(diff>>1))^255;
}

static inline strmap_node_t* strmap_leaf_node
    (uint8_t *key, void *data)
{
  strmap_node_t *N;
  N= malloc(sizeof(*N));
  N->internal= 0;
  N->child[0]= key;
  N->child[1]= data;
  return N;
}

static inline strmap_node_t* strmap_internal_node
    (uint32_t byte, uint32_t mask)
{
  strmap_node_t *N;
  N= malloc(sizeof(*N));
  N->internal= 1;
  N->byte= byte;
  N->mask= mask;
  return N;
}

static void* strmap_insert(strmap_t* map, char* skey, void *data)
{
  uint8_t *key; uint32_t len_key;
  strmap_node_t *node_elt;

  key= (void*) skey;

  if (!map->root)
  {
     map->root= strmap_leaf_node(key,data);
     return 0;
  }

  len_key= strlen(skey);

  node_elt= map->root;
  while(node_elt->internal)
    node_elt= node_elt->child[strmap_dir(key,len_key,node_elt)];

  uint32_t crit_byte;
  uint8_t  mask;
  uint8_t  *elt;
  int dir_elt;
  strmap_node_t *node_key;

  elt= node_elt->child[0];

  for(crit_byte=0;
      crit_byte<=len_key && elt[crit_byte]==key[crit_byte];
      crit_byte++)
          ;

  if (crit_byte>len_key) return node_elt->child[1];

  mask= strmap_make_mask(elt[crit_byte],key[crit_byte]);

  dir_elt= (1+(mask|elt[crit_byte]))>>8;

  node_key= strmap_internal_node(crit_byte,mask);
  node_key->child[1-dir_elt]= strmap_leaf_node(key,data);

  strmap_node_t *parent;
  strmap_node_t *node;

  parent= 0;
  node= map->root;

  while(node->internal)
  {
     if (node->byte > crit_byte) break;
     if (node->byte==crit_byte && node->mask > mask) break;
     parent= node;
     node= node->child[strmap_dir(key,len_key,node)];
  }

  node_key->child[dir_elt]= node;
  if (parent) { if (parent->child[0]==node) parent->child[0]= node_key;
                                    else    parent->child[1]= node_key; }
  else map->root= node_key;
  return 0;
}

static void *strmap_remove(strmap_t* map, char *skey)
{
  uint8_t *key; uint32_t len_key;
  void *retv;

  if (!map->root) return 0;

  key= (void*) skey;
  len_key= strlen(skey);
   
  strmap_node_t *node;
  strmap_node_t *sibling;
  strmap_node_t *parent;
  strmap_node_t *gparent;

  node= map->root;
  gparent= 0;
  sibling= 0;
  parent= 0;

  while(node->internal)
  {
    int dir;
    dir= strmap_dir(key, len_key, node);
    gparent= parent;
    parent= node;
    sibling= node->child[1-dir];
    node= node->child[dir];
  }

  if (strcmp(node->child[0],skey)) return 0;
  retv= node->child[1];
  free(node);

  if (!gparent)
  {
     map->root= sibling;
     if (parent) free(parent);
     return retv;
  }

  if (gparent->child[0]==parent)
    gparent->child[0]= sibling;
  else
    gparent->child[1]= sibling;

  free(parent);

  return retv;
}

static void *strmap_destroy(strmap_t *map)
{
  strmap_node_t *node;
  strmap_node_t *parent;
  strmap_node_t *gparent;
  strmap_node_t *sibling;
  void *retv;

  if (!map->root) return 0;

  parent= 0;
  gparent= 0;
  sibling= 0;
  node= map->root;
  while(node->internal)
  {
    gparent= parent;
    parent= node;
    sibling= node->child[1];
    node= node->child[0];
  }
  retv= node->child[1];
  free(node);
  if (!gparent) 
  {
    map->root= sibling;
    if (parent) free(parent);
    return retv;
  }
  if (gparent->child[0]==parent)
    gparent->child[0]= sibling;
  else
    gparent->child[1]= sibling;

  free(parent);

  return retv;
}

static strmap_t  *strmap_new()
{
  strmap_t *s;
  s= malloc(sizeof(*s));
  s->root= 0;
  return s;
}

void * __strmap_unused__[]= 
{
  strmap_new, strmap_remove, strmap_destroy,
};

struct inivalue
{
  char *key;
  char *value;
};

struct inisection
{
  char *name;
  strmap_t values;
};

struct inifile
{
  char *file_name;
  struct inifile *next;
  strmap_t sections;
  char  error_text[2000];
  int   error_code;

  size_t line_size;
  char *line_data;
  int line_no;

  jmp_buf errout;
  FILE *input;
};

/***
   ===      ERROR HANDLING

 ***/

char      *ini_get_error(inifile_t *F)
{
  return F->error_text;
}

int        ini_get_errno(inifile_t *F)
{
  return F->error_code;
}


static void* ini_die_alloc(void *value, inifile_t *F)
{
  if (value) return value;
  F->error_code= errno;
  longjmp(F->errout, 1);
  return NULL; 
}

static void ini_perror(inifile_t *F, char *msg)
{
  if (F->file_name)
     snprintf(F->error_text, sizeof(F->error_text),
        "ini(%s:%d): %s", F->file_name, F->line_no, msg);
  else
     snprintf(F->error_text, sizeof(F->error_text),
        "ini(mem:%d): %s", F->line_no, msg);
  
  F->error_text[sizeof(F->error_text)-1]= 0;
  F->error_code= EINVAL;

  longjmp(F->errout, 1);
}

static void ini_serror(inifile_t *F, char *fmt, ...)
{
  va_list ap;
  int eL, av;
  F->error_code= errno;
  va_start(ap, fmt);
  vsnprintf(F->error_text, sizeof(F->error_text), fmt, ap);
  va_end(ap);
  F->error_text[sizeof(F->error_text)-1]= 0;
  eL= strlen(F->error_text);
  av= sizeof(F->error_text)-eL;
  snprintf(F->error_text+eL, av, ": %s", strerror(F->error_code));
  F->error_text[sizeof(F->error_text)-1]= 0;
  longjmp(F->errout, 1);
}

 /***
     ===          MEMORY  ALLOCATIONS
  ***/

static void* ini_calloc(inifile_t *F, size_t size)
{
  return ini_die_alloc(calloc(1,size), F);
}

static char *ini_strdup(inifile_t *F, const char *str)
{
  return ini_die_alloc(strdup(str ? str : ""), F);
}

static void* ini_malloc(inifile_t *F, size_t size)
{
  return ini_die_alloc(malloc(size), F);
}

static char *ini_cut(inifile_t *F, char *S, char *E)
{
  char *R;
  size_t L;
  L= E-S;
  R= ini_malloc(F, L+1);
  memcpy(R, S, L);
  R[L]= 0;
  return R;
}

static char *ini_append(inifile_t *F, char *A, char *B)
{
  char *R;
  int lA, lB;
  lA= strlen(A);
  lB= strlen(B);
  R= ini_malloc(F, lA+lB+1);
  memcpy(R, A, lA);
  memcpy(R+lA, B, lB);
  R[lA+lB]= 0;
  free(A);
  free(B);
  return R;
}


  /***
    ===       CHARACTER CLASS OPERATIONS
  ***/

static int ini_isletter(char *str)
{
  char p= *str;
  return ((p>='a' && p<='z') ||
          (p>='A' && p<='Z') ||
          (p=='_'))             ? 256 : p;
}

static int ini_isident(char *str)
{
  char p= *str;
  return ((p>='a' && p<='z') ||
          (p>='A' && p<='Z') ||
          (p=='_')           ||
          (p>='0' && p<='9'))   ? 256 : p;
}

static int ini_isspace(char *str)
{
  unsigned char p= *(unsigned char*) str;
  return ((p>0 && p<=' ') ||
          (p==127))             ? 256 : p;
}

static char *ini_trim_begin(char *start)
{
  while(ini_isspace(start)==256) start++;
  if (*start==';' || *start=='#')
    *start= 0;
  return start;
}

static char *ini_trim_end(char *start)
{
  char *end;
  for(end=start+strlen(start);end>start && ini_isspace(end-1)==256;*--end=0)
      ; 
  return end;
}

static const char *ini_section_name(const char *name)
{
  return (name && name[0]) ? name: "=root=";
}



static int ini_prepare_file_name
    (inifile_t *F, const char *fmt, va_list ap)
{
  if (!F->file_name) return 1;
  return 0;
}

static void ini_close(inifile_t *F)
{
  if (F->input)
  {
    fclose(F->input);
    F->input= NULL;
  }
  if (F->line_data) 
  {
    free(F->line_data);
    F->line_data= NULL; 
  }
  F->line_size= 0;
}


static void ini_free_all();


static int ini_getline(inifile_t *F)
{
  ssize_t line_len;
  char *r;
  line_len= getline(&F->line_data, &F->line_size, F->input);
  if (line_len<=0) 
  {
    if (!feof(F->input))
      ini_serror(F, "ini_load(read:%s):", F->file_name);
    return 0;
  }
  r= strchr(F->line_data, '\r');
  if (r) *r= 0;
  F->line_no++;
  return 1;
} 


static struct inisection *ini_alloc_section(inifile_t *F, char *name)
{
  struct inisection *section;
  section= strmap_find(&F->sections, name);
  if (section) return section;
  section= ini_calloc(F, sizeof(*section));
  section->name= name; 
  strmap_insert(&F->sections, section->name, section);
  return section;
}

static struct inisection* ini_make_section(inifile_t *F, char *line)
{
  char *end;
  struct inisection *section;

  line= ini_trim_begin(line+1);
  if (ini_isletter(line)!=256) ini_perror(F,"bad section name"); 

  for(end=line;ini_isident(end)==256;end++)
        ;

  section= ini_alloc_section(F, ini_cut(F, line,end));
 
  line= ini_trim_begin(end);
  if (line[0]!=']') ini_perror(F, "bad section spec");

  line= ini_trim_begin(line+1);
  if (line[0]!=0) ini_perror(F, "extra chars after section spec\n");

  return section;
}

static char *ini_read_block(inifile_t *F, char *marker)
{
  buffer_t *B;
  int L;
  char *rest;
  char *R;

  L= strlen(marker);
  B= buffer_new(1024);

  while(ini_getline(F))
  {
    if (!memcmp(F->line_data, marker, L)) 
    {
      rest= ini_trim_begin(F->line_data + L);
      if (rest[0]!=0) ini_perror(F, "extra characters after marker");
      break;
    }
    buffer_write(B, F->line_data, strlen(F->line_data));
  }
  buffer_write(B, "", 1);
  buffer_free(B, (void**) &R, NULL);
  return R;
}

static void ini_parse_value
   (inifile_t *F, struct inisection *section, char *line)
{
  char *key, *value, *end, *block;
  int flavor, append, trim_b;
  struct inivalue *V;

  for(end=line;ini_isident(end)==256;end++)
        ;
  key= ini_cut(F, line, end);
  line= ini_trim_begin(end);
/**
4 operators:
  =      normal assignment
  +=     appends the value
  <=     sets the value, but a long string
  <+     appends the value, but a long string
 **/
  trim_b= 1;
  switch(line[0])
  {
  case '=': flavor= '=';
            append= 0;
            break;
  case '<': if (line[1]!='+' && line[1]!='=') 
               ini_perror(F, "bad operator");
            flavor= '<';
            append= line[1] == '+';
            line++;
            break;
  case '+': if(line[1]!='=') ini_perror(F, "bad assignment");
            line++;
            append= 1; flavor= '='; trim_b= 0;
            break; 
  default:  free(key);
            ini_perror(F, "unknown operator"); 
  }

  line= trim_b ? ini_trim_begin(line+1) : line+1;
  end= ini_trim_end(line);
  value= ini_cut(F, line, end);

  if (flavor=='<')
  {
    block= ini_read_block(F, value);
    free(value);
    value= block;
  }
 
  V= strmap_find(&section->values, key);
  switch( (V!=NULL ? 2: 0) + (append ? 1 : 0))
  {
  case 0:
  case 1: V= ini_calloc(F, sizeof(*V));
          V->key= key;
          V->value= value;
          strmap_insert(&section->values, key, V);
          break;
  case 2: free(V->value);
          V->value= value;
          free(key);
          break;
  case 3: V->value= ini_append(F, V->value, value);
          free(key);
          break;
  }
  return ;

free_key:
  free(key);
}

inifile_t *ini_new()
{
  return calloc(1, sizeof(inifile_t));
}

int ini_load_file(inifile_t *F, const char *fmt, ...)
{
  va_list ap;
  struct inisection *section;
  char *line;
  char fnbuf[PATH_MAX+1];

  if (setjmp(F->errout)) goto errexit;

  memset(fnbuf, 0, sizeof(fnbuf));
  va_start(ap, fmt);
  vsnprintf(fnbuf, sizeof(fnbuf)-1, fmt, ap);
  va_end(ap);
  fnbuf[sizeof(fnbuf)-1]= 0;

  F->file_name= ini_strdup(F, fnbuf);

  F->input= fopen(F->file_name, "rb");
  if (!F->input) 
     ini_serror(F, "ini_load(open:%s)", F->file_name);

  section= ini_alloc_section(F, ini_strdup(F, ini_section_name("")));
 
  while (ini_getline(F))
  {
    line= ini_trim_begin(F->line_data);
    switch(ini_isletter(line))
    {
    case 0: continue;     // empty line
    case '[': section= ini_make_section(F, line); continue;
    case 256: ini_parse_value(F, section, line); continue;
    }
    ini_perror(F, "unrecognized lead-char");
  }

  ini_close(F);
  return 0;

errexit:
  ini_close(F);
  return 1;
}


static void ini_free_p(void **R) { if (*R) { free(*R); *R= NULL; }} 

static void ini_destroy(inifile_t *F)
{
  struct inisection *sec;
  struct inivalue *val;
  int i;
  ini_close(F);
  ini_free_p((void**) &F->file_name);
  while((sec=strmap_destroy(&F->sections)))
  {
    ini_free_p((void**) &sec->name);
    while((val=strmap_destroy(&sec->values)))
    {
      ini_free_p((void**) &val->key);
      ini_free_p((void**) &val->value);
      free(val);
    }
    free(sec);
  }
  free(F);
}


void       ini_free (inifile_t *F)
{
  if (!F) return ;
  ini_destroy(F);
}

static struct inivalue *ini_get(inifile_t *F, const char *section, char *key)
{
  struct inisection *S;
  if (F && ((S= strmap_find(&F->sections, ini_section_name(section)))))
     return strmap_find(&S->values, key);
  return NULL;
}

int ini_getint (inifile_t *F, char *section, char *key, int defval)
{
  struct inivalue *V= ini_get(F, section, key);
  return V ? atoi(V->value) : defval;
}

int ini_getintn (inifile_t *F, char *section, char *key)
{
  return ini_getint(F, section, key, -1);
}

char *ini_getstr (inifile_t *F, char *section, char *key, char *defval)
{
  struct inivalue *V= ini_get(F, section, key);
  return V ? V->value : defval;
}

char *ini_getstrn(inifile_t *F, char *section, char *key)
{
  return ini_getstr(F, section, key, NULL);
}

#endif


#ifdef INI_TEST 

//
// test arguments:
//
//  ./a.out filename [i|s][0..9]section key
//
//   i : integer
//   s : string
//
//   0..9 : default value
//          integer : (N+1)^2
//          string  : N + "A"
//
//   multiple queries OK.
//

int main(int argc, char **argv)
{
  inifile_t *F;
  int i;
  char *arg;
  int type, defval;
  int   defval_i[10]= {   1,   4,   9,  16,  25,  36,  49,  64,  81, 100 };
  char* defval_s[10]= { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };

  F= ini_new();

  if (ini_load_file(F, argv[1])) 
     exit(fprintf(stderr,"%s\n", ini_get_error(F)));
  for(i=2;i+1<argc && (arg=argv[i]);i+=2)
  {
    type= *arg++;
    defval= *arg++ - '0';

    switch(type)
    {
    case 'i': printf("<%s>=%d\n", argv[i+1],
                     ini_getint(F, arg, argv[i+1], defval_i[defval]));
              break;
    case 's': printf("<%s>={%s}\n", argv[i+1],
                     ini_getstr(F, arg, argv[i+1], 
                     defval_s[defval]));
              break;
    }
  }
  ini_free(F);
  return 0;
}

#endif
