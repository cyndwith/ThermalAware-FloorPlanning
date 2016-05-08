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

void insertNode();

void deleteNode();




#endif // BTREE_H_INCLUDED
