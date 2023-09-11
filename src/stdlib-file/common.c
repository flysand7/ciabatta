
FILE *stdin;
FILE *stdout;
FILE *stderr;

static Cia_Pool _page_allocator;
static Cia_Pool _file_pool;
static Cia_Mutex _g_pool_mutex;
