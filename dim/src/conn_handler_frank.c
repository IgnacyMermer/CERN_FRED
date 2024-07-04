/*
 * DNA (Delphi Network Access) implements the network layer for the DIM
 * (Delphi Information Managment) System.
 *
 * Started date   : 10-11-91
 * Written by     : C. Gaspar
 * UNIX adjustment: G.C. Ballintijn
 *
 */

 /* This module can only handle one type of array, and DIC or DNS.
  * It cannot handle both simultaniously. It handles at same time
  * the NET and DNA array's. Although these have to be explicitly
  * created.
  */

#define DIMLIB
#include <dim.h>

static SRC_TYPES My_type;		/* Var. indicating type DIC or DIS */

#ifdef VMS
DIM_NOSHARE DNS_CONNECTION* Dns_conns;
DIM_NOSHARE DIC_CONNECTION* Dic_conns;
DIM_NOSHARE DNA_CONNECTION* Dna_conns;
DIM_NOSHARE NET_CONNECTION* Net_conns;
DIM_NOSHARE int Curr_N_Conns;
#else
DllExp DIM_NOSHARE DNS_CONNECTION* Dns_conns = 0;
DIM_NOSHARE DIC_CONNECTION* Dic_conns = 0;
DllExp DIM_NOSHARE DNA_CONNECTION* Dna_conns = 0;
DllExp DIM_NOSHARE NET_CONNECTION* Net_conns = 0;
DllExp DIM_NOSHARE int Curr_N_Conns = 0;
#endif

typedef struct id_item
{
	int arrId;
	void* ptr;
	SRC_TYPES type;
}ID_ITEM;

static ID_ITEM* Id_arr;
/*
static void **Id_arr;
*/
static int Curr_N_Ids = 0;
static int Curr_id = 0;

void conn_arr_create(SRC_TYPES type)
{

	if (Curr_N_Conns == 0)
		Curr_N_Conns = CONN_BLOCK;

	switch (type)
	{
	case SRC_DIC:
		Dic_conns = (DIC_CONNECTION*)
			calloc((size_t)Curr_N_Conns, sizeof(DIC_CONNECTION));
		My_type = type;
		break;
	case SRC_DNS:
		Dns_conns = (DNS_CONNECTION*)
			calloc((size_t)Curr_N_Conns, sizeof(DNS_CONNECTION));
		My_type = type;
		break;
	case SRC_DNA:
		Dna_conns = (DNA_CONNECTION*)
			calloc((size_t)Curr_N_Conns, sizeof(DNA_CONNECTION));
		Net_conns = (NET_CONNECTION*)
			calloc((size_t)Curr_N_Conns, sizeof(NET_CONNECTION));
		break;
	default:
		break;
	}
}


int conn_get() {
	register DNA_CONNECTION *dna_connp;
	int i, n_conns, conn_id;

	DISABLE_AST
		for (i = 1, dna_connp = &Dna_conns[1]; i < Curr_N_Conns; i++, dna_connp++) {
			if (!dna_connp->busy) {
				dna_connp->busy = TRUE;
				ENABLE_AST
					return(i);
			}
		}
	n_conns = Curr_N_Conns + CONN_BLOCK;
	Dna_conns = arr_increase(Dna_conns, sizeof(DNA_CONNECTION), n_conns);
	Net_conns = arr_increase(Net_conns, sizeof(NET_CONNECTION), n_conns);
	switch (My_type) {
		case SRC_DIC:
			Dic_conns = arr_increase(Dic_conns, sizeof(DIC_CONNECTION),
									 n_conns);
			break;
		case SRC_DNS:
			Dns_conns = arr_increase(Dns_conns, sizeof(DNS_CONNECTION),
									 n_conns);
			break;
		default:
			break;
	}
	conn_id = Curr_N_Conns;
	Curr_N_Conns = n_conns;
	Dna_conns[conn_id].busy = TRUE;
	ENABLE_AST
		return(conn_id);
}


void conn_free(int conn_id) {
	DISABLE_AST
		Dna_conns[conn_id].busy = FALSE;
	ENABLE_AST
}

void* arr_increase(void* conn_ptr, int conn_size, int n_conns)
{
	register char* new_ptr;

	new_ptr = realloc(conn_ptr, (size_t)(conn_size * n_conns));
	memset(new_ptr + conn_size * Curr_N_Conns, 0, (size_t)(conn_size * CONN_BLOCK));
	return(new_ptr);
}

int id_get(void* ptr, SRC_TYPES type)
{
	int newId = 0;
	DISABLE_AST
	if (Curr_id == Curr_N_Ids) {
		Curr_N_Ids += ID_BLOCK;
		if (Curr_N_Ids == ID_BLOCK)
			Id_arr = (void*)malloc((size_t)(Curr_N_Ids * sizeof(ID_ITEM)));
		else
			Id_arr = realloc(Id_arr, (size_t)(sizeof(ID_ITEM) * Curr_N_Ids));
		for (int i = Curr_id; i < Curr_N_Ids; i++)
			Id_arr[i].arrId = i;
	}
	newId = Id_arr[Curr_id].arrId;
	Id_arr[newId].ptr = ptr;
	Id_arr[newId].type = type;
	Curr_id++;
	ENABLE_AST
	return(newId);
}

void* id_get_ptr(int id, SRC_TYPES type)
{
	void *tmPtr = 0;
	DISABLE_AST
		tmPtr = Id_arr[id].ptr;
	ENABLE_AST
		return(tmPtr);
}

void id_free(int id, SRC_TYPES type)
{
	DISABLE_AST
	if (Id_arr[id].type == type) {
		Curr_id--;
		Id_arr[Curr_id].arrId = id;
	}
	ENABLE_AST
}
