#ifndef TREE_H
#define TREE_H

#define max_code 10
#define max_name 10
#define max_depnm 10 
#define m 3

typedef enum status_code{false,true} bool;
typedef enum status_code_tag{failure,deleted,merged} status_code;
typedef struct Node_tag
{
	char nm[max_code];
	int data;
	struct Node_tag* next;
}Node;
typedef struct data_node_tag
{
	int roll_no,marks,sem_no;
	char subjcode[max_code],name[max_name],dep_name[max_depnm];
	struct data_node_tag *down;
}data_node;


typedef struct data_list_tag
{
	struct data_list_tag *next,*prev;
	data_node *dnode_head;
	int count;
}data_list_node;

typedef struct key_list_node key_list;
typedef union tree_node_tag tree;
typedef struct inner_node_tag
{
	tree *smallest;
	key_list *head;
	char type;
}inner_node;

typedef struct key_list_node
{
	struct key_list_node *right;
	tree *down;
	int roll_no;
	char subjcode[max_code];
}key_list;

typedef struct key_leaf_node key_leaf;
typedef struct leaf_node_tag
{
	data_list_node *smallest;
	key_leaf *head;
	char type;
}leaf_node;

typedef struct key_leaf_node
{
	struct key_leaf_node *right;
	data_list_node *down;
	int roll_no;
	char subjcode[max_code];
}key_leaf;

typedef union tree_node_tag
{
	inner_node *inode_ptr;
	leaf_node *lnode_ptr;
}tree;

#ifndef insert_h
#define insert_h
key_list* insert(tree**,int,char[],char[],int,char[],int);
#endif

#ifndef insert_sort_h
#define insert_sort_h
void insert_sort(data_list_node**,data_node*);
#endif

#ifndef split_data_h
#define split_data_h
data_list_node* split_data(data_list_node**);
#endif

#ifndef split_key_leaf_h
#define split_key_leaf_h
key_list* split_key_leaf(key_leaf*);
#endif

#ifndef split_key_inner_h
#define split_key_inner_h
key_list* split_key_inner(key_list*);
#endif

#ifndef make_datanode_h
#define make_datanode_h
data_node* make_datanode(int,char[],char[],int,char[],int);
#endif

#ifndef make_datalist_h
#define make_datalist_h
data_list_node* make_datalist(data_node*);
#endif

#ifndef make_leaf_node_h
#define make_leaf_node_h
leaf_node* make_leaf_node(key_leaf*,data_list_node*);
#endif

#ifndef make_inner_node_h
#define make_inner_node_h
inner_node* make_inner_node(key_list*,tree*);
#endif

#ifndef print_data_h
#define print_data_h
void print_data(tree*);
#endif

#ifndef Search_h
#define Search_h
bool Search(tree*,int,char[]);
#endif

#ifndef getNumRecords_h
#define getNumRecords_h
int getNumRecords(tree*);
#endif

#ifndef height_h
#define height_h
int height(tree*);
#endif


#ifndef Range_search_h
#define Range_search_h
void Range_search(tree*,int,char[],int,char[]);
#endif


#ifndef Getmax_h
#define Getmax_h
void Getmax(tree*,char[]);
#endif


#ifndef GetListofFailures_h
#define GetListofFailures_h
void GetListofFailures(tree*,char[],int);
#endif

#ifndef GetTopper_h
#define GetTopper_h
void GetTopper(tree*);
#endif

#ifndef GetKthHighest_h
#define GetKthHighest_h
void GetKthHighest(tree*,char[],int);
#endif

#ifndef mergesort_h
#define mergesort_h
Node* mergesort(Node*);
#endif

#ifndef divide_h
#define divide_h
Node* divide(Node*);
#endif

#ifndef merge_h
#define merge_h
Node* merge(Node*,Node*);
#endif


#ifndef delete_h
#define delete_h
void delete(tree**,int,char[]);
#endif


#ifndef del_h
#define del_h
status_code del(tree**,tree**,int,char[]);
#endif

#ifndef del1_h
#define del1_h
status_code del1(tree**,tree**,int,char[]);
#endif



#ifndef make_key_leaf_h
#define make_key_leaf_h
key_leaf* make_key_leaf(int,char[]);
#endif

#ifndef make_key_list_h
#define make_key_list_h
key_list* make_key_list(int,char[]);
#endif

#ifndef make_key_list_h
#define make_key_list_h
key_list* make_key_list(int,char[]);
#endif

#ifndef display_tree_h
#define display_tree_h
void display_tree(tree*);
#endif


#endif


