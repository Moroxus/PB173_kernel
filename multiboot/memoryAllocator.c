#include "memoryAllocator.h"
#include "pageAllocator.h"
#include "Common.h"

struct FreeList freeZones = {0,0};

void removeChunkFromList(struct FreeChunk *chunk) {
  if (chunk->prev != NULL) {
    chunk->prev->next = chunk->next;
  }
  if (chunk->next != NULL) {
    chunk->next->prev = chunk->prev;
  }
  if (freeZones.first == chunk) {
    freeZones.first = chunk->next;
  }
  if (freeZones.last == chunk) {
    freeZones.last = chunk->prev;
  }
}

void moveChunk(struct FreeChunk *chunk) {
  struct FreeChunk *moved = (struct FreeChunk *)chunk->start;
  *moved = *chunk;
  if (freeZones.first == chunk) {
    freeZones.first = moved;
  }
  if (freeZones.last == chunk) {
    freeZones.last = moved;
  }
  if (chunk->prev != NULL) {
    chunk->prev->next = moved;
  }
  if (chunk->next != NULL) {
    chunk->next->prev = moved;
  }
}

struct FreeChunk *createChunk(unsigned int address) {
  struct FreeChunk *chunk = (struct FreeChunk *)address;
  chunk->prev = NULL;
  chunk->next = NULL;
  chunk->start = address;
  chunk->end = address + 4095;
  return chunk;
}

void tryMergeChunk(struct FreeChunk *chunk) {
  if (chunk->next != NULL) {
    struct FreeChunk *following = chunk->next;
    if (chunk->end + 1 == following->start) {
      chunk->end = following->end;
      chunk->next = following->next;
      if (freeZones.last == following) {
        freeZones.last = chunk;
      }
    }
  }
  if (chunk->prev != NULL) {
    struct FreeChunk *previous = chunk->prev;
    if (previous->end + 1 == chunk->start) {
      previous->end = chunk->end;
      previous->next = chunk->next;
      if (freeZones.last == chunk) {
        freeZones.last = previous;
      }
    }
  }
}

void insertChunkIntoList(struct FreeChunk* chunk) {
  struct FreeChunk *following = freeZones.first;
  while (following != NULL && following->start < chunk->start) {
    following = following->next;
  }
  if (following != NULL) {
    struct FreeChunk *previous = following->prev;
    if (previous != NULL) {
      previous->next = chunk;
      chunk->prev = previous;
    } else {
      freeZones.first = chunk;
    }
    following->prev = chunk;
    chunk->next = following;
  } else {
    if (freeZones.last != NULL) {
      chunk->prev = freeZones.last;
      freeZones.last->next = chunk;
    } else {
      freeZones.first = chunk;
    }
    freeZones.last = chunk;
  }
  tryMergeChunk(chunk);
}

void *malloc(unsigned int size) {
  unsigned int sizeToAllocate = max(size + 4, sizeof(struct FreeChunk));
  struct FreeChunk *chunk = freeZones.first;
  while (chunk != NULL && chunk->end - chunk->start + 1 < sizeToAllocate) {
    chunk = chunk->next;
  }
  if (chunk == NULL) {
    unsigned int pageNeededNumber = (sizeToAllocate / 4096) + 1;
    for (unsigned int i = 0; i < pageNeededNumber; i++) {
      unsigned int ptr = allocatePage();
      chunk = createChunk(ptr);
      insertChunkIntoList(chunk);
    }
    chunk = freeZones.last;
  }

  unsigned int dataStart = chunk->start;
  unsigned int dataEnd = chunk->start + sizeToAllocate - 1;
  if (!(chunk->end - dataEnd > sizeof(struct FreeChunk))) {
    dataEnd = chunk->end;
    removeChunkFromList(chunk);
  } else {
    chunk->start = dataEnd + 1;
    moveChunk(chunk);
  }
  unsigned int *meta = (unsigned int*)dataStart;
  *meta = dataEnd - dataStart + 1;
  return (void *)(meta + 1);
}

void free(void *ptr) {
  unsigned int* start = ((unsigned int *)ptr);
  start--;
  unsigned int bytes = *start;
  struct FreeChunk *chunk = (struct FreeChunk *)start;
  chunk->start = (unsigned int)start;
  chunk->end = chunk->start + bytes - 1;
  insertChunkIntoList(chunk);
  unsigned int size = freeZones.last->end - freeZones.last->start + 1;
  while (size > 4096) {
    freeZones.last->end = (freeZones.last->end & ~0xfff) - 1;
    freePage();
    size = freeZones.last->end - freeZones.last->start + 1;
  }
}

void *realloc(void *ptr, unsigned int new_size) {
  unsigned int* ptrToSize = ptr;
  ptrToSize--;
  void *new_ptr = malloc(new_size);
  memcpy(new_ptr, ptr, *ptrToSize);
  free(ptr);
  return new_ptr;
}
