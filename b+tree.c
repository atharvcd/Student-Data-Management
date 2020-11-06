#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "b+tree.h"

key_list* insert(tree** root,int r,char n[],char c[],int ms,char d[],int s)
{
	int flag = 1,count;
	tree *rootv,*ptr;
	data_list_node *data,*new_data,*data_ptr;
	data_node *dptr,*trav;
	key_leaf *l_key,*lprev,*lptr,*lhead_ptr,*tmp;
	key_list *iptr,*iprev,*head_ptr,*retval,*tmp_ret,*ihead_ptr;
	rootv=*root;
	if(rootv==NULL)
	{
		rootv=(tree*)malloc(sizeof(tree));
		data=(data_list_node*)malloc(sizeof(data_list_node));
		data->prev=data->next=NULL;
		dptr=make_datanode(r,n,c,ms,d,s);
		data->dnode_head=dptr;
		rootv->lnode_ptr=(leaf_node*)malloc(sizeof(leaf_node));
		rootv->lnode_ptr->smallest=data;
		rootv->lnode_ptr->head=NULL;
		rootv->lnode_ptr->type='l';
		data->count=1;
		*root=rootv;
		retval=NULL;
	}
	
	else if(rootv->lnode_ptr->head==NULL)
	{
		dptr=make_datanode(r,n,c,ms,d,s);
		if(rootv->lnode_ptr->smallest->count < m)
		{
			data=rootv->lnode_ptr->smallest;
			insert_sort(&data,dptr);
		}
		else if(rootv->lnode_ptr->smallest->count == m)
		{
			data=rootv->lnode_ptr->smallest;
			trav=rootv->lnode_ptr->smallest->dnode_head;
			insert_sort(&data,dptr);
			new_data=split_data(&data);
			l_key=(key_leaf*)malloc(sizeof(key_leaf));
			l_key->roll_no=new_data->dnode_head->roll_no;
			strcpy(l_key->subjcode,new_data->dnode_head->subjcode);
			//printf("\n\n%d\t%s\n",l_key->roll_no,l_key->subjcode);
			l_key->right=NULL;
			l_key->down=new_data;
			rootv->lnode_ptr->head=l_key;
		}
		retval=NULL;
	}
	else
	{
		if(rootv->inode_ptr->type=='i')
		{
			iptr=rootv->inode_ptr->head;
			head_ptr=iptr;
			flag=1;
			while(iptr!=NULL && flag==1)
			{
				if(r < iptr->roll_no)
				{
					flag=0;
				}
				else if(iptr->roll_no==r)
				{
					if(strcmp(c,iptr->subjcode)<0)
					{
						flag=0;
						/*iprev=iptr;
						iptr=iptr->right;*/
					}
					else if(strcmp(iptr->subjcode,c)==0)
					{
						flag=0;
						iprev=iptr;
						iptr=iptr->right;
					}
					else
					{	
						iprev=iptr;
						iptr=iptr->right;
					}
				}
				else
				{
					iprev=iptr;
					iptr=iptr->right;
				}
			}
			if(iptr==head_ptr)
			{
				retval=insert(&(rootv->inode_ptr->smallest),r,n,c,ms,d,s);
			}
			else
			{
				retval=insert(&(iprev->down),r,n,c,ms,d,s);
			}
			if(retval!=NULL)
			{
				flag=1;
				tmp_ret=retval;
				iptr=rootv->inode_ptr->head;
				ihead_ptr=iptr;
				count=0;
				while(iptr!=NULL)
				{
					if(tmp_ret->roll_no < iptr->roll_no && flag==1)
					{
						tmp_ret->right=iptr;
						if(iptr!=ihead_ptr)
						{
							iprev->right=tmp_ret;
						}
						else
						{
							ihead_ptr=tmp_ret;
						}	
						flag=0;	
					}
					else if(tmp_ret->roll_no == iptr->roll_no && flag==1)
					{
						if(strcmp(tmp_ret->subjcode,iptr->subjcode)<0)
						{
							tmp_ret->right=iptr;
							if(iptr!=ihead_ptr)
							{
								iprev->right=tmp_ret;
							}
							else
							{
								ihead_ptr=tmp_ret;
							}
							flag=0;
						}
						else
						{
							iprev=iptr;
							iptr=iptr->right;
						}
					}
					else
					{
						iprev=iptr;
						iptr=iptr->right;
					}
					count=count+1;
				}
				if(flag==1)
				{
					iprev->right=tmp_ret;
					tmp_ret->right=NULL;
				}
				count=count+1;
				if(count < m)
					retval=NULL;
				else if(count==m)
					retval=split_key_inner(ihead_ptr);	
			}
		}
		else if(rootv->inode_ptr->type=='l')
		{
			
			flag=1;
			lptr=rootv->lnode_ptr->head;
			lhead_ptr=lptr;
			while(lptr!=NULL && flag==1)
			{
				if(r < lptr->roll_no)
				{
					flag=0;
				}
				else if(lptr->roll_no==r)
				{
					if(strcmp(c,lptr->subjcode)<0)
					{
						flag=0;
						/*lprev=lptr;
						lptr=lptr->right;*/
					}
					else if(strcmp(lptr->subjcode,c)==0)
					{
						flag=0;
						lprev=lptr;
						lptr=lptr->right;
					}
					else
					{	
						lprev=lptr;
						lptr=lptr->right;
					}
				}
				else
				{
					lprev=lptr;
					lptr=lptr->right;
				}
			}
			if(lptr==lhead_ptr)
			{	
				data_ptr=rootv->lnode_ptr->smallest;
			}		
			else
			{
				data_ptr=lprev->down;
			}
			dptr=make_datanode(r,n,c,ms,d,s);
			if(data_ptr->count < m)
			{
				insert_sort(&data_ptr,dptr);
				retval=NULL;
				//printf("\n%d\n",data_ptr->count);
			}
			else if(data_ptr->count >= m)	
			{
				insert_sort(&data_ptr,dptr);
				new_data=split_data(&data_ptr);
				
				
				l_key=(key_leaf*)malloc(sizeof(key_leaf));
				l_key->roll_no=new_data->dnode_head->roll_no;
				strcpy(l_key->subjcode,new_data->dnode_head->subjcode);
				l_key->down=new_data;
				
				
/*this line is changed		lprev->right=l_key;
				l_key->right=lptr;*/
				
				lptr=lhead_ptr;
				count=0;
				flag=1;
				lprev=NULL;
				while(lptr!=NULL)
				{
					if(l_key->roll_no < lptr->roll_no && flag==1)
					{
						l_key->right=lptr;
						if(lptr!=lhead_ptr)
						{
							lprev->right=l_key;
						}
						else
						{
							lhead_ptr=l_key;
						}
						flag=0;	
					}
					else if(l_key->roll_no == lptr->roll_no && flag==1)
					{
						if(strcmp(l_key->subjcode,lptr->subjcode)<0)
						{
							l_key->right=lptr;
							if(lptr!=lhead_ptr)
								lprev->right=l_key;
							else
							{
								lhead_ptr=l_key;
							}	
							flag=0;
						}
						else
						{
							lprev=lptr;
							lptr=lptr->right;
						}
					}
					else
					{
						lprev=lptr;
						lptr=lptr->right;
					}
					count=count+1;
				}
				if(flag==1)
				{
					lprev->right=l_key;
					l_key->right=NULL;
				}
				count=count+1;
				if(count < m)
				{
					retval=NULL;
				}	
				else if(count==m)
				{
					//printf("Now leaf node will split\n");
					retval=split_key_leaf(lhead_ptr);
				}	
			}
		}
	}
	return(retval);
}


