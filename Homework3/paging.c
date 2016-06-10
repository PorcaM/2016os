#include <string.h>

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

void pag_init(){
	tlb_hit = tlb_miss = pt_hit = pt_miss = 0;
	memset(pt, 0, sizeof(pt));
	memset(tlb, 0, sizeof(tlb));
	return;
}

int addr_tr(data_t page_addr){
	data_t	ret		= 0;
	int	temp, fn, pn, offset;
	pn				= page_addr.bytes.b2;
	offset			= page_addr.bytes.b1;
	printf("page#: %d, offset: %d\n", pn, offset);
	if ((temp = lookup_tlb (pn)) != -1){
		/*tlb hit*/
		fn = temp;
	}
	else if ((temp = lookup_pt (pn)) != -1){
		/*page table hit*/
		fn = temp;
		update_tlb (pn);
	}
	else{
		/*trap*/
		if((temp = empty_frame ()) == -1){
			/*No empty frame*/
			temp = get_victim ();
			fn = temp;
		}
		else{
			/*temp is empty frame number*/
			fn = temp;
		}
		update_ft (pn, fn);
		update_tlb (pn, fn);
		update_pt (pn, fn);
	}
	ret.bytes.b2 = fn;
	ret.bytes.b1 = offset;
	return ret;
}

byte lookup_tlb(byte pn){
	int		i;
	byte	ret = -1;
	bool	hit = false;
	for (i = 0; i < TLB_ENTRY_SIZE; i++){
		if (tlb[i].valid && tlb[i].page == pn){
			ret		= tlb[i].fn;
			flag	= true;
		}
	}
	if (hit)	tlb_hit++;
	else		tlb_miss++;
	return ret;
}

byte lookup_pt(byte pn){
	if (pt[pn].valid)
		return pt[pn].frame;
	else
		return -1;
}

int empty_frame(byte &ret){
	int i;
	for (i = 0; i < FRAME_ENTRY_SIZE; i++){
		if (!ft[i].filled){
			ret = i;
			return 0;
		}
	}
	return -1;	// no empty frame
}

int empty_tlb(int &ret){
	int i;
	for (i = 0; i < FRAME_ENTRY_SIZE; i++){
		if (!tlb[i].valid){
			ret = i;
			return 0;
		}
	}
	return -1;	// no empty tlb
}

void update_tlb(int pn, int fn){
	int it, temp;
	if ((temp = empty_tlb (it)) == -1){
		it = victim (tlb_stack);
	}
	tlb[it].page	= pn;
	tlb[it].frame	= fn;
	tlb[it].valid	= true;
	return;
}

void update_ft(int pn, int fn){
	int it, temp;
	if ((temp = empty_frame(it)) == -1){
		it = victim (ft_stack);
	}
	pt[pn].valid = false;
	ft[it].page = pn;
	return;
}

void update_pt(int pn, int fn){
	pt[pn].valid = true;
	pt[pn].frame = fn;
	return;
}
