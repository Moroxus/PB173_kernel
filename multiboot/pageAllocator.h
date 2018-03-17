#ifndef PAGE_ALLOCATOR
#define PAGE_ALLOCATOR

#define PAGES ((64) * (1024))

void loadPageDirectory(unsigned int*);

void enablePaging();

extern unsigned int page_directory[1024] __attribute__((aligned(4096)));

extern unsigned int page_tables[64][1024] __attribute__ ((aligned(4096)));

extern unsigned int bitmap[PAGES/32];

extern unsigned int framePointer;

extern unsigned int pagePointer;

unsigned int allocatePage();

void freePage();

#endif // PAGE_ALLOCATOR
