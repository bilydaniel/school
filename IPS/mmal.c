// xgajdo24
/**
 * Implementace My MALloc
 * Demonstracni priklad pro 1. ukol IPS/2018
 * Ales Smrcka
 */

#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool
#include <assert.h> // assert
#include <string.h>

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS 0x20
#endif

#ifndef arena_append
    static void arena_append(Arena *a);
#endif

#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
 *   ---+------+----------------------------+---
 *      |Header|DDD not_free DDDDD...free...|
 *   ---+------+-----------------+----------+---
 *             |-- Header.asize -|
 *             |-- Header.size -------------|
 */
typedef struct header Header;
struct header {

    /**
     * Pointer to the next header. Cyclic list. If there is no other block,
     * points to itself.
     */
    Header *next;

    /// size of the block
    size_t size;

    /**
     * Size of block in bytes allocated for program. asize=0 means the block 
     * is not used by a program.
     */
    size_t asize;
};

/**
 * The arena structure.
 *   /--- arena metadata
 *   |     /---- header of the first block
 *   v     v
 *   +-----+------+-----------------------------+
 *   |Arena|Header|.............................|
 *   +-----+------+-----------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
typedef struct arena Arena;
struct arena {

    /**
     * Pointer to the next arena. Single-linked list.
     */
    Arena *next;

    /// Arena size.
    size_t size;
};

#define PAGE_SIZE (128*1024)

#endif // NDEBUG

Arena *first_arena = NULL;

/**
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
    size = ((size + (PAGE_SIZE - 1))/PAGE_SIZE * PAGE_SIZE);
    return size;
}

/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 * @pre req_size > sizeof(Arena) + sizeof(Header)
 */

