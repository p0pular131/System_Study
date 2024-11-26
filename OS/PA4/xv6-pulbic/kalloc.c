// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;

struct {
  struct spinlock lock;
  char* bitmap;
  int num_free_blocks;
} swap_space;

struct page pages[PHYSTOP/PGSIZE];
struct page *page_lru_head;
struct spinlock page_lock;
int num_free_pages;
int num_lru_pages;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  num_free_pages = 0;
  for (int i = 0; i < PHYSTOP/PGSIZE; i++) {
    pages[i].pgdir = 0;
    pages[i].vaddr = 0;
    pages[i].next = 0;
    pages[i].prev = 0;
  }
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;

  initlock(&swap_space.lock, "swap_space");
  swap_space.bitmap = kalloc();
  swap_space.num_free_blocks = BM_SZ;

  initlock(&page_lock, "page_lock");
  num_lru_pages = 0;
  page_lru_head = 0;

  memset(swap_space.bitmap, 0, PGSIZE);
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  if(kmem.use_lock)
    release(&kmem.lock);
  num_free_pages++;
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

try_again:
  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(!r) {
    if(swap_out()){
      goto try_again;
    }
    else {
      cprintf("kalloc : Out of memory\n");
      return 0;
    }
  }
  if(r)
    kmem.freelist = r->next;
  if(kmem.use_lock)
    release(&kmem.lock);
  num_free_pages--;
  return (char*)r;
}

int
get_block_num() 
{
  if (swap_space.num_free_blocks == 0) {
    cprintf("get_block_num : No free blocks\n");
    return -1;
  }

  acquire(&swap_space.lock);  

  for (int idx = 0; idx < BM_IDX; idx++) {
    char byte = swap_space.bitmap[idx];

    if (byte != (char)0xFF) {
      for (uint shift = 0; shift < 8; shift++) {
        if (!(byte & (1 << shift))) {
          int blkno = idx * 8 + shift; 
          swap_space.bitmap[idx] |= (1 << shift);  
          swap_space.num_free_blocks--; 
          release(&swap_space.lock);  
          return blkno;  
        }
      }
    }
  }

  release(&swap_space.lock);  
  return -1; 
}

void            
set_bitmap(int blkno) 
{
  acquire(&swap_space.lock);    
  swap_space.bitmap[blkno / 8] |= (1 << (blkno % 8));
  if (swap_space.num_free_blocks > 0) {
    swap_space.num_free_blocks--;
  }
  release(&swap_space.lock);
}

void
clear_bitmap(int blkno)
{
  acquire(&swap_space.lock);
  swap_space.num_free_blocks++;
  swap_space.bitmap[blkno / 8] &= ~(1 << (blkno % 8));
  release(&swap_space.lock);
}

void  
add_to_lru(pde_t* pgdir, char* pa, char* va) 
{
  uint page_idx = V2P(pa) >> 12;  

  acquire(&page_lock);  

  num_lru_pages++;
  num_free_pages--;

  pages[page_idx].pgdir = pgdir;
  pages[page_idx].vaddr = va;

  if (page_lru_head == 0) { // nonempty
      page_lru_head = &pages[page_idx];
      pages[page_idx].next = &pages[page_idx];
      pages[page_idx].prev = &pages[page_idx];
  } 
  else { // empty
      pages[page_idx].next = page_lru_head;            
      pages[page_idx].prev = page_lru_head->prev;      
      page_lru_head->prev->next = &pages[page_idx];      
      page_lru_head->prev = &pages[page_idx];            
      page_lru_head = &pages[page_idx];         
  }

  release(&page_lock);  
}

void
rm_from_lru(char* pa)
{
  uint page_idx = V2P(pa) >> 12; 

  acquire(&page_lock);

  // there is no page
  if (pages[page_idx].next == 0 || pages[page_idx].prev == 0) {
      release(&page_lock);
      return;
  }

  num_lru_pages--;
  num_free_pages++;

  if (num_lru_pages == 0) {
      page_lru_head = 0;
  } 
  else {
      if (page_lru_head == &pages[page_idx]) {
          page_lru_head = pages[page_idx].next; 
      }

      pages[page_idx].prev->next = pages[page_idx].next;
      pages[page_idx].next->prev = pages[page_idx].prev;
  }

  memset((void *)&pages[page_idx], 0, sizeof(struct page));

  release(&page_lock);
}

int
swap_out(void)
{
  if (page_lru_head == 0 || num_lru_pages == 0) {
    cprintf("swap_out : No pages to swap out\n");
    return 0;
  }

  release(&kmem.lock);  
  acquire(&page_lock);  

  struct page *victim = page_lru_head;  
  pte_t *pte = 0;

  while (1) {
    pte = walkpgdir(victim->pgdir, (void *)victim->vaddr, 0);

    if (*pte & PTE_A) {
      *pte &= ~PTE_A;  
      page_lru_head = victim->next;  
      victim = victim->next; 
    } 
    else if (!(*pte & PTE_U)) {
      rm_from_lru(P2V(PTE_ADDR(*pte)));
    } 
    else {
      break;
    }
  }

  release(&page_lock);  

  int blkno = get_block_num();
  if (blkno == -1) {
    cprintf("swap_out : No free blocks\n");
    return 0;
  }

  char *addr = P2V(PTE_ADDR(*pte));  

  swapwrite(addr, blkno);
  set_bitmap(blkno); 

  rm_from_lru(addr);

  *pte = (*pte & ~0xFFF) | (blkno << 12);  
  *pte &= ~PTE_P;  

  kfree(addr);  

  return 1;  
}
