#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "paging.h"
#include "LRU_stack.h"

#define PHYSICAL_SPACE_SIZE	32768
#define PAGE_SIZE			256
#define FRAME_SIZE			256
#define PAGE_ENTRY_SIZE		256
#define FRAME_ENTRY_SIZE	128
#define TLB_ENTRY_SIZE		16

typedef unsigned char	byte;
typedef unsigned short	u16;
typedef unsigned int	cnt_val;

typedef struct {
	byte	b1, b2, b3, b4;
} byte_struct;

typedef union {
	byte_struct bytes;
	int		whole;
} data_t;

typedef struct {
	int		fn;
	bool	valid;
} pt_attr;

typedef struct {
	int		pn;
	bool	filled;
} ft_attr;

typedef struct {
	int		pn, fn;
	bool	valid;
} tlb_attr;

cnt_val		tlb_hit, tlb_miss, pt_hit, pt_miss;
pt_attr		pt[PAGE_ENTRY_SIZE];
ft_attr		ft[FRAME_ENTRY_SIZE];
tlb_attr	tlb[TLB_ENTRY_SIZE];

stack 		tlb_stack, ft_stack;

int		lookup_pt (int, int*);
int		lookup_tlb (int, int*);
int		empty_tlb (int*);
int		empty_frame (int*);
void	update_tlb (int, int);
void	update_ft (int, int*);
void	update_pt (int, int);

void pag_init(){
	tlb_hit = tlb_miss = pt_hit = pt_miss = 0;
	stk_init (&tlb_stack);
	stk_init (&ft_stack);
	memset(pt, 0, sizeof(pt));
	memset(tlb, 0, sizeof(tlb));
	return;
}

int addr_tr(int page_addr){
	data_t	ret; 
	ret.whole = page_addr;
	int	temp, fn, pn, offset;
	printf ("===================================\n");
	pn				= ret.bytes.b2;
	offset			= ret.bytes.b1;
	printf("page#: %d, offset: %d\n", pn, offset);
	if ((temp = lookup_tlb (pn, &fn)) != -1){
		/*tlb hit*/
		printf("CASE: TLB HIT\n");	
	}
	else if ((temp = lookup_pt (pn, &fn)) != -1){
		/*page table hit*/
		update_tlb (pn, fn);
		printf("CASE: PAGE TABLE HIT\n");
	}
	else{
		/*trap*/
		printf("CASE: PAGE FAULT\n");
		update_ft (pn, &fn);
		update_tlb (pn, fn);
		update_pt (pn, fn);
	}
	ret.bytes.b2 = fn;
	ret.bytes.b1 = offset;
	return ret.whole;
}

int lookup_tlb(int pn, int *ret){
	int		i;
	bool	hit = false;
	for (i = 0; i < TLB_ENTRY_SIZE; i++){
		if (tlb[i].valid && tlb[i].pn == pn){
			*ret	= tlb[i].fn;
			hit		= true;
		}
	}
	if (hit){
		reff (&tlb_stack, i);
		tlb_hit++;
		return 0;
	}
	tlb_miss++;
	return -1;
}

int lookup_pt(int pn, int *ret){
	if (pt[pn].valid){
		*ret = pt[pn].fn;
		reff (&ft_stack, *ret);
		pt_hit++;
		return 0;
	}
	pt_miss++;
	return -1;
}

int empty_frame(int *ret){
	int i;
	printf ("\tFUNC: EMPTY_FRAME called\n");
	for (i = 0; i < FRAME_ENTRY_SIZE; i++){
		if (!ft[i].filled){
			*ret = i;
			ft[i].filled = true;
			printf ("\tFUNC: EMPTY_FRAME return 0\n");
			return 0;
		}
	}
	printf("\tFUNC: EMPTY_FRAME return -1\n");
	return -1;	// no empty frame
}

int empty_tlb(int *ret){
	int i;
	for (i = 0; i < FRAME_ENTRY_SIZE; i++){
		if (!tlb[i].valid){
			*ret = i;
			return 0;
		}
	}
	return -1;	// no empty tlb
}

void update_tlb(int pn, int fn){
	int it, temp;
	printf ("\tFUNC: UPDATE_TLB called\n");
	if ((temp = empty_tlb (&it)) == -1){
		it = victim (&tlb_stack);
	}
	tlb[it].pn		= pn;
	tlb[it].fn		= fn;
	tlb[it].valid	= true;
	reff (&tlb_stack, it);
	printf ("tlb[%d] = (%d, %d)\n", it, pn, fn);
	printf ("\tFUNC: UPDATE_TLB return\n");
	return;
}

void update_ft(int pn, int *ret){
	printf ("\tFUNC: UPDATE_FT called\n");
	printf("pn: %d\n", pn);
	int it, temp;
	if ((temp = empty_frame(&it)) == -1){
		it = victim (&ft_stack);
		printf("it: %d\n", it);
	}
	printf ("it: %d\n", it);
	pt[pn].valid = false;
	ft[it].pn = pn;
	reff (&ft_stack, it);
	*ret = it;
	printf ("ft[%d] = (%d)\n", it, pn);
	printf ("\tFUNC: UPDATE_FT return\n");
	return;
}

void update_pt(int pn, int fn){
	pt[pn].valid = true;
	pt[pn].fn = fn;
	printf ("pn[%d] = (%d)\n", pn, fn);
	return;
}

void tlb_hr(){
	printf("Whole: %d, hit: %d\n", tlb_hit+tlb_miss, tlb_hit);
}
void lru_hr(){
	printf("Whole: %d, hit: %d\n", pt_hit+pt_miss, pt_hit);
}