void insert_sort(data_list_node **h,data_node *node)
{
	int found=0;
	data_node *ptr,*prev,*tmp;
	ptr=(*h)->dnode_head;
	prev=(data_node*)malloc(sizeof(data_node));
	prev->down=ptr;
	tmp=prev;
	while(ptr!=NULL && found==0)
	{
		if(ptr->roll_no < node->roll_no)
		{
			prev=ptr;
			ptr=ptr->down;
		}
		else if(ptr->roll_no==node->roll_no)
		{
			if(strcmp(ptr->subjcode,node->subjcode) < 0)
			{
				prev=ptr;
				ptr=ptr->down;
			}
			else if(strcmp(ptr->subjcode,node->subjcode)==0)
			{
				found=1;
				prev->down=node;
				node->down=ptr->down;
				free(ptr);
				(*h)->count=(*h)->count - 1;
			}
			else
			{
				found=1;
				prev->down=node;
				node->down=ptr;
			}
		}
		else
		{
			found=1;
			prev->down=node;
			node->down=ptr;
		}	
	}
	
	if(found==0)
	{
		prev->down=node;
		node->down=NULL;
	}
	(*h)->count=((*h)->count)+1;
	if(prev==tmp)
	{
		(*h)->dnode_head=prev->down;
	}
	free(tmp);
}


data_list_node* split_data(data_list_node **list_node)
{
	data_node *tmp,*ptr,*trail;
	data_list_node *d,*h;
	h=*list_node;
	trail=h->dnode_head;
	ptr=trail->down;
	while(ptr!=NULL)
	{
		tmp=trail;
		trail=trail->down;
		ptr=ptr->down;
		if(ptr!=NULL)
		{
			ptr=ptr->down;
		}
	}
	tmp->down=NULL;
	d=make_datalist(trail);
	if(m%2==1)
	{
		d->count=h->count=(h->count)/2;
	}
	else
	{
		h->count=(h->count)/2;
		d->count=(h->count)+1;
	}
	d->next=h->next;
	if(h->next!=NULL)
		h->next->prev=d;
	h->next=d;
	d->prev=h;
	return(d);
}


data_node* make_datanode(int r,char n[],char c[],int ms,char d[],int s)
{
	data_node *dptr;
	dptr=(data_node*)malloc(sizeof(data_node));
	dptr->roll_no=r;
	strcpy(dptr->name,n);
	strcpy(dptr->subjcode,c);
	dptr->marks=ms;
	strcpy(dptr->dep_name,d);
	dptr->sem_no=s;
	dptr->down=NULL;
	return(dptr);
}

data_list_node* make_datalist(data_node *dptr)
{
	data_list_node *dlist;
	dlist=(data_list_node*)malloc(sizeof(data_list_node));
	dlist->dnode_head=dptr;
	dlist->prev=dlist->next=NULL;
	return(dlist);
}


key_list* split_key_leaf(key_leaf *leaf_head)
{
	key_leaf *trail=leaf_head,*ptr=trail->right,*tmp;
	key_list *list_ptr;
	leaf_node *l;
	tree *t1;
	while(ptr!=NULL)
	{
		tmp=trail;
		trail=trail->right;
		ptr=ptr->right;
		if(ptr!=NULL)
			ptr=ptr->right;	
	}
	tmp->right=NULL;
	tmp=trail->right;
	trail->right=NULL;
	l=make_leaf_node(tmp,trail->down);
	t1=(tree*)malloc(sizeof(tree));
	t1->lnode_ptr=l;
	list_ptr=(key_list*)malloc(sizeof(key_list));
	list_ptr->roll_no=trail->roll_no;
	strcpy(list_ptr->subjcode,trail->subjcode);
	list_ptr->right=NULL;
	list_ptr->down=t1;
	free(trail);
	return(list_ptr);
}

key_list* split_key_inner(key_list *inner_head)
{
	key_list *trail=inner_head,*ptr=trail->right,*tmp;
	inner_node *l;
	tree *t1;
	while(ptr!=NULL)
	{
		tmp=trail;
		trail=trail->right;
		ptr=ptr->right;
		if(ptr!=NULL)
			ptr=ptr->right;	
	}
	tmp->right=NULL;
	tmp=trail->right;
	trail->right=NULL;
	l=make_inner_node(tmp,trail->down);
	t1=(tree*)malloc(sizeof(tree));
	t1->inode_ptr=l;
	trail->down=t1;
	return(trail);
}


leaf_node* make_leaf_node(key_leaf* head_ptr,data_list_node* child_ptr)
{
	leaf_node *retval;
	retval=(leaf_node*)malloc(sizeof(leaf_node));
	retval->head=head_ptr;
	retval->smallest=child_ptr;
	retval->type='l';
	return(retval);
}


inner_node* make_inner_node(key_list* head_ptr,tree* child_ptr)
{
	inner_node *retval;
	retval=(inner_node*)malloc(sizeof(inner_node));
	retval->head=head_ptr;
	retval->smallest=child_ptr;
	retval->type='i';
	return(retval);
}


void print_data(tree *root)
{
	tree *ptr;
	data_list_node *data_ptr;
	data_node *dptr;
	ptr=root;
	if(root==NULL)
	{
		printf("\ndatabase is empty\n");
	}
	else{
	while(ptr->inode_ptr->type != 'l')
	{
		ptr=ptr->inode_ptr->smallest;
	}
	data_ptr=ptr->lnode_ptr->smallest;
	while(data_ptr != NULL)
	{
		dptr=data_ptr->dnode_head;
		while(dptr!=NULL)
		{
			printf("%d ",dptr->roll_no);
			printf("%s ",dptr->name);
			printf("%s ",dptr->subjcode);
			printf("%d ",dptr->marks);
			printf("%s ",dptr->dep_name);
			printf("%d \n\n",dptr->sem_no);
			dptr=dptr->down;
		}
		data_ptr=data_ptr->next;
	}
	}
}


