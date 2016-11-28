#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int count=2;
int fitnessArr[10];
float probabiltyArr[10];
float cumalativeprobablityArr[10];


//node
struct node
{
	int id;
	int class;
	int age;
	int sex;
	int priority;
	int fitness;
	float probabliy;
	struct node *next;
};


//calculates the cumlaive array for roullete wheel selection
void doCalculateCumalativeArr()
{
	int i=0;
	float t=0;
	for(i=0;i<=9;i++)
	{
		t=t+probabiltyArr[i];
		cumalativeprobablityArr[i]=t;
	}
}

//create Node
struct node* createNode(int class,int age,int sex,int fitness,int priority,int id)
{
	struct node *temp=(struct node* ) malloc(sizeof(struct node));
	temp->class=class;
	temp->id=id;
	temp->age=age;
	temp->sex=sex;
	temp->fitness=fitness;
	temp->priority=priority;
	temp->probabliy=0;
	temp->next=NULL;
	return temp;
}


//make the linked list of node
struct node* insert(struct node *node,int class,int age,int sex,int fitness,int priority)
{

	if(node==NULL)
		return createNode(class,age,sex,fitness,priority,1);
	else
	{
		struct node *temp=node;
		struct node *new=createNode(class,age,sex,fitness,priority,count);
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
		temp->next=new;
		count++;
		return node;
	}
}


// returns the fitness value 
int calcFitness(int class,int age,int sex,int priority)
{
	int t=((( class * age ) * sex) * priority);
	return t;
}


//Compare two strings
int StringCompare(char *x,char *y)
{
	while(*x == *y)
	{
		if (*x == '\0' || *y == '\0')
			break;
		x++;
		y++;
	}
	if(*x == '\0' && *y == '\0')
		return 1;
	else 
		return 0;
}


// create the global Linked list from file
struct node* createList(FILE *f,struct node *node)
{
	char *x="1st",*y="2nd",*z="3rd",*c="crew",*ch="child",*a="adult",*female="female",*m="male";
	int flag=0;
	int fitness=0;
	struct node *head=NULL;
	char str[99];
	int class=0;
	int age=0;
	int sex=0;
	if (f)
	{
		while(fscanf(f,"%[^\n]\n",str)!=EOF)
		{
			char *p=strtok(str,"  ");
			flag=StringCompare(p,x);
			if(flag==1)
			{
				class=15;
				flag=0;
			}
			flag=StringCompare(p,y);
			if(flag==1)
			{
				class=12;
				flag=0;
			}
			flag=StringCompare(p,z);
			if(flag==1)
			{
				class=10;
				flag=0;
			}
			flag=StringCompare(p,c);
			if(flag==1)
			{
				class=8;
				flag=0;
			}			
			p=strtok(NULL,"  ");
			flag=StringCompare(p,ch);
			if(flag==1)
			{
				age=13;
				flag=0;
			}
			flag=StringCompare(p,a);
			if(flag==1)
			{
				age=9;
				flag=0;
			}
			p=strtok(NULL,"  ");
			flag=StringCompare(p,female);
			if(flag==1)
			{
				sex=10;
				flag=0;
			}
			flag=StringCompare(p,m);
			if(flag==1)
			{
				sex=4;
				flag=0;
			}
			int r = rand() % 10;
			if(r==0)
				r=1;
			fitness=calcFitness(class,age,sex,r);
			head=insert(head,class,age,sex,fitness,r);	
		}
		fclose(f);
	}
	return head;
}

//prints the array
void printArr(int arr[],int n)
{
	int i=0;
	for(i=0;i<=n-1;i++)
	{
		printf(" %d ",arr[i]);
	}
}

//print the whole list
void printList(struct node *node)
{
	struct node *temp=node;
	while(temp!=NULL)
	{
		printf("{id==%d & class==%d & age==%d & sex==%d & priority==%d & fitness==%d & probabliy=%f }\n",temp->id,temp->class,temp->age,temp->sex,temp->priority,temp->fitness,(temp->probabliy)*10000000);
		temp=temp->next;
	}
	printf("*** END ***\n");
}

//prints the whole node
void printNode(struct node *temp)
{
	printf("{id==%d & class==%d & age==%d & sex==%d & priority==%d & fitness==%d & probabilty=%f }\n",temp->id,temp->class,temp->age,temp->sex,temp->priority,temp->fitness,temp->probabliy);
}
/*
//return the maximum fitness node
struct node* retMax(struct node *node)
{
	if(node==NULL)
		return node;
	struct node *temp=node;
	int i=node->fitness;
	struct node *rettemp=NULL;
	while(temp!=NULL)
	{
		if(temp->fitness > i)
		{
			i=temp->fitness;
			rettemp=temp;
		}
		temp=temp->next;
	}
	return rettemp;
}*/

