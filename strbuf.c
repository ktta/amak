
struct strbuf {
   struct strbuf *prev,*next;
   char data[4076];
   int len;
};

static int sb_collect(strbuf_t *sb, char *dst) ;
static void sb_free(strbuf_t *sb) ;


void strbuf_printf(strbuf_t *sb,char *fmt,...) {
	char *raw;
	va_list ap;
	va_start(ap,fmt);
again:
	for(raw=fmt;raw[0];raw++)
		if (raw[0]=='%')
			break;
	if (raw!=fmt)
		strbuf_puts_l(sb, fmt, raw-fmt);
	if (raw[0]==0)
		goto ende;
	switch(raw[1]) {
		case 0: strbuf_puts(sb, "%"); goto ende; 
		case 'd': {
			char buf[40];
			int arg= va_arg(ap,int);
			sprintf(buf,"%d", arg);
			strbuf_puts(sb,buf);
			break; }
		case 's': {
			char* arg= va_arg(ap,char*);
			strbuf_puts(sb,arg);
			break; }
		default:
			strbuf_puts_l(sb, raw+1, 1);
			break;
	}
	fmt= raw+2;
	goto again;
ende:
	va_end(ap);
}

void strbuf_puts(strbuf_t *sb,char *str) {
	strbuf_puts_l(sb,str,strlen(str));
}

void strbuf_puts_l(strbuf_t *sb,char *str,int len) {
	while(len) {
		strbuf_t *t;
		int cp;
		t= sb->prev;
		cp= sizeof(t->data)-t->len;
		if (cp>len) cp= len;
		if (cp==0) {
			
	t= (typeof(t)) calloc(1, sizeof(t[0]));


			t->len= 0;
			 {
	(t)->prev= (sb)->prev; 
	(t)->next= (sb); 
	(sb)->prev->next= (t); 
	(sb)->prev= (t); 
}


			continue;
		}
		memcpy(t->data+t->len, str, cp);
		str+= cp;
		len-= cp;
		t->len+= cp;
	}
}

strbuf_t* strbuf_init() {
	strbuf_t *t;
	
	t= (typeof(t)) calloc(1, sizeof(t[0]));


	t->len= 0;
	t->prev= t->next= t;
	return t;
}


char *strbuf_collect(strbuf_t *sb) {
	int l;
	char *k;
	l= sb_collect(sb, 0);
	k= malloc( l + 1);
	sb_collect(sb, k);
	k[l]= 0;
	sb_free(sb);
	return k;
}


static  int sb_collect(strbuf_t *sb, char *dst) {
	strbuf_t *k;
	int L;
	k= sb;
	L= 0;
	do {
		if (dst) 
			memcpy(dst+L, k->data, k->len);
		L+= k->len;
		k= k->next;
	} while(k!=sb);
	return L;
}

static void sb_free(strbuf_t *sb) 
{
	while(sb->prev!=sb){ 
		strbuf_t *t= sb->prev;
		sb->prev= t->prev;
		free(t);
	}
	free(sb);
}

int strbuf_pos(strbuf_t *sb)
{
  int P;
  strbuf_t *K;
  P= 0;
  for(K=sb;;K=K->next)
  {
    P+= K->len;
    if (K->next==sb) break;
  }
  return P;
}

void strbuf_reset(strbuf_t *sb, int pos)
{
  strbuf_t *K,*Z,*U;

  if (sb->prev==sb) // only one block
  {
    if (sb->len>pos) sb->len= pos;
    return ; 
  }

  for(K=sb;;K=K->next)
  {
    if (K->len>=pos) break;
    pos -= K->len;
    if (K->next==sb) return ;
  }
    
  K->len= pos;
  if (K->len!=0) K= K->next; 
  for(Z=K;Z!=sb;Z=U)
  {
    U= Z->next;
    Z->prev->next= Z->next;
    Z->next->prev= Z->prev;
    free(Z); 
  }

}