bool Search(tree *root,int r,char c[])
{
	tree *t;
	int flag,found;
	bool retval=false;
	key_list *list_ptr,*list_prev=NULL;
	key_leaf *leaf_ptr,*leaf_prev;
	data_list_node *data_ptr;
	data_node *dptr;
	t=root;
	if(root==NULL)
	{
		printf("Database is empty\n");
		retval=false;
	}
	else if(root->inode_ptr->head==NULL)
	{
		data_ptr=t->lnode_ptr->smallest;
		dptr=data_ptr->dnode_head;
		found=1;
		while(dptr!=NULL && found==1)
		{
			if(dptr->roll_no==r && strcmp(dptr->subjcode,c)==0)
			{
				printf("\nGiven data is present in the database\n");
				printf("%d %s %s %d %s %d\n",dptr->roll_no,dptr->name,dptr->subjcode,dptr->marks,dptr->dep_name,dptr->sem_no);
				retval=true;
				found=0;
			}
			else
			{
				dptr=dptr->down;
			}
		}
	}
	else
	{
		while(t->inode_ptr->type!='l')
		{
			list_ptr=t->inode_ptr->head;
			flag=1;
			while(list_ptr!=NULL && flag==1)
			{
				if(r < list_ptr->roll_no)
				{
					flag=0;
				}
				else if(r==list_ptr->roll_no)
				{
					if(strcmp(c,list_ptr->subjcode)<0)
					{
						flag=0;
					}
					else if(strcmp(list_ptr->subjcode,c)==0)
					{
						flag=0;
						list_prev=list_ptr;
						list_ptr=list_ptr->right;
					}
					else
					{
						list_prev=list_ptr;
						list_ptr=list_ptr->right;
					}
				}
				else
				{
					list_prev=list_ptr;
					list_ptr=list_ptr->right;
				}
			}
			if(list_ptr==t->inode_ptr->head)
			{
				t=t->inode_ptr->smallest;
			}
			else
			{
				t=list_prev->down;
			}
		}
		leaf_ptr=t->lnode_ptr->head;
		flag=1;
		while(leaf_ptr!=NULL && flag==1)
		{
			if(r < leaf_ptr->roll_no)
				{
					flag=0;
				}
				else if(r==leaf_ptr->roll_no)
				{
					if(strcmp(c,leaf_ptr->subjcode)<0)
					{
						flag=0;
					}
					else if(strcmp(leaf_ptr->subjcode,c)==0)
					{
						flag=0;
						leaf_prev=leaf_ptr;
						leaf_ptr=leaf_ptr->right;
					}
					else
					{
						leaf_prev=leaf_ptr;
						leaf_ptr=leaf_ptr->right;
					}
				}
				else
				{
					leaf_prev=leaf_ptr;
					leaf_ptr=leaf_ptr->right;
				}
		}
		if(leaf_ptr==t->lnode_ptr->head)
		{
			data_ptr=t->lnode_ptr->smallest;
		}
		else
		{
			data_ptr=leaf_prev->down;
		}
		dptr=data_ptr->dnode_head;
		found=1;
		while(dptr!=NULL && found==1)
		{
			if(dptr->roll_no==r && strcmp(dptr->subjcode,c)==0)
			{
				printf("\nGiven data is present in the database\n");
				printf("%d %s %s %d %s %d\n",dptr->roll_no,dptr->name,dptr->subjcode,dptr->marks,dptr->dep_name,dptr->sem_no);
				retval=true;
				found=0;
			}
			else
			{
				dptr=dptr->down;
			}
		}
	}
	return(retval);
}

int getNumRecords(tree* root)
{
	
	int count=0;
	tree *ptr;
	data_list_node *data_ptr;
	ptr=root;
	if(ptr==NULL)
	{
		printf("\nDatabase is empty\n");
	}
	else 
	{
		if(ptr->inode_ptr->type=='l')
		{
			data_ptr=ptr->lnode_ptr->smallest;
		}
		else if(ptr->inode_ptr->type == 'i')
		{
			while(ptr->inode_ptr->type!='l')
			{
				ptr=ptr->inode_ptr->smallest;
			}
		}	
		
		data_ptr=ptr->lnode_ptr->smallest;
		while(data_ptr!=NULL)
		{
			count=count+(data_ptr->count);
			data_ptr=data_ptr->next;
		}
	}
	return(count);
}

int height(tree *root)
{
	int retval=0;
	tree *ptr;
	ptr=root;
	if(ptr==NULL)
	{
		retval=0;
	}
	else
	{
		while(ptr->inode_ptr->type!='l')
		{
			retval=retval+1;
			ptr=ptr->inode_ptr->smallest;
		}
		retval=retval+2;
	}
	return(retval);
}

void Range_search(tree *root,int r1,char c1[],int r2,char c2[])
{
	tree *t;
	int flag,found;
	key_list *list_ptr,*list_prev=NULL;
	key_leaf *leaf_ptr,*leaf_prev;
	data_list_node *data_ptr;
	data_node *dptr,*tmp;
	t=root;
	if(root==NULL)
	{
		printf("Database is empty\n");
	}
	else if(root->inode_ptr->head==NULL)
	{
		data_ptr=t->lnode_ptr->smallest;
		dptr=data_ptr->dnode_head;
		found=1;
		while(dptr!=NULL && found==1)
		{
			if(dptr->roll_no==r1 && strcmp(dptr->subjcode,c1)==0)
			{
				found=0;
			}
			else
			{
				dptr=dptr->down;
			}
		}
	}
	else
	{
		while(t->inode_ptr->type!='l')
		{
			list_ptr=t->inode_ptr->head;
			flag=1;
			while(list_ptr!=NULL && flag==1)
			{
				if(r1 < list_ptr->roll_no)
				{
					flag=0;
				}
				else if(r1==list_ptr->roll_no)
				{
					if(strcmp(c1,list_ptr->subjcode)<0)
					{
						flag=0;
					}
					else if(strcmp(list_ptr->subjcode,c1)==0)
					{
						flag=0;
						list_prev=list_ptr;
						list_ptr=list_ptr->right;
					}
					else
					{
						list_prev=list_ptr;
						list_ptr=list_ptr->right;
					}
				}
				else
				{
					list_prev=list_ptr;
					list_ptr=list_ptr->right;
				}
			}
			if(list_ptr==t->inode_ptr->head)
			{
				t=t->inode_ptr->smallest;
			}
			else
			{
				t=list_prev->down;
			}
		}
		leaf_ptr=t->lnode_ptr->head;
		flag=1;
		
		while(leaf_ptr!=NULL && flag==1)
		{
				if(r1 < leaf_ptr->roll_no)
				{
					flag=0;
				}
				else if(r1==leaf_ptr->roll_no)
				{
					if(strcmp(c1,leaf_ptr->subjcode)<0)
					{
						flag=0;
					}
					else if(strcmp(leaf_ptr->subjcode,c1)==0)
					{
						flag=0;
						leaf_prev=leaf_ptr;
						leaf_ptr=leaf_ptr->right;
					}
					else
					{
						leaf_prev=leaf_ptr;
						leaf_ptr=leaf_ptr->right;
					}
				}
				else
				{
					leaf_prev=leaf_ptr;
					leaf_ptr=leaf_ptr->right;
				}
		}
		if(leaf_ptr==t->lnode_ptr->head)
		{
			data_ptr=t->lnode_ptr->smallest;
		}
		else
		{
			data_ptr=leaf_prev->down;
		}
		dptr=data_ptr->dnode_head;
		found=1;
		while(dptr!=NULL && found==1)
		{
			if(dptr->roll_no==r1 && strcmp(dptr->subjcode,c1)==0)
			{
				found=0;
			}
			else
			{
				dptr=dptr->down;
			}
		}
	}
	if(found==1)
	{
		printf("First key not present\n");
	}
	else
	{
		found=1;
		tmp=dptr;
		printf("Roll-no Name Subject Marks Department Semester\n");
		while(data_ptr!=NULL && found==1)
		{
			if(tmp!=dptr)
				dptr=data_ptr->dnode_head;
			while(dptr!=NULL && found==1)
			{
				if(dptr->roll_no==r2 && strcmp(dptr->subjcode,c2)==0)
					found=0;
				printf("%d\t%s\t%s\t%d\t%s\t%d\n",dptr->roll_no,dptr->name,dptr->subjcode,dptr->marks,dptr->dep_name,dptr->sem_no);
				dptr=dptr->down;
			}
			data_ptr=data_ptr->next;
		}
	}
}


