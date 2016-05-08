#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

class node
{
    public:
    int blockNo;
    double width,height,power;
    struct node *left;
    struct node *right;

}*ROOT;

void insertNode(fpBlock B)
{
    /* create a B*tree node with following properties of floor plan block */
    node *newNode = new node;
    newNode->blockNo = B->blockNo;
    newNode->width = B->width;
    newNode->height = B->height;
    newNode->power = B->power;

    newNode->left = newNode->right = NULL;
}



void deleteNode()
{
    /* We have three cases if its 1. Leaf node 2. Node with one child 3. Node with two child */

}




#endif // BTREE_H_INCLUDED
