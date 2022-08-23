
struct pfx(fmt_stream_t) typedef pfx(fmt_stream_t);
struct pfx(fmt_stream_t) {
    int w;
    void *ctx;
    int (*pfx(out_ctype))(void *ctx, ctype ch);
    int (*pfx(out_wchar))(void *ctx, wchar ch);
    int (*pfx(out_char)) (void *ctx, char  ch);
};

#define stream_out(s, ch)  if((s->w++, !s->out_ctype(s->ctx, ch))) return -1
#define stream_outc(s, ch) if((s->w++, !s->out_char (s->ctx, ch))) return -1
#define stream_outw(s, ch) if((s->w++, !s->out_wchar(s->ctx, ch))) return -1