void Getmax(tree *root,char c[])
{
	tree *ptr;
	int max=-1,flag=1;
	data_list_node *data_ptr;
	data_node *dptr,*new_node,*trail_ptr,*head,*p;
	ptr=root;
	trail_ptr=NULL;
	head=NULL;
	if(ptr==NULL)
	{
		printf("Database is empty\n");
	}
	else
	{
	while(ptr->inode_ptr->type != 'l')
	{
		ptr=ptr->inode_ptr->smallest;
	}
	data_ptr=ptr->lnode_ptr->smallest;
	while(data_ptr!=NULL)
	{
		dptr=data_ptr->dnode_head;
		while(dptr!=NULL)
		{
			if(strcmp(dptr->subjcode,c)==0)
			{
				flag=0;
				if(dptr->marks > max)
				{
					max=dptr->marks;
					new_node=make_datanode(dptr->roll_no,dptr->name,dptr->subjcode,dptr->marks,dptr->dep_name,dptr->sem_no);
					new_node->down=NULL;
					if(head!=NULL)
					{
						p=head;
						while(p!=NULL)
						{
							p=head->down;
							free(head);
							head=p;
						}
					}
					head=new_node;
					trail_ptr=new_node;
				}
				else if(dptr->marks==max)
				{
					new_node=make_datanode(dptr->roll_no,dptr->name,dptr->subjcode,dptr->marks,dptr->dep_name,dptr->sem_no);
					new_node->down=NULL;
					trail_ptr->down=new_node;
				}
			}
			dptr=dptr->down;
		}
		data_ptr=data_ptr->next;
	}
	if(flag==1)
	{
		printf("No such subject entry in database\n");
	}
	else
	{
		p=head;
		while(p!=NULL)
		{
			printf("%d %s %s %d %s %d\n",p->roll_no,p->name,p->subjcode,p->marks,p->dep_name,p->sem_no);
			p=p->down;
		}
	}
 }
}


void GetListofFailures(tree *root,char c[],int p)
{
	tree *ptr;
	int flag=1;
	data_list_node *data_ptr;
	data_node *dptr;
	ptr=root;
	if(root==NULL)
	{
		printf("Database is empty\n");
	}
	else
	{
		while(ptr->inode_ptr->type!='l')
		{
			ptr=ptr->inode_ptr->smallest;
		}
		data_ptr=ptr->lnode_ptr->smallest;
		while(data_ptr!=NULL)
		{
			dptr=data_ptr->dnode_head;
			while(dptr!=NULL)
			{
				if(strcmp(dptr->subjcode,c)==0 && dptr->marks<p)
				{
					flag=0;
					printf("%d\t%s\t%s\t%d\t%s\t%d\n",dptr->roll_no,dptr->name,dptr->subjcode,dptr->marks,dptr->dep_name,dptr->sem_no);
				}
				dptr=dptr->down;
			}
			data_ptr=data_ptr->next;
		}
		if(flag==1)
			printf("All the students have passed\n");
	}
}

void GetTopper(tree *root)
{
	tree *ptr;
	int max=-1,total=0,r;
	data_list_node *data_ptr;
	data_node* dptr,*new_node,*dhead=NULL,*dtrail=NULL,*tmp,*dprev;
	ptr=root;
	if(root==NULL)
	{
		printf("Database is empty\n");
	}
	else
	{
		while(ptr->inode_ptr->type!='l')
		{
			ptr=ptr->inode_ptr->smallest;
		}
		data_ptr=ptr->lnode_ptr->smallest;
		r=data_ptr->dnode_head->roll_no;
		dprev=data_ptr->dnode_head;
		while(data_ptr!=NULL)
		{
			dptr=data_ptr->dnode_head;
			while(dptr!=NULL)
			{
				if(dptr->roll_no==r)
				{
					total=total+dptr->marks;
				}
				else
				{
					if(max<total)
					{
						max=total;
						new_node=make_datanode(dprev->roll_no,dprev->name,"---",total,dprev->dep_name,dprev->sem_no);
						new_node->down=NULL;
						if(dhead!=NULL)
						{
							tmp=dhead;
							while(tmp!=NULL)
							{
								tmp=tmp->down;
								free(dhead);
								dhead=tmp;
							}
						}
						dhead=dtrail=new_node;
					}
					else if(max==total)
					{
						dtrail->down=new_node;
						dtrail=new_node;
					}
					total=dptr->marks;
					r=dptr->roll_no;
					dprev=dptr;
				}
				dptr=dptr->down;
			}
			data_ptr=data_ptr->next;
		}
		if(total > max)
		{
			new_node=make_datanode(dprev->roll_no,dprev->name,"---",total,dprev->dep_name,dprev->sem_no);
			if(dhead!=NULL)
			{
				tmp=dhead;
				while(tmp!=NULL)
				{
					tmp=tmp->down;
					free(dhead);
					dhead=tmp;
				}
			}
			dhead=dtrail=new_node;
		}
		else if(total == max)
		{
			new_node=make_datanode(dprev->roll_no,dprev->name,"---",total,dprev->dep_name,dprev->sem_no);
			dtrail->down=new_node;
			new_node->down=NULL;
		}
		tmp=dhead;
		while(tmp!=NULL)
		{
			printf("%d %s %s %d %s %d\n",tmp->roll_no,tmp->name,tmp->subjcode,tmp->marks,tmp->dep_name,tmp->sem_no);
			tmp=tmp->down;
		}
	}
	
}

void GetKthHighest(tree *root,char c[],int k)
{
	Node *nptr,*new_node,*prev=NULL;
	tree *ptr;
	data_list_node *data_ptr;
	data_node *dptr;
	ptr=root;
	int count=1,s;
	if(root==NULL)
	{
		printf("Database is empty\n");
	}
	else
	{
		while(ptr->inode_ptr->type!='l')
		{
			ptr=ptr->inode_ptr->smallest;
		}
		data_ptr=ptr->lnode_ptr->smallest;
		dptr=data_ptr->dnode_head;
		while(data_ptr!=NULL)
		{
			dptr=data_ptr->dnode_head;
			while(dptr!=NULL)
			{
				if(strcmp(dptr->subjcode,c)==0)
				{
					new_node=(Node*)malloc(sizeof(Node));
					new_node->data=dptr->marks;
					strcpy(new_node->nm,dptr->name);
					new_node->next=prev;
					prev=new_node;
				}
				dptr=dptr->down;
			}
			data_ptr=data_ptr->next;
		}
		
		nptr=mergesort(prev);
		s=nptr->data;
		count=1;
		while(nptr!=NULL)
		{
			
			if(s!=nptr->data)
			{
				count=count+1;
				s=nptr->data;
			}
			if(count==k)
			{
				printf("%d %s\n",nptr->data,nptr->nm);
			}
			nptr=nptr->next;
		}
	}
}


