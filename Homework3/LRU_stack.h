typedef struct {
	int num;
	sdt *next, *prev;
} sdt;

typedef struct {
	sdt *head, *tail;
} stack;

void stk_init (stack);
int victim (stack);
void reff (stack, int);