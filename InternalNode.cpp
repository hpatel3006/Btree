#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
for (int i = 1; keys[i] != NULL; i++)
    if (value < keys[i])
    {
	if (this->children[i - 1]->insert(value) != NULL)
        {
            this->insert(this->children[i - 1]->getRightSibling());
            return NULL;
        }
        else
        {
            this->updatekeys();
            return NULL;
        }
    }
    else
        continue;
if (value > keys[this->getCount() - 1])
    if (this->children[this->getCount() - 1]->insert(value) != NULL)
    {
            if (this->insert(this->children[this->getCount() - 1]->getRightSibling()) != NULL)
                return (InternalNode*)this->getRightSibling();
    }
    // possibly need to check to see if n is full, else assumed last key insert here
    
// insert at the last sibling possible means bigger than any current key...
  // students must write this
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
    this->keys[this->count] = oldRoot->getMinimum();
    this->children[this->count] = oldRoot;
    this->children[this->count]->setParent(this);
    this->count++;
    this->keys[this->count] = node2->getMinimum();
    this->children[this->count] = node2;
    this->children[this->count]->setParent(this);
    this->count++;
    this->updatekeys();
} // InternalNode::insert()

InternalNode* InternalNode::insert(BTreeNode *newNode) // from a sibling
{
   if (this->count < this->internalSize)
   {
       this->keys[count] = newNode->getMinimum();
       //change what child points to what key
      // while //this key does not equal the minimum of what we just placed
               // continue
               // once it does set this index to be the child[index]
               // for everything left in the count increment index by 1
       this->children[this->count] = newNode;
       this->count++;   
       this->updatekeys();
   }
   else
       if (this->split(newNode) != NULL)
         return (InternalNode*)this->getRightSibling();// (InternalNode*(this->getRightSibling())->parent)->insert(this, this->getRightSibling()); // update root
   //(((InternalNode*)(this->getRightSibling()->parent))->insert(this, this->getRightSibling()));
} // InternalNode::insert()

InternalNode* InternalNode::split(BTreeNode *node)
{
    int max = node->getMinimum();
    BTreeNode* maxnode = node;
    int tempkey;
    int moving = 0;
    this->rightSibling = new InternalNode(this->internalSize, this->leafSize, this->parent, this->leftSibling, this->rightSibling);
    if (this->rightSibling->getRightSibling() != NULL)
        (this->rightSibling->getRightSibling())->setLeftSibling(this->rightSibling);
    if (max > this->keys[getCount() - 1])
    {//
        max = node->getMinimum();
        maxnode = node;
    }   
    else
    {
        tempkey = this->keys[getCount() - 1];
        BTreeNode* tempnode = this->children[getCount() - 1]; 
        this->keys[getCount() - 1] = max;
        this->children[getCount() - 1] = node;
        this->updatekeys();
        max = tempkey;
        BTreeNode* maxnode = tempnode;
    }
    if(this->getCount() % 2 == 0)
        moving = this->getCount()/2;
    else
        moving = (this->getCount() - 1)/2;
    ((InternalNode*)(this->getRightSibling()))->insert(maxnode); // insert into right internal node the max leafnode
    for(int i = 0; i < moving; i++)
    {
        ((InternalNode*)this->rightSibling)->insert(this->children[getCount() - 1]);
        this->count = this->count - 1;
        this->keys[getCount()] = 0;
        this->updatekeys();
    }
    if ((this->rightSibling != NULL || this->leftSibling != NULL) && this->parent == NULL)
    {
      //InternalNode* newroot = new InternalNode(this->internalSize, this->leafSize, NULL, NULL, NULL);  
      //newroot->insert(this, this->getRightSibling());
      return ((InternalNode*)(this->getRightSibling()));
    }
    else
        return NULL;
    //return ((InternalNode*)this->rightSibling);
}

void InternalNode::updatekeys()
{
    for (int i = 0; i < this->count; i++)
    {
        this->keys[i] = this->children[i]->getMinimum();
    }
    this->sortkeys();
    int index = 0;
    for (BTreeNode* ptr = this->children[0]; ptr != NULL; ptr = ptr->getRightSibling())
    {
        this->children[index] = ptr;
        index = index + 1;
    }
}



void InternalNode::sortkeys()
{
      int i, j, flag = 1;    // set flag to 1 to start first pass
      int temp;             // holding variable
      int numLength = this->getCount(); 
      for(i = 0; (i < numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength -1); j++)
         {
               if (this->keys[j+1] < this->keys[j])      // ascending order simply changes to <
              { 
                    temp = this->keys[j];             // swap elements
                    this->keys[j] = this->keys[j+1];
                    this->keys[j+1] = temp;
                    flag = 1;               // indicates that a swap occurred.
               }
          }
     }
     //return;   //arrays are passed to functions by address; nothing is returned
}
    /* int i, j, flag = 1;    // set flag to 1 to start first pass
      int temp;             // holding variable
      int numLength = this->getCount();
      for(i = 1; (i <= numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength - 1); j++)
         {
               if (this->keys[j+1] < this->keys[j])      // ascending order simply changes to <
              {
                    temp = this->keys[j];             // swap elements
                    this->keys[j] = this->keys[j+1];
                   this->keys[j+1] = temp;
                    flag = 1;               // indicates that a swap occurred.
               }
          }
     } */
    
void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