/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
Arena *arena_alloc(size_t req_size)
{
    assert(req_size > sizeof(Arena) + sizeof(Header));
    Arena *arena = mmap(NULL, allign_page(req_size+sizeof(struct header) + sizeof(struct arena)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//

    if(arena == MAP_FAILED)
        return NULL;
    
    *(Arena*)arena = (Arena){NULL, allign_page(req_size)};

    arena_append(arena);
    *(Header *) (&arena[1]) = (Header){(Header*)(&arena[1]),arena->size - sizeof(struct arena) - sizeof(struct header),0};

    Header *first_header = (Header*)(first_arena + 1);
    Header *tmp = first_header;
    while(tmp->next != first_header)
        tmp = tmp->next;    
    ((Header*)(&arena[1]))->next = first_header;
    tmp->next = ((Header*)(&arena[1]));

    return arena;
}

/**
 * Appends a new arena to the end of the arena list.
 * @param a     already allocated arena
 */
static
void arena_append(Arena *a)
{
    if(first_arena == NULL)
        first_arena = a;
    else
    {
        Arena *tmp_arena = first_arena;
        while(tmp_arena->next != NULL)
            tmp_arena = tmp_arena->next;
        
        tmp_arena->next = a;
        a->next = NULL;
    }
}

/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
 * @pre size > 0
 */
/**
 *   +-----+------+------------------------+----+
 *   | ... |Header|........................| ...|
 *   +-----+------+------------------------+----+
 *
 *                |-- Header.size ---------|
 */
static
void hdr_ctor(Header *hdr, size_t size)
{
    assert(size > 0);  
    *(Header *) hdr = (Header){NULL,size,0};

}

/**
 * Checks if the given free block should be split in two separate blocks.
 * @param hdr       header of the free block
 * @param size      requested size of data
 * @return true if the block should be split
 * @pre hdr->asize == 0
 * @pre size > 0
 */
static
bool hdr_should_split(Header *hdr, size_t size)
{
    assert(hdr->asize == 0);
    assert(size > 0);
    
    if(size > hdr->size)
        return false;
    else if (hdr->size > size + 2*sizeof(struct header))
        return true;

    return false;
}

/**
 * Splits one block in two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @return pointer to the new (right) block header.
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 */
/**
 * Before:        |---- hdr->size ---------|
 *
 *    -----+------+------------------------+----
 *         |Header|........................|
 *    -----+------+------------------------+----
 *            \----hdr->next---------------^
 */
/**
 * After:         |- req_size -|
 *
 *    -----+------+------------+------+----+----
 *     ... |Header|............|Header|....|
 *    -----+------+------------+------+----+----
 *             \---next--------^  \--next--^
 */
static
Header *hdr_split(Header *hdr, size_t req_size)
{
    assert((hdr->size >= req_size + 2*sizeof(Header)));
    int *tmp = ((void*)hdr + sizeof(Header) + req_size);
    hdr_ctor( (Header *) tmp, hdr->size - req_size - sizeof(struct header));
    ((Header *)tmp)->next = hdr->next;
    hdr->next = (Header *)tmp;
    hdr->size = req_size;
    return  hdr->next;

}

/**
 * Detect if two adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 * @pre left->next == right
 * @pre left != right
 */
static
bool hdr_can_merge(Header *left, Header *right)
{
    assert(left->next == right);
    assert(left != right);
    
    if(left->asize == 0 && right->asize == 0)
    {
        int *tmp = ((void*)left + sizeof(Header)+ left->size);
        if((Header*)tmp == right)
            return true;
    }

    return false;
}

/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 * @pre left->next == right
 * @pre left != right
 */
static
void hdr_merge(Header *left, Header *right)
{
    assert(left->next == right);
    assert(left != right);

    left->size += sizeof(struct header) + right->size;
    left->next = right->next;

}

/**
 * Finds the first free block that fits to the requested size.
 * @param size      requested size
 * @return pointer to the header of the block or NULL if no block is available.
 * @pre size > 0
 */
static
Header *first_fit(size_t size)
{
    assert(size > 0);
    Header *first_header = (Header *)(&first_arena[1]);
    Header *tmp_header = first_header;
    do
    {
        if(tmp_header->asize == 0 && tmp_header->size >= size)
            return tmp_header;
        tmp_header = tmp_header->next;
    }while(tmp_header != first_header);

    return NULL;
}

/**
 * Search the header which is the predecessor to the hdr. Note that if 
 * @param hdr       successor of the search header
 * @return pointer to predecessor, hdr if there is just one header.
 * @pre first_arena != NULL
 * @post predecessor->next == hdr
 */
static
Header *hdr_get_prev(Header *hdr)
{
    assert(first_arena != NULL);
    Header *tmp_header = (Header *)(&first_arena[1]);
    if(tmp_header->next != tmp_header)
    {
        while(tmp_header->next != hdr)
            tmp_header = tmp_header->next;
        return tmp_header;
    }

    return NULL;
}

/**
 * Allocate memory. Use first-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error or size = 0.
 */
void *mmalloc(size_t size)
{
    if (size != 0)
    {
        Header *allo = NULL;
        if(first_arena != NULL)
        {
            if ( (allo = first_fit(size)) != NULL)
            {
                if (hdr_should_split(allo, size))
                    hdr_split(allo, size);
                allo->asize = size;
                return (&allo[1]);
            }
            else
            {
                arena_alloc(allign_page(size+sizeof(struct arena) + sizeof(struct header)));
                if ( (allo = first_fit(size)) != NULL)
                {
                    if (hdr_should_split(allo, size))
                    {
                        hdr_split(allo, size);
                    }
                    allo->asize = size;
                    return (&allo[1]);
                }
                else
                    return NULL;
            }
        }
        else
        {
            arena_alloc(allign_page(size+sizeof(struct arena) + sizeof(struct header)));
            if ( (allo = first_fit(size)) != NULL)
            {

                if (hdr_should_split(allo, size))
                    hdr_split(allo, size);
                allo->asize = size;
                return (&allo[1]);
            }
            else
                return NULL;
        }
    }
    else
        return NULL;
}

/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 * @pre ptr != NULL
 */
void mfree(void *ptr)
{
    assert(ptr != NULL);
    Header *tmp = (Header *)(ptr-sizeof(struct header));
    tmp->asize = 0;
    if (hdr_can_merge(tmp,tmp->next))
        {
            hdr_merge(tmp,tmp->next);
        }
    if (hdr_can_merge(hdr_get_prev(tmp),tmp))
        hdr_merge(hdr_get_prev(tmp),tmp);

}

/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space or NULL if size equals to 0.
 */
void *mrealloc(void *ptr, size_t size)
{
    if (size != 0)
    {
        if (size == ((Header *)(ptr-sizeof(struct header)))->size)
           return ptr;
        void*tmp = mmalloc(size);
        if (tmp != NULL)
        {
           memcpy(tmp,ptr,size);
           mfree(ptr);
           return tmp;
        }
    }
    else
        mfree(ptr);
    return NULL;
}
