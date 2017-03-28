#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
    if (this->count < this->leafSize)
    {
        this->values[count] = value;
        this->count++;
        this->sortarray();
        return NULL;
    }
    else
    {
        if (this->leftSibling != NULL && (this->leftSibling->getCount() < this->leafSize))
        {
            if (this->getMinimum() < value)
            {
                int temp = this->getMinimum();
                this->values[0] = value;
                this->sortarray();
                value = temp;
                this->leftSibling->insert(value);
                return NULL;
            }
            return NULL;//this->leftSibling;
        }    // put value in herre
        else if (this->rightSibling != NULL && (this->rightSibling->getCount() < this->leafSize))
        {
            this->rightSibling->insert(this->values[this->leafSize - 1]);//this->rightSibling;
            this->values[this->leafSize - 1] = value;
            this->sortarray();
        }    //put value in here
        else
        {
            return this->split(value);
        }
        /*    if (this->parent == NULL)
                this->parent = new InternalNode(this->);
            //InternalNode->insert(this,this->rightSibling);

            // if this->parent is
            //null make new internal node
            //and set both rightsibling and
            //this to be connected to that and make internal root
        }*/
    }

  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

LeafNode* LeafNode::split(int value)
{
    int max = value;
    int temp;
    int moving = 0;
    this->rightSibling = new LeafNode(this->leafSize, this->parent, this, this->rightSibling);
    if (this->rightSibling->getRightSibling() != NULL)
        (this->rightSibling->getRightSibling())->setLeftSibling(this->rightSibling);
    if (max > this->values[getCount() - 1])
        max = value;
    else
    {
        temp = this->values[getCount() - 1];
        this->values[getCount() - 1] = max;
        this->sortarray();
        max = temp;
    }
    if(this->getCount() % 2 == 0)
        moving = this->getCount()/2;
    else
        moving = (this->getCount() - 1)/2;
    this->rightSibling->insert(max);
    for(int i = 0; i < moving; i++)
    {
        this->rightSibling->insert(this->values[getCount() - 1]);
        this->count = this->count - 1;
    }
    return ((LeafNode*)this->rightSibling);
}

void LeafNode::sortarray()
{
     int i, j, flag = 1;    // set flag to 1 to start first pass
      int temp;             // holding variable
      int numLength = this->getCount();
      for(i = 1; (i <= numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength - 1); j++)
         {
               if (this->values[j+1] < this->values[j])      // ascending order simply changes to <
              {
                    temp = this->values[j];             // swap elements
                    this->values[j] = this->values[j+1];
                   this->values[j+1] = temp;
                    flag = 1;               // indicates that a swap occurred.
               }
          }
     }
}

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


