/* curver.c - takes in a bh1 file and subdivides it one level.*/

#include <stdio.h>
#include <stdlib.h>
#include "shape.h"

int V(Shape s, int i)
{
	return s->tTable[i];
}


int L(int i,int* oTable)
{
	return oTable[P(i)];
}

int R(int i,int *oTable)
{
	return oTable[N(i)];
}

int N(int i)
{
	i++;
	if(i % 3 == 0)
		i-= 3;
	return i;
}

int P(int i)
{
	return N(N(i));
}

int main(int argc, char** argv)
{
   Shape inShape = NULL;
   Shape outShape= NULL;
   int i,c;

   int *oTable, *mTable;
   float *vTable;
   float *mVTable;
   int counter;

   if(argc != 3)
   {
	printf("Usage:\n\t%s inFile outFile.\n",argv[0]);
 	return 1;
   }

   inShape = readShapeFromFile(argv[1]);

   if(inShape == NULL)
	return 1;
 
   printf("Let's see what's in the inShape!\n");

   printf("There are %d vertices.\n",inShape->numVertices);
   printf("The vTable contains:\n");
   for(c=0;c<inShape->numVertices*3;c+=3)
   {
	   printf("%d => %f %f %f\n",c,inShape->vTable[c],
			   inShape->vTable[c+1],
			   inShape->vTable[c+2]);
   }
   printf("The tTable contains:\n");
   for(c=0;c<numTriangles(inShape)*3;c+=3)
   {
	   printf("%d => %d %d %d\n", c,inShape->tTable[c],
			   inShape->tTable[c+1],
			   inShape->tTable[c+2]);
   }

   oTable = malloc(sizeof(int)*inShape->numVertices*3);
   for(c=0;c<inShape->numVertices*3;c++)
   {
	   oTable[c] = -1;
   }

   for(c=0;c<inShape->numVertices*3;c++)
	   for(i=c+1;i<inShape->numVertices*3;i++)
	   {
		   if((V(inShape,N(c)) == V(inShape,P(i)))
 		    &&(V(inShape,P(c)) == V(inShape,N(i))))
		   {
			   oTable[c] = i;
			   oTable[i] = c;
		   }
	   }

   printf("This is the oTable:\n");
   for(c=0;c<inShape->numVertices*3;c++)
   {
	   printf("%d => %d\n",c,oTable[c]);
   }
   
   mTable = malloc(sizeof(int)*inShape->numVertices*3);
   for(c=0;c<inShape->numVertices*3;c++)
	   mTable[c] = -1;

   counter = 0;

   outShape = createShape(4*inShape->numVertices-6);

   for(c=0;c<inShape->numVertices*3;c++)
   {
	   outShape->vTable[c] = inShape->vTable[c];
   }

   counter = inShape->numVertices;

   printf("Counter starts at: %d\n",counter);

   vTable = outShape->vTable;
   for(c=0;c<inShape->numVertices*3;c++)
   {
	if(mTable[c] == -1)
	{
/*		outShape->vTable[counter*3]=
			(vTable[3*V(inShape,N(c))]+
			vTable[3*V(inShape,P(c))])/2
			+
			(
			 ( vTable[3*V(inShape,c)] +
			   vTable[3*V(inShape,oTable[c])]
			 )/2
			 - 
			 (
			  vTable[3*L(c,oTable)] + vTable[3*R(c,oTable)]
			 +vTable[3*L(oTable[c],oTable)] +
			  vTable[3*R(oTable[c],oTable)]
		         )/4	
			 
			 )/4;
		outShape->vTable[counter*3+1]=
			(vTable[3*(V(inShape,N(c)))+1]+
			 vTable[3*(V(inShape,P(c)))+1])/2
			+
			(
			 ( vTable[3*(V(inShape,c))+1] +
			   vTable[3*(V(inShape,oTable[c]))+1]
			 )/2
			 - 
			 (
			  vTable[3*(L(c,oTable))+1] + vTable[3*(R(c,oTable))+1]
			 +vTable[3*(L(oTable[c],oTable))+1] +
			  vTable[3*(R(oTable[c],oTable))+1]
		         )/4	
			 
			 )/4;
		outShape->vTable[counter*3+2]=
			(vTable[3*(V(inShape,N(c)))+2]+
			 vTable[3*(V(inShape,P(c)))+2])/2
			+
			(
			 ( vTable[3*(V(inShape,c))+2] +
			   vTable[3*(V(inShape,oTable[c]))+2]
			 )/2
			 - 
			 (
			  vTable[3*(L(c,oTable))+2] + vTable[3*(R(c,oTable))+2]
			 +vTable[3*(L(oTable[c],oTable))+2] +
			  vTable[3*(R(oTable[c],oTable))+2]
		         )/4	
			 
			 )/4;*/
		outShape->vTable[counter*3] =
			(vTable[3*V(inShape,N(c))] + vTable[3*V(inShape,P(c))])/2;
		outShape->vTable[counter*3+1] =
			(vTable[3*(V(inShape,N(c)))+1] + vTable[3*(V(inShape,P(c)))+1])/2;
		outShape->vTable[counter*3+2] =
			(vTable[3*(V(inShape,N(c)))+2] + vTable[3*(V(inShape,P(c)))+2])/2;

		printf("Created new point %d at %f %f %f from %d, %d, and %d\n",
				counter,vTable[counter*3],
				vTable[counter*3+1],
				vTable[counter*3+2],
				V(inShape,c),V(inShape,N(c)),V(inShape,P(c)));
		mTable[c] = counter;
		mTable[oTable[c]] = counter;
		counter++;
	}
   }

   printf("mTable:\n");
   for(c=0;c<inShape->numVertices*3;c++)
	   printf("\t%d => %d\n",c,mTable[c]);

   printf("new vTable:\n");
   for(c=0;c<outShape->numVertices;c++)
   	  printf("\t%d => %f %f %f\n",c,outShape->vTable[c*3],
			  		outShape->vTable[c*3+1],
					outShape->vTable[c*3+2]);
   printf("Counter finishes at: %d\n",counter);
   
   for(c=0;c<numTriangles(inShape)*3;c+=3)
   {
	printf("interating through %d...\n",c);
	outShape->tTable[c*4] = V(inShape,c);
	outShape->tTable[c*4+1]=mTable[c+2];
	outShape->tTable[c*4+2]=mTable[c+1];

	outShape->tTable[c*4+3]=mTable[c+1];
	outShape->tTable[c*4+4]=mTable[c];
	outShape->tTable[c*4+5]=V(inShape,c+2);

	outShape->tTable[c*4+6]=mTable[c+2];
	outShape->tTable[c*4+7]=mTable[c];
	outShape->tTable[c*4+8]=V(inShape,c+1);

	outShape->tTable[c*4+9]=mTable[c];
	outShape->tTable[c*4+10]=mTable[c+1];
	outShape->tTable[c*4+11]=mTable[c+2];
  }
   
  printf("new tTable:\n");
  for(c=0;c<numTriangles(outShape);c++)
  {
	  printf("%d => %d %d %d\n",c,outShape->tTable[c*3],
			  	      outShape->tTable[c*3+1],
				      outShape->tTable[c*3+2]);
  }
  writeShapeToFile(outShape,argv[2]);
   return 0;
} 