//prints the list in file
FILE* printFile(struct node *node,FILE *f)
{
	struct node *temp=node;
	if (f==NULL)
	{
		printf("Error!");
	}
	else
	{	
		while(temp!=NULL)
		{
			fprintf(f,"{id==%d & class==%d & age==%d & sex==%d & priority==%d & fitness==%d & probabliy=%f }\n",temp->id,temp->class,temp->age,temp->sex,temp->priority,temp->fitness,(temp->probabliy)*10000000);			
			temp=temp->next;
		}
	}
	return f;
}

//calclulates the probabilty
struct node* calcProbabliy(struct node *node)
{
	struct node *temp1=node;
	struct node *temp2=node;
	int total=0;
	while(temp1!=NULL)
	{
		total=total+temp1->fitness;
		temp1=temp1->next;
	}
	printf("total==%d\n",total);
	while(temp2!=NULL)
	{

		float prob=( (float)temp2->fitness / (float)total );
		temp2->probabliy=prob;
		temp2=temp2->next;

	}
    return node;
}

//finds the probablity system
float probabiltySum(struct node *node)
{
	struct node *temp=node;
	float prob=0;
	while(temp!=NULL)
	{
		prob=prob+temp->probabliy;
		temp=temp->next;
	}
	return prob;
}

//swap two numbers
void swap(int *a,int *b)
{
	int temp=*a;
	*a=*b;
	*b=temp;
}



//genrate the bit string
void createBitString(struct node *node,int arr[16])
{
	struct node *temp=node;
	int qt=0,i=0,temparr[4];
	int class=temp->class;
	int age=temp->age;
	int sex=temp->sex;
	int priority=temp->priority;
	//printf("class = %d age = %d sex = %d priority = %d \n",class,age,sex,priority);
	qt=class;
	while(qt!=0)
	{
		arr[i++]=qt % 2;
		qt = qt / 2;
	}
	swap(&arr[0],&arr[3]);
	swap(&arr[1],&arr[2]);
	qt=age;
	while(qt!=0)
	{
		arr[i++]=qt % 2;
		qt = qt / 2;
	}
	swap(&arr[4],&arr[7]);
	swap(&arr[5],&arr[6]);
	qt=sex;
	while(qt!=0)
	{
		arr[i++]=qt % 2;
		qt = qt / 2;
	}
	swap(&arr[8],&arr[11]);
	swap(&arr[9],&arr[10]);
	qt=temp->priority;
	i++;
	while(qt!=0)
	{
		//printf("i==%d\n",i);
		arr[i++]=qt % 2;
		//printf("qt 2== %d \n",qt % 2);
		qt = qt / 2;
	}
	swap(&arr[13],&arr[14]);
	swap(&arr[12],&arr[15]);
} 


// calculates the fitness for child
int calcFitness2(int arr[16])
{
	int i=0,class=0,age=0,sex=0,priority=0;
	class=(1*arr[3]) + (2*arr[2]) + (4*arr[1]) +(8*arr[0]);
	age=(1*arr[7]) + (2*arr[6]) + (4*arr[5]) +(8*arr[4]);
	sex=(1*arr[11]) + (2*arr[10]) + (4*arr[9]) +(8*arr[8]);
	priority=(1*arr[15]) + (2*arr[14]) + (4*arr[13]) +(8*arr[12]);

	int fitness=calcFitness(class,age,sex,priority);

	return fitness; 
}

//crossover 
void crossover(int a[16],int b[16],int child[16])
{
	//srand(time(NULL));
	//srand(time(0));
	int r=rand() % 15;
	//printf("crossover random number==%d\n",r);
	int child1[16]={0};
	int child2[16]={0};
	int i=0;
	for(i=0;i<=15;i++)
	{
		child1[i]=a[i];
		child2[i]=b[i];
	}

	for(i=r;i<=15;i++)
	{
		child1[i]=b[i];
	}

	for(i=r;i<=15;i++)
	{
		child2[i]=a[i];
	}

	int fitness1=calcFitness2(child1);
	int fitness2=calcFitness2(child2);

	if(fitness1 >= fitness2)
		 {
		 	for(i=0;i<=15;i++)
		 		child[i]=child1[i];
		 }
	else
		{
			for(i=0;i<=15;i++)
		 		child[i]=child2[i];
		}
}


//mutation
void mutation(int a[15])
{
	//srand(time(NULL));

	int r=rand() % 2;

	//printf("mutation random value==%d\n",r);
	if (r==1)
	{
		//printf("mutation is done\n");
		//srand(time(NULL));
		//srand(time(0));
		int r2=rand() % 15;
		//printf("r2 random number==%d\n",r2);
		if(a[r2]==1)
			a[r2]=0;
		else
			a[r2]=1;
	}
	else
	{	//printf("no mutation\n");
	}
}

