#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "b+tree.h"

int main()
{
	bool x;
	tree *root,*r;
	key_list *ans;
	int roll_no,marks,sem_no,a=1,num_records,h,r1,r2,p;
	char name[max_name],dep_name[max_depnm],subjcode[max_code],c1[max_code],c2[max_code];
	root=NULL;
	printf("1.  Insert\n");
	printf("2.  Print\n");
	printf("3.  Search\n");
	printf("4.  GetNumRecords\n");
	printf("5.  height\n");
	printf("6.  Range Search\n");
	printf("7.  GetMax\n");
	printf("8.  GetNumFailures\n");
	printf("9.  GetTopper\n");
	printf("10. GetKthHighest\n");
	printf("11. Delete\n\n\n");
	while(a!=0)
	{
		printf("\nEnter value of a : ");
		scanf("%d",&a);
		switch(a)
		{
		case 1:
		printf("\nEnter the data\n");
		printf("\nroll-no : ");
		scanf("%d",&roll_no);
		printf("\nstudent_name : ");
		scanf("%s",name);
		printf("\nsubject code : ");
		scanf("%s",subjcode);
		printf("\nmarks : ");
		scanf("%d",&marks);
		printf("\ndepartment name : ");
		scanf("%s",dep_name);
		printf("\nsemester number : ");
		scanf("%d",&sem_no);
		ans=insert(&root,roll_no,name,subjcode,marks,dep_name,sem_no);
		if(ans==NULL)
			{
				printf("\nInsertion SUCCESSFUL");
			}
		else
		{
			r=(tree*)malloc(sizeof(tree));
			r->inode_ptr=(inner_node*)malloc(sizeof(inner_node));
			r->inode_ptr->type='i';
			r->inode_ptr->head=ans;
			r->inode_ptr->smallest=root;
			root=r;
		}
		break;
		case 2:
				print_data(root);
				break;
		case 3:
				printf("Enter the key values\n");
				printf("roll-no : ");
				scanf("%d",&roll_no);
				printf("Subject Code : ");
				scanf("%s",subjcode);
				x=Search(root,roll_no,subjcode);	
				if(x==false)
				{
					printf("Data entry not found\n");
				}
				break;
		case 4:
			num_records=getNumRecords(root);
			if(num_records != 0)
			{
				printf("Total number of records is: %d\n",num_records);
			}	
			break;	
		case 5:
			h=height(root);
			if(h==0)
			{
				printf("tree is empty\n");
			}	
			else
			{
				printf("Height of tree is : %d\n",h);
			}
			break;
		case 6:
			printf("Enter two keys for Range Search\n");
			printf("Roll-no: ");
			scanf("%d",&r1);
			printf("Subject: ");
			scanf("%s",c1);
			printf("Roll-no: ");
			scanf("%d",&r2);
			printf("Subject: ");
			scanf("%s",c2);
			Range_search(root,r1,c1,r2,c2);	
			break;
		case 7:
			printf("Enter Subject code\n");
			scanf("%s",c1);	
			Getmax(root,c1);
			break;
			
		case 8:
			printf("Enter Subject code\n");
			scanf("%s",c1);	
			printf("Enter the passing marks\n");
			scanf("%d",&p);
			GetListofFailures(root,c1,p);
			break;
			
		case 9:
			
			GetTopper(root);
			break;	
		
		case 10:
			printf("Enter Subject code\n");
			scanf("%s",c1);	
			printf("Enter the value of k\n");
			scanf("%d",&p);
			GetKthHighest(root,c1,p);
			break;	
		case 11:
			printf("Enter the key values\n");
			printf("Roll-no: ");
			scanf("%d",&r1);
			printf("Subject: ");
			scanf("%s",c1);
			delete(&root,r1,c1);
			break;
				
		case 0:
			break;	
		}		
	}
	return(0);
}
