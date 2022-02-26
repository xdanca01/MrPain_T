/**
 * Implementace My MALloc
 * Demonstracni priklad pro 1. ukol IPS/2019
 * Ales Smrcka
 */


#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool

#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
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

#define PAGE_SIZE 128*1024

#endif

Arena *first_arena = NULL;

/**
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
    size = size + sizeof(Header) + sizeof(Arena) + sizeof(Header *);
    long int PAGE = PAGE_SIZE, modulo = (long int)size % PAGE;
    if(modulo != 0)
    {
        size = size + PAGE - modulo;
    }
    return size;
}

/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 */

/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
void hdr_ctor();
Arena *arena_alloc(size_t req_size)
{
    Arena *poloha;
    req_size = allign_page(req_size);
    poloha = mmap(NULL, req_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(poloha == MAP_FAILED) return NULL;
    poloha->next = NULL;
    poloha->size = req_size;
    Header *head = (Header *)(&poloha[1]);
    if(first_arena == NULL) first_arena  = poloha;
    else
    {
        Arena **dalsi = &first_arena;
        while (*dalsi != NULL) dalsi = &(*dalsi)->next;
        *dalsi = poloha;
    }
    hdr_ctor(head,(req_size - sizeof(Arena) - sizeof(Header)));
    return poloha;
}

/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
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
    if(size < 1) return;
    if(hdr == NULL) return;
    hdr->size = size;
    hdr->asize = 0;
    hdr->next = (Header *)(&first_arena[1]);
}

/**
 * Splits one block into two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @pre   (req_size % PAGE_SIZE) = 0
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 * @return pointer to the new (right) block header.
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
    if((req_size + 2*sizeof(Header)) >= hdr->size) return NULL;
    int size = hdr->size - req_size - sizeof(Header);
    size = size - (size % sizeof(Header *));
    if(req_size % sizeof(Header) != 0) hdr->size = req_size - (req_size%sizeof(Header)) + sizeof(Header);
    else hdr->size = req_size;
    Header *dalsi = (Header *)((char *)(&hdr[1]) + hdr->size);
    Header *zaloha = hdr->next;
    hdr->next = dalsi;
    dalsi->size = size;
    dalsi->asize = 0;
    if (zaloha != NULL) dalsi->next = zaloha;
    else dalsi->next = (Header *)(&first_arena[1]);
    return dalsi;
}

/**
 * Detect if two blocks adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 */
static
bool hdr_can_merge(Header *left, Header *right)
{
    
    if(left->asize == 0 && right->asize == 0)
    {
        Arena *pomocna = first_arena;
        Header *test;
        if(pomocna != NULL) test =(Header *)(&pomocna[1]);
        while(pomocna != NULL)
        {
            if(test == right) return false;
            pomocna = pomocna->next;
            if(pomocna != NULL) test = (Header *)(&pomocna[1]);
        }
        return true;
    }
    return false;
}

/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 */
static
void hdr_merge(Header *left, Header *right)
{
    if(left == right) return;
    left->next = right->next;
    left->size = left->size + sizeof(Header) + right->size;
    right->next = NULL;
    right->size = 0;
}

/**
 * Allocate memory. Use best-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error.
 */
void *mmalloc(size_t size)
{
    if(size <= 0) return NULL;
    //pokud neni zadna arena vytvor arenu o velikosti PAGE_SIZE
    if(first_arena == NULL) first_arena = arena_alloc(size);
    if(first_arena == NULL) return NULL;
    Header *headr = (Header *)(&first_arena[1]), *konec = headr, *nejmensiH, *posledniH;
    //pomocna promenna pro uchovani nejmensi velikosti size headeru
    unsigned nejmensi = 0;
    //nejmensi vyhovujici nalezeny prazdny header
    do
    {
        if(headr->asize == 0)
        {
            if(headr->size > size)
            {
                if(nejmensi > headr->size || nejmensi == 0)
                {
                    nejmensi = headr->size;
                    nejmensiH = headr;
                }
            }
        }
        headr = headr->next;
        posledniH = headr;
    }
    while(headr != konec);
    if(nejmensi == 0)
    {
        Arena *dalsi = arena_alloc(size);
        if(dalsi == NULL) return NULL;
        nejmensiH = (Header *)(&dalsi[1]);
        nejmensi = nejmensiH->size;
        posledniH->next = nejmensiH;
    }
    if(nejmensi > (sizeof(Header) + size))
    {
        hdr_split(nejmensiH, size);
        //if (hdr_split(nejmensiH, size) == NULL) return;
    }
    nejmensiH->asize = size;
    return &nejmensiH[1];
}

/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 */
void mfree(void *ptr)
{
    if(ptr == NULL) return;
    Header *headr = (Header *)(&first_arena[1]), *konec = headr, *uvolnit = headr;
    while(&uvolnit[1] != ptr) uvolnit = uvolnit->next;
    uvolnit->asize = 0;
    do
    {
            while(hdr_can_merge(headr,headr->next) == true)
            {
                hdr_merge(headr,headr->next);
            }
            headr = headr->next;
    }
    while(headr != konec);
    if(hdr_can_merge(headr,headr->next) == true)
    {
        hdr_merge(headr,headr->next);
    }
    return;
}

/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space.
 */
void *mrealloc(void *ptr, size_t size)
{
    mfree(ptr);
    if(size == 0 || ptr == NULL)
    {
        return NULL;
    }
    return mmalloc(size);
}