// genitic algo
void doGenetics(struct node *node,int populationSize,int iteration,int bitStringLength)
{
	int population[populationSize][16];
	int totalPopulation=2201;
	int i=0,j=0;
	for(i=0;i<=populationSize-1;i++)
	{
		for(j=0;j<=bitStringLength-1;j++)
		{
			population[i][j]=0; // make the population all 0 intially
		}
	}

	// this loop will select 10 random people from the group OF 2201
	for(i=0;i<=populationSize-1;i++)
	{
		struct node *temp=node;
		//srand(time(NULL));
		int random = rand() % totalPopulation;
		totalPopulation--;
		//printf("random numnber == %d\n",random);
		for(j=0;j<=random-3;j++)
		{
			temp=temp->next;
		}
		//printf("reached 2 temp -> id==%d \n",temp->id);
		int arr[16]={0};
		createBitString(temp->next,arr);
		for(j=0;j<=bitStringLength-1;j++)
		{
			population[i][j]=arr[j];
		}
		struct node *del=temp->next;
		temp->next=del->next;
		free(del);
	}
	int x=0,v=0;
	FILE *f=fopen("fitness.txt","w");
	for(x=0;x<=iteration;x++)
	{
		// printing the pupulation bit string and their fitness
		printf("Genration no=== %d \n",x);
		for(i=0;i<=populationSize-1;i++)
		{
			for(j=0;j<=bitStringLength-1;j++)
			{
				printf(" %d ",population[i][j]);
			}
			int fitness=calcFitness2(population[i]);
			printf(" fitness == %d",fitness);
			fitnessArr[i]=fitness;
			printf("\n");
		}

		int c=0;
		for(i=0;i<=9;i++)
		{
			c=c+fitnessArr[i];
		}

		printf("total fitness == %d\n",c);



		 if (f==NULL)
		 {
		 	printf("Error!");
		 }

		else
		{
			fprintf(f, "%d\t%d\n",x,c);
		}
		int t=0;
		for(i=0;i<=populationSize-1;i++)
		{
			t=t+fitnessArr[i]; // calulating the total fitness for calclulating probablity
		}

		//
		for(i=0;i<populationSize-1;i++)
		{
			float p=fitnessArr[i] / (float)t;
			probabiltyArr[i]=p; // calulating probablity  array 
		}

		printArr(fitnessArr,10);
		printf("\n");
	

		//srand(time(NULL));
		int r=rand()%100000;
		float ran=(float)r/100000; // random number between 0.000000 to 0.999999 
		//printf("random number==%f\n",ran);
		i=0;

		// bubble sorting the probablity array
		for(i=0;i<=9;i++)
		{
			for(j=i;j<=9;j++)
			{
				if(probabiltyArr[i] > probabiltyArr[j])
				{
					float t=probabiltyArr[i];
					probabiltyArr[i]=probabiltyArr[j];
					probabiltyArr[j]=t;
				}
			}
		}


		//calclulates the cumlaive probablity for roullete wheel selection
		doCalculateCumalativeArr(); 
		/*for(i=0;i<populationSize-1;i++)
		{
			printf("%f",cumalativeprobablityArr[i]);
		}*/

		//printf("\n..........\n%f\n.......\n ",ran);
	
    	
		i=0;
		int num=0;
		for(i=0;i<=9;i++)
		{
			if(ran < cumalativeprobablityArr[i])
			{
				//printf("probabiltyArr[%d] == %f\n",i,probabiltyArr[i]);	// selection of roullette wheel 
				num=i;
				break;	
			}
		}

	 	//printf("probabiltyArr[%d] == %f\n",num,probabiltyArr[num]);

		int child1[16]={0};
		int child2[16]={0};
		crossover(population[num],population[num+1],child1);
		mutation(child1);
		crossover(population[num-1],population[num],child2);
		mutation(child2);
		//printf("child==");
		//printArr(child1,16);
		//printf("\nchild2==");
		//printArr(child2,16);


		int f1=calcFitness2(population[num]);
		int f2=calcFitness2(population[num+1]);

		int fc1=calcFitness2(child1);
		int fc2=calcFitness2(child2);


		//printf("\n f1==%d , f2==%d , fc1==%d , fc2==%d\n",f1,f2,fc1,fc2);

		if(v==9)
 			v=0;

		if(fc1 > fc2 && fc1 > 0 && fc1 < 13000)
		{
			//printf("child1 is selected\n");
			for(i=0;i<=15;i++)
				population[v][i]=child1[i];
		}

		if(fc2 > fc1 && fc2 > 0 && fc2 < 13000)
		{
			//printf("child2 is selected\n");
			for(i=0;i<=15;i++)
				population[v][i]=child2[i];	
		}
		
		v++;
	}

}





//main
int main()
{
	srand(time(NULL));
	FILE *f=fopen("titanic.txt","r");
	FILE *f2=fopen("out.txt","w");
	struct node *head=NULL;
	head=createList(f,head);
	head=calcProbabliy(head);
	//printList(head);
	/*struct node *max=retMax(head);
	printNode(max);*/
	int Population=10;

	doGenetics(head,10,500,16);
	/*float pro=probabiltySum(head);
	printf("probabilty == %f \n",pro);
	printFile(head,f2);
	double r=r2();
	printf("rand numnber=== %lf\n",r);*/
}