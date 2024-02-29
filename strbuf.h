
#ifndef _DOT__DOT__SLASH_INCLUDE_SLASH_SCAUX_SLASH_STRBUF_DOT_H_INCLUDED
#define _DOT__DOT__SLASH_INCLUDE_SLASH_SCAUX_SLASH_STRBUF_DOT_H_INCLUDED

typedef struct strbuf strbuf_t;
#ifdef __cplusplus
extern "C"
{
#endif
  void strbuf_printf (strbuf_t * sb, char *fmt, ...);
  void strbuf_puts (strbuf_t * sb, char *str);
  void strbuf_puts_l (strbuf_t * sb, char *str, int len);
  strbuf_t *strbuf_init ();
  char *strbuf_collect (strbuf_t * sb);

  int strbuf_pos(strbuf_t *sb);
  void strbuf_reset(strbuf_t *sb, int pos);
#ifdef __cplusplus
}
#endif

#endif