Node* mergesort(Node *head)
{
	Node *mid,*retval;
	retval=head;
	if(head!=NULL && head->next!=NULL)
	{
		
		mid=divide(head);
		head=mergesort(head);
		mid=mergesort(mid);
		retval=merge(head,mid);
	}
	return(retval);
}

Node* divide(Node* head)
{
	Node *slow,*fast,*retval;
	slow=fast=head;
	if(slow==NULL)
		retval=slow;
	else if(slow->next==NULL)
		retval=slow->next;	
	else
	{
		fast=fast->next->next;
		while(fast!=NULL)
		{
			slow=slow->next;
			fast=fast->next;
			if(fast!=NULL)
				fast=fast->next;
		}
		retval=slow->next;
		slow->next=NULL;
	}
return(retval);
}

Node* merge(Node* lptr1,Node* lptr2)
{
	Node *h,*r,*p,*q;
	p=lptr1;
	q=lptr2;
	if(p==NULL)
		h=q;
	else if(q==NULL)
		h=p;
	else
	{
		if(p->data < q->data)
		{
			r=q;
			q=q->next;
		}
		else
		{
			r=p;
			p=p->next;
		}
		h=r;
		while(p!=NULL && q!=NULL)
		{
			if(p->data < q->data)
			{
				r->next=q;
				q=q->next;
				r=r->next;
			}
			else
			{
				r->next=p;
				p=p->next;
				r=r->next;
			}	
		}
		if(p!=NULL)
			r->next=p;
		else if(p==NULL)
			r->next=q;
	}
	return(h);
}


void delete(tree **root,int r,char c[])
{
	tree *rootv,*ptr;
	rootv=*root;
	int found;
	status_code rval;
	data_list_node* data_ptr;
	data_node *dptr,*dprev;
	if(rootv==NULL)
	{
		printf("Database is empty\n");
	}
	else if(rootv->lnode_ptr->head==NULL)
	{
		found=1;
		data_ptr=rootv->lnode_ptr->smallest;
		dptr=data_ptr->dnode_head;
		while(dptr!=NULL && found==1)
		{
			if(dptr->roll_no==r && strcmp(dptr->subjcode,c)==0)
			{
				found=0;
			}
			else
			{
				dprev=dptr;
				dptr=dptr->down;
			}
		}
		if(found==1)
		{
			//printf("Record not present in database\n");
			rval=failure;
		}
		else
		{
			if(dptr==data_ptr->dnode_head)
			{
				dprev=data_ptr->dnode_head=dptr->down;
				free(dptr);
			}
			else
			{
				dprev->down=dptr->down;
				free(dptr);
			}
			data_ptr->count=data_ptr->count-1;
			rval=deleted;
			//printf("Deletion Successful\n");	
			if(data_ptr->dnode_head==NULL)
			{
				printf("database has now become empty\n");
			}
		}
	}
	else
	{
		rval=del(root,NULL,r,c);
		if(rval==merged)
		{
			ptr=*root;
			*root=rootv->inode_ptr->smallest;
			free(ptr);
		}
		if(rval==merged || rval==deleted)
			printf("Deletion Successful\n");
		else if(rval==failure)
			printf("Data Entry not present\n");	
	}
	
	
}


