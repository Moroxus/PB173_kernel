#include "pageAllocator.h"

unsigned int page_directory[1024] __attribute__((aligned(4096))) = {0};

unsigned int page_tables[64][1024] __attribute__ ((aligned(4096))) = {{0}};

unsigned int bitmap[PAGES/32] = {0};

unsigned int framePointer = 0;

unsigned int pagePointer = 0;

unsigned int allocatePage() {
  unsigned int frameIndex = framePointer;
  unsigned int u = bitmap[frameIndex/32] & (1 << (frameIndex % 32));
  while (u != 0) {
    frameIndex = (frameIndex + 1) % PAGES;
    u = bitmap[frameIndex/32] & (1 << (frameIndex % 32));
  }
  framePointer = frameIndex;
  bitmap[framePointer/32] |= (1 << (framePointer % 32));
  page_tables[pagePointer/1024][pagePointer % 1024] = (framePointer * 0x1000) | 3;
  unsigned int address = ((pagePointer/1024) << 22) | ((pagePointer % 1024) << 12);
  framePointer = (framePointer + 1) % PAGES;
  pagePointer = (pagePointer + 1) % PAGES;
  return address;
}

void freePage() {
  if (pagePointer > 1024) {
    pagePointer--;
    page_tables[pagePointer/1024][pagePointer % 1024] &= 0xfffffffe;
    unsigned int frameIndex = (page_tables[pagePointer/1024][pagePointer % 1024] & 0xfffff000) / 0x1000;
    bitmap[frameIndex/32] &= ~(1 << (frameIndex % 32));
  }
}
