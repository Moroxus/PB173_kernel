#ifndef MEMORY_ALLOCATOR
#define MEMORY_ALLOCATOR

struct FreeChunk {
  struct FreeChunk *next;
  struct FreeChunk *prev;
  unsigned int start;
  unsigned int end;
};

struct FreeList {
  struct FreeChunk *first;
  struct FreeChunk *last;
};

extern struct FreeList freeZones;

void removeChunkFromList(struct FreeChunk *chunk);

void moveChunk(struct FreeChunk *chunk);

struct FreeChunk *createChunk(unsigned int address);

void tryMergeChunk(struct FreeChunk *chunk);

void insertChunkIntoList(struct FreeChunk* chunk);

#endif // MEMORY_ALLOCATOR