status_code del(tree** root,tree** parent,int r,char c[])
{
	int flag = 1,count,min,found,key_count,c1,c2;
	status_code rval;
	tree *rootv,*ptr;
	data_list_node *data,*new_data,*data_ptr,*data_sib1,*data_sib2;
	data_node *dptr,*new_node,*d,*tail,*dprev;
	key_leaf *l_key,*lprev,*lptr,*lhead_ptr,*tmp,*lnode1,*kptr,*kprev,*key_sib1,*key_sib2,*last,*new_key_leaf,*prev_last,*kkptr,*kkptr1,*lnode_prev;
	key_list *iptr,*iprev,*head_ptr,*retval,*tmp_ret,*ihead_ptr,*parent_ptr,*parent_prev,*parent_next;
	rootv=*root;

	if(rootv->inode_ptr->type=='i')
		{
			iptr=rootv->inode_ptr->head;
			head_ptr=iptr;
			flag=1;
			while(iptr!=NULL && flag==1)
			{
				if(r < iptr->roll_no)
				{
					flag=0;
				}
				else if(iptr->roll_no==r)
				{
					if(strcmp(c,iptr->subjcode)<0)
					{
						flag=0;
						/*iprev=iptr;
						iptr=iptr->right;*/
					}
					else if(strcmp(iptr->subjcode,c)==0)
					{
						flag=0;
						iprev=iptr;
						iptr=iptr->right;
					}
					else
					{	
						iprev=iptr;
						iptr=iptr->right;
					}
				}
				else
				{
					iprev=iptr;
					iptr=iptr->right;
				}
			}
			if(iptr==head_ptr)
			{
				rval=del(&(rootv->inode_ptr->smallest),&rootv,r,c);
			}
			else
			{
				rval=del(&(iprev->down),&rootv,r,c);
			}
			
			if(rval==merged)
			{
				rval=del1(root,parent,r,c);
			}
		}	
		else if(rootv->inode_ptr->type=='l')
		{
			flag=1;
			lptr=rootv->lnode_ptr->head;
			lhead_ptr=lptr;
			lnode_prev=NULL;
			lprev=NULL;
			while(lptr!=NULL && flag==1)
			{
				if(r < lptr->roll_no)
				{
					flag=0;
				}
				else if(lptr->roll_no==r)
				{
					if(strcmp(c,lptr->subjcode)<0)
					{
						flag=0;
						/*lprev=lptr;
						lptr=lptr->right;*/
					}
					else if(strcmp(lptr->subjcode,c)==0)
					{
						flag=0;
						lnode_prev=lprev;
						lprev=lptr;
						lptr=lptr->right;
					}
					else
					{	
						lnode_prev=lprev;
						lprev=lptr;
						lptr=lptr->right;
					}
				}
				else
				{
					lnode_prev=lprev;
					lprev=lptr;
					lptr=lptr->right;
				}
			}
			if(lptr==lhead_ptr)
			{	
				data_ptr=rootv->lnode_ptr->smallest;
				lnode1=lptr;
				lnode_prev=NULL;
			}		
			else
			{
				data_ptr=lprev->down;
				lnode1=lprev;
			}
			dptr=data_ptr->dnode_head;
			found=1;
			while(dptr!=NULL && found==1)
			{
				if(dptr->roll_no==r && strcmp(dptr->subjcode,c)==0)
				{
					found=0;
				}
				else
				{
					dprev=dptr;
					dptr=dptr->down;
				}
			}
			if(found==1)
			{
				//printf("Record not present in database\n");
				rval=failure;
			}
			else
			{
				if(dptr==data_ptr->dnode_head)
				{
					dprev=data_ptr->dnode_head=dptr->down;
					free(dptr);
				}
				else
				{
					dprev->down=dptr->down;
					free(dptr);
				}
				data_ptr->count=data_ptr->count-1;
				
				min=m/2;
				if(m%2!=0)
					min=min+1;	
				if(data_ptr->count < min)
				{
					printf("count is less");
					if(data_ptr==rootv->lnode_ptr->smallest)
					{
						data_sib2=NULL;
						data_sib1=data_ptr->next;
					}
					else if(lnode1->right==NULL)
					{
						data_sib1=NULL;
						data_sib2=data_ptr->prev;
					}
					else
					{
						data_sib1=data_ptr->next;
						data_sib2=data_ptr->prev;
					}
					
					
					if(data_sib1==NULL)
					{
						if(data_sib2->count > min)
						{
							printf("next is NULL");
							dptr=data_ptr->dnode_head;
							d=data_sib2->dnode_head;
							tail=NULL;
							while(d->down!=NULL)
							{
								tail=d;	
								d=d->down;			
							}
							tail->down=NULL;
							data_sib2->count=data_sib2->count-1;
							d->down=dptr;
							data_ptr->dnode_head=d;
							data_ptr->count=data_ptr->count+1;
							lnode1->roll_no=d->roll_no;
							strcpy(lnode1->subjcode,d->subjcode);
							rval=deleted;
						}
/*~~~~~~~~~~~~*/						else
						{
								
								dptr=data_ptr->dnode_head;
								d=data_sib2->dnode_head;
								while(d->down!=NULL)
								{
									d=d->down;
								}
								data_sib2->next=data_ptr->next;
								d->down=dptr;
								data_sib2->count=data_ptr->count+data_sib2->count;
								if(lnode1==lhead_ptr)
									rootv->lnode_ptr->head=NULL;
								else
								{
									lnode_prev->right=lnode1->right;
								}	
								
								free(data_ptr);
								free(lnode1);
								//rootv->lnode_ptr->head=NULL;
								rval=merged;
						}
					}
				
				
					else if(data_sib2==NULL)
					{
						if(data_sib1->count > min)
						{
							dptr=data_sib1->dnode_head;
							d=data_ptr->dnode_head;
							while(d->down!=NULL)
							{
								d=d->down;
							}
							d->down=dptr;
							data_sib1->dnode_head=dptr->down;
							dptr->down=NULL;
							data_ptr->count=data_ptr->count+1;
							data_sib1->count=data_sib1->count-1;
							lnode1->roll_no=data_sib1->dnode_head->roll_no;
							strcpy(lnode1->subjcode,data_sib1->dnode_head->subjcode);
							rval=deleted;
						}
						else
						{
							dptr=data_sib1->dnode_head;
							d=data_ptr->dnode_head;
							while(d->down!=NULL)
							{
								d=d->down;
							}
							d->down=dptr;
							data_ptr->count=data_ptr->count+data_sib1->count;
							rootv->lnode_ptr->head=lnode1->right;
							data_ptr->next=data_sib1->next;
							if(data_sib1->next!=NULL)
								data_sib1->next->prev=data_ptr;
							free(data_sib1);
							free(lnode1);
							rval=merged;
						}
							
					}
				
				
				else
				{
					if(data_sib1->count > min)
					{
						dptr=data_sib1->dnode_head;
						d=data_ptr->dnode_head;
						while(d->down!=NULL)
						{
							d=d->down;
						}
						d->down=dptr;
						data_sib1->dnode_head=dptr->down;
						data_ptr->count=data_ptr->count+1;
						data_sib1->count=data_sib1->count-1;
						dptr->down=NULL;
						lnode1->roll_no=data_sib1->dnode_head->roll_no;
						strcpy(lnode1->subjcode,data_sib1->dnode_head->subjcode);
						rval=deleted;
					}
					else if(data_sib2->count > min)
					{
						dptr=data_ptr->dnode_head;
						d=data_sib2->dnode_head;
						tail=NULL;
						while(d->down!=NULL)
						{
							tail=d;	
							d=d->down;			
						}
						tail->down=NULL;
						d->down=dptr;
						data_ptr->dnode_head=d;
						lnode1->roll_no=d->roll_no;
						strcpy(lnode1->subjcode,d->subjcode);
						data_ptr->count=data_ptr->count+1;
						data_sib2->count=data_sib2->count-1;
						rval=deleted;
					}
					else
					{
						dptr=data_sib1->dnode_head;
						d=data_ptr->dnode_head;
						while(d->down!=NULL)
						{
							d=d->down;
						}
						d->down=dptr;
						data_ptr->count=data_ptr->count+data_sib1->count;
						data_ptr->next=data_sib1->next;
						if(data_sib1->next!=NULL)
							data_sib1->next->prev=data_ptr;
						lnode_prev->right=lnode1->right;	
						free(data_sib1);
						free(lnode1);
						rval=merged;
					}
				}
			}
			else if(data_ptr->count > min)
			{
				rval=deleted;
			}
		
			if(rval==merged)
			{
				lptr=rootv->lnode_ptr->head;
				key_count=0;
				while(lptr!=NULL)
				{
					key_count=key_count+1;
					last=lptr;
					lptr=lptr->right;
				}
				
				if(key_count < min-1 /*&& rootv->lnode_ptr->head!=NULL*/)
				{
					
					parent_ptr=(*parent)->inode_ptr->head;
					parent_prev=NULL;
					if(rootv==(*parent)->inode_ptr->smallest)
					{
						parent_next=(*parent)->inode_ptr->head;
						key_sib1=parent_next->down->lnode_ptr->head;
						kptr=key_sib1;
						key_count=0;
						while(kptr!=NULL)
						{
							key_count=key_count+1;
							kptr=kptr->right;
						}
						if(key_count < min-1)
						{
							if(lptr==NULL)
							{
								new_key_leaf=make_key_leaf((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								new_key_leaf->down=parent_next->down->lnode_ptr->smallest;
								new_key_leaf->right=key_sib1;
								rootv->lnode_ptr->head=new_key_leaf;
								free(parent_next->down);
								(*parent)->inode_ptr->head=parent_next->right;
								free(parent_next);
								rval=merged;
							}
							else
							{
								new_key_leaf=make_key_leaf((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								last->right=new_key_leaf;
								new_key_leaf->down=parent_next->down->lnode_ptr->smallest;
								new_key_leaf->right=key_sib1;
								free(parent_next->down);
								(*parent)->inode_ptr->head=parent_next->right;
								free(parent_next);
								rval=merged;
							}
						}
						else
						{
							if(lptr==NULL)
							{
								new_key_leaf=make_key_leaf((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								(*parent)->inode_ptr->head->roll_no=key_sib1->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,key_sib1->subjcode);
								new_key_leaf->down=parent_next->down->lnode_ptr->smallest;
								rootv->lnode_ptr->head=new_key_leaf;
								parent_next->down->lnode_ptr->smallest=key_sib1->down;
								(*parent)->inode_ptr->head->down->lnode_ptr->head=key_sib1->right;
								free(key_sib1);
								rval=deleted;
							}
							else
							{
								new_key_leaf=make_key_leaf((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								last->right=new_key_leaf;
								(*parent)->inode_ptr->head->roll_no=key_sib1->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,key_sib1->subjcode);	
								last->right->down=parent_next->down->lnode_ptr->smallest;
								parent_next->down->lnode_ptr->smallest=key_sib1->down;
								(*parent)->inode_ptr->head->down->lnode_ptr->head=key_sib1->right;
								free(key_sib1);
								rval=deleted;
							}	
						}
					}
					else
					{
						parent_ptr=(*parent)->inode_ptr->head;
						while(parent_ptr->down!=rootv)
						{
							parent_prev=parent_ptr;
							parent_ptr=parent_ptr->right;
						}
						parent_next=parent_ptr->right;
						if(parent_ptr==(*parent)->inode_ptr->head)
						{
							kptr=(*parent)->inode_ptr->smallest->lnode_ptr->head;
							parent_prev=NULL;
						}
						else
						 {
						 	kptr=parent_prev->down->lnode_ptr->head;
						}						
						key_count=0;
						prev_last=NULL;
							
							while(kptr->right!=NULL)
							{
								key_count=key_count+1;
								prev_last=kptr;
								kptr=kptr->right;
							}
							key_count=key_count+1;
							
						if(parent_next==NULL)
						{
							
							if(key_count <= min-1)
							{
								new_key_leaf=make_key_leaf(parent_ptr->roll_no,parent_ptr->subjcode);
								new_key_leaf->right=parent_ptr->down->lnode_ptr->head;
								new_key_leaf->down=parent_ptr->down->lnode_ptr->smallest;
								kptr->right=new_key_leaf;
								free(parent_ptr->down);
								free(parent_ptr);
								if(parent_prev!=NULL)
									parent_prev->right=NULL;
								else
									(*parent)->inode_ptr->head=NULL;	
								rval=merged;
							}
							else
							{
								new_key_leaf=make_key_leaf(parent_ptr->roll_no,parent_ptr->subjcode);
								new_key_leaf->right=parent_ptr->down->lnode_ptr->head;
								new_key_leaf->down=parent_ptr->down->lnode_ptr->smallest;
								parent_ptr->roll_no=kptr->roll_no;
								strcpy(parent_ptr->subjcode,kptr->subjcode);
								prev_last->right=NULL;
								parent_ptr->down->lnode_ptr->smallest=kptr->down;
								//kptr->down->next=new_key_leaf->down;
								//new_key_leaf->down->prev=kptr->down;
								free(kptr);
								rval=deleted;
								if(lptr==NULL)
								{
									parent_ptr->down->lnode_ptr->head=new_key_leaf;
								}
							}
						}
						else
						{
							c1=c2=0;
							kkptr=parent_next->down->lnode_ptr->head;
							kkptr1=parent_prev->down->lnode_ptr->head;
							while(kkptr!=NULL || kkptr1!=NULL)
							{
								if(kkptr!=NULL)
								{
									kkptr=kkptr->right;
									c1=c1+1;
								}
								if(kkptr1!=NULL)
								{
									kkptr1=kkptr1->right;
									c2=c2+1;
								}
							}
							
							if(c1>min-1)
							{
								if(lptr==NULL)
							{
								new_key_leaf=make_key_leaf((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								(*parent)->inode_ptr->head->roll_no=parent_next->down->lnode_ptr->head->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,parent_next->down->lnode_ptr->head->subjcode);
								new_key_leaf->down=parent_next->down->lnode_ptr->smallest;
								rootv->lnode_ptr->head=new_key_leaf;
								parent_next->down->lnode_ptr->smallest=parent_next->down->lnode_ptr->head->down;
								tmp=parent_next->down->lnode_ptr->head;
								parent_next->down->lnode_ptr->head=parent_next->down->lnode_ptr->head->right;
								free(tmp);
								rval=deleted;
							}
							else
							{
								key_sib1=parent_next->down->lnode_ptr->head;
								new_key_leaf=make_key_leaf((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								last->right=new_key_leaf;
								(*parent)->inode_ptr->head->roll_no=key_sib1->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,key_sib1->subjcode);	
								last->right->down=parent_next->down->lnode_ptr->smallest;
								parent_next->down->lnode_ptr->smallest=key_sib1->down;
								(*parent)->inode_ptr->head->down->lnode_ptr->head=key_sib1->right;
								free(key_sib1);
								rval=deleted;
							}	
							}
							
							else if(c2>min-1)
							{
								new_key_leaf=make_key_leaf(parent_ptr->roll_no,parent_ptr->subjcode);
								new_key_leaf->right=parent_ptr->down->lnode_ptr->head;
								new_key_leaf->down=parent_ptr->down->lnode_ptr->smallest;
								parent_ptr->roll_no=kptr->roll_no;
								strcpy(parent_ptr->subjcode,kptr->subjcode);
								prev_last->right=NULL;
								parent_ptr->down->lnode_ptr->smallest=kptr->down;
								//kptr->down->next=new_key_leaf->down;
								//new_key_leaf->down->prev=kptr->down;
								free(kptr);
								rval=deleted;
								if(lptr==NULL)
								{
									parent_ptr->down->lnode_ptr->head=new_key_leaf;
								}
							}
							
							else
							{
								key_sib1=parent_next->down->lnode_ptr->head;
								new_key_leaf=make_key_leaf(parent_next->roll_no,parent_next->subjcode);
								last->right=new_key_leaf;
								new_key_leaf->down=parent_next->down->lnode_ptr->smallest;
								new_key_leaf->right=key_sib1;
								free(parent_next->down);
								parent_ptr->right=parent_next->right;
								free(parent_next);
								rval=merged;
							}							
						}
					}
				}
				else
				{
					rval=deleted;
				}
			}	
		}	
	}
	return(rval);
}


status_code del1(tree** root,tree** parent,int r,char c[])
{
	int flag = 1,count,min,found,key_count,c1,c2;
	status_code rval=deleted;
	tree *rootv,*ptr;

key_list *l_key,*lprev,*lptr,*lhead_ptr,*tmp,*lnode1,*kptr,*kprev,*key_sib1,*key_sib2,*last,*new_key_leaf,*prev_last,*kkptr,*kkptr1, 		         	  *parent_ptr,*parent_prev,*parent_next;
	rootv=*root;


	//printf("Execution control is in del1\n");

	lptr=rootv->inode_ptr->head;
				key_count=0;
				if((rootv->inode_ptr->head)==NULL)
				{
					ptr=rootv;
					*root=rootv->inode_ptr->smallest;
					free(ptr);
				}
				else{
				while(lptr!=NULL)
				{
					key_count=key_count+1;
					last=lptr;
					lptr=lptr->right;
				}
				
				
				if(key_count < min-1)
				{
					parent_ptr=(*parent)->inode_ptr->head;
					parent_prev=NULL;
					if(rootv==(*parent)->inode_ptr->smallest)
					{
						parent_next=(*parent)->inode_ptr->head;
						key_sib1=parent_next->down->inode_ptr->head;
						kptr=key_sib1;
						key_count=0;
						while(kptr!=NULL)
						{
							key_count=key_count+1;
							kptr=kptr->right;
						}
						if(key_count < min-1)
						{
							if(lptr==NULL)
							{
								new_key_leaf=make_key_list((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								new_key_leaf->down=parent_next->down->inode_ptr->smallest;
								new_key_leaf->right=key_sib1;
								rootv->inode_ptr->head=new_key_leaf;
								free(parent_next->down);
								(*parent)->inode_ptr->head=parent_next->right;
								free(parent_next);
								rval=merged;
							}
							else
							{
								new_key_leaf=make_key_list((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								last->right=new_key_leaf;
								new_key_leaf->down=parent_next->down->inode_ptr->smallest;
								new_key_leaf->right=key_sib1;
								free(parent_next->down);
								(*parent)->inode_ptr->head=parent_next->right;
								free(parent_next);
								rval=merged;
							}
						}
						else
						{
							if(lptr==NULL)
							{
								new_key_leaf=make_key_list((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								(*parent)->inode_ptr->head->roll_no=key_sib1->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,key_sib1->subjcode);
								new_key_leaf->down=parent_next->down->inode_ptr->smallest;
								rootv->inode_ptr->head=new_key_leaf;
								parent_next->down->inode_ptr->smallest=key_sib1->down;
								(*parent)->inode_ptr->head->down->inode_ptr->head=key_sib1->right;
								free(key_sib1);
								rval=deleted;
							}
							else
							{
								new_key_leaf=make_key_list((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								last->right=new_key_leaf;
								(*parent)->inode_ptr->head->roll_no=key_sib1->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,key_sib1->subjcode);	
								last->right->down=parent_next->down->inode_ptr->smallest;
								parent_next->down->inode_ptr->smallest=key_sib1->down;
								(*parent)->inode_ptr->head->down->inode_ptr->head=key_sib1->right;
								free(key_sib1);
								rval=deleted;
							}	
						}
					}
					else
					{
						parent_ptr=(*parent)->inode_ptr->head;
						while(parent_ptr->down!=rootv)
						{
							parent_prev=parent_ptr;
							parent_ptr=parent_ptr->right;
						}
						parent_next=parent_ptr->right;
						
						kptr=parent_prev->down->inode_ptr->head;
						key_count=0;
						prev_last=NULL;
							
							while(kptr->right!=NULL)
							{
								key_count=key_count+1;
								prev_last=kptr;
								kptr=kptr->right;
							}
							key_count=key_count+1;
							
						if(parent_next==NULL)
						{
							
							if(key_count < min-1)
							{
								new_key_leaf=make_key_list(parent_ptr->roll_no,parent_ptr->subjcode);
								new_key_leaf->right=parent_ptr->down->inode_ptr->head;
								new_key_leaf->down=parent_ptr->down->inode_ptr->smallest;
								kptr->right=new_key_leaf;
								free(parent_ptr->down);
								free(parent_ptr);
								parent_prev->right=NULL;
								rval=merged;
							}
							else
							{
								new_key_leaf=make_key_list(parent_ptr->roll_no,parent_ptr->subjcode);
								new_key_leaf->right=parent_ptr->down->inode_ptr->head;
								new_key_leaf->down=parent_ptr->down->inode_ptr->smallest;
								parent_ptr->roll_no=kptr->roll_no;
								strcpy(parent_ptr->subjcode,kptr->subjcode);
								prev_last->right=NULL;
								parent_ptr->down->inode_ptr->smallest=kptr->down;
								//kptr->down->next=new_key_leaf->down;
								//new_key_leaf->down->prev=kptr->down;
								free(kptr);
								rval=deleted;
								if(lptr==NULL)
								{
									parent_ptr->down->inode_ptr->head=new_key_leaf;
								}
							}
						}
						else
						{
							c1=c2=0;
							kkptr=parent_next->down->inode_ptr->head;
							kkptr1=parent_prev->down->inode_ptr->head;
							while(kkptr!=NULL || kkptr1!=NULL)
							{
								if(kkptr!=NULL)
								{
									kkptr=kkptr->right;
									c1=c1+1;
								}
								if(kkptr1!=NULL)
								{
									kkptr1=kkptr1->right;
									c2=c2+1;
								}
							}
							
							if(c1>min-1)
							{
								if(lptr==NULL)
							{
								new_key_leaf=make_key_list((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								(*parent)->inode_ptr->head->roll_no=parent_next->down->inode_ptr->head->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,parent_next->down->inode_ptr->head->subjcode);
								new_key_leaf->down=parent_next->down->inode_ptr->smallest;
								rootv->inode_ptr->head=new_key_leaf;
								parent_next->down->inode_ptr->smallest=parent_next->down->inode_ptr->head->down;
								tmp=parent_next->down->inode_ptr->head;
								parent_next->down->inode_ptr->head=parent_next->down->inode_ptr->head->right;
								free(tmp);
								rval=deleted;
							}
							else
							{
								key_sib1=parent_next->down->inode_ptr->head;
								new_key_leaf=make_key_list((*parent)->inode_ptr->head->roll_no,(*parent)->inode_ptr->head->subjcode);
								last->right=new_key_leaf;
								(*parent)->inode_ptr->head->roll_no=key_sib1->roll_no;
								strcpy((*parent)->inode_ptr->head->subjcode,key_sib1->subjcode);	
								last->right->down=parent_next->down->inode_ptr->smallest;
								parent_next->down->inode_ptr->smallest=key_sib1->down;
								(*parent)->inode_ptr->head->down->inode_ptr->head=key_sib1->right;
								free(key_sib1);
								rval=deleted;
							}	
							}
							
							else if(c2>min-1)
							{
								new_key_leaf=make_key_list(parent_ptr->roll_no,parent_ptr->subjcode);
								new_key_leaf->right=parent_ptr->down->inode_ptr->head;
								new_key_leaf->down=parent_ptr->down->inode_ptr->smallest;
								parent_ptr->roll_no=kptr->roll_no;
								strcpy(parent_ptr->subjcode,kptr->subjcode);
								prev_last->right=NULL;
								parent_ptr->down->inode_ptr->smallest=kptr->down;
								//kptr->down->next=new_key_leaf->down;
								//new_key_leaf->down->prev=kptr->down;
								free(kptr);
								rval=deleted;
								if(lptr==NULL)
								{
									parent_ptr->down->inode_ptr->head=new_key_leaf;
								}
							}
							
							else
							{
								key_sib1=parent_next->down->inode_ptr->head;
								new_key_leaf=make_key_list(parent_next->roll_no,parent_next->subjcode);
								last->right=new_key_leaf;
								new_key_leaf->down=parent_next->down->inode_ptr->smallest;
								new_key_leaf->right=key_sib1;
								free(parent_next->down);
								parent_ptr->right=parent_next->right;
								free(parent_next);
								rval=merged;
							}							
						}
					}

		}
	}	
		return(rval);
}






key_leaf* make_key_leaf(int r,char c[])
{
	key_leaf *k;
	k=(key_leaf*)malloc(sizeof(key_leaf));
	k->roll_no=r;
	strcpy(c,k->subjcode);
	k->down=NULL;
	k->right=NULL;
	return(k);
}


key_list* make_key_list(int r,char c[])
{
	key_list *k;
	k=(key_list*)malloc(sizeof(key_list));
	k->roll_no=r;
	strcpy(c,k->subjcode);
	k->down=NULL;
	k->right=NULL;
	return(k);
}


/*void display_tree(tree *rootv)
{
	key_list *kptr;
	key_leaf *lptr;
	
	while(rootv->inode_ptr->type!='l')
	{
		kptr=rootv->inode_ptr->head;
		while(kptr!=NULL)
		{
			printf("[%d %s]  ",kptr->roll_no,kptr->subjcode);
			kptr=kptr->right;
		}
		rootv=rootv
	}
}
*/












