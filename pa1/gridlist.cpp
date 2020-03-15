// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"

// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.

PNG GridList::Render() const
{
  //assign b to data of northwest
  Block b = northwest->data;
  //initialize the size of the image 
  PNG image = PNG(dimx * b.Dimension(), dimy * b.Dimension()); 
  //create a pointer that points to northwest
  GridNode * pointer = northwest;
  //render each block of the image
  for (int y = 0; y < dimy; y++){
    for (int x = 0; x < dimx; x++){
      pointer->data.Render(image, y*b.Dimension(), x*b.Dimension());
      pointer = pointer->next;
    }
  }
  return image;
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata)
{
  GridNode *newNode = new GridNode(bdata);
  GridNode *pointer;

  if(!IsEmpty()){
    //pointer points to n
    pointer = southeast;

    //southeast points to n+1
    southeast = newNode;

    //n.next points to n+1
    (*pointer).next = newNode;

    //(n+1).prev points to n
    (*southeast).prev = pointer; 
  }
  else{
    //southeast points to newNode
    southeast = newNode;

    //northwest points to newNode
    northwest = newNode;
  }
}

// if this list has an odd number of column blocks, then the right side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList& inner)
{
  //this list and inner list are the same
  if(inner.northwest == this->northwest ){
    return;
  }
  //this list has less then 2 column blocks
  else if(this->dimx < 2){
    return;
  }
  //inner list has less then 1 column blocks
  else if(inner.dimx < 1){
    return;
  }
  //vertical block dimension and block size not equal
  else if((inner.dimy != this->dimy) || (inner.northwest->data.Dimension() != this->northwest->data.Dimension())){
    return;
  }
  else{
    //create pointers to keep track of locations
    GridNode *pointer = this->northwest;
    GridNode *pointer2 = pointer;
    GridNode *nw = inner.northwest;
    GridNode *se = nw;
    GridNode *save = se;

    //loop row by row
    for(int y = 0; y < dimy; y++){

      //set pointer and pointer2 to beginning of every next row
      if(y>=1){
        nw = save;
        se = save;
        pointer = pointer2;

        for(int x = dimx/2; x < dimx; x++){
          pointer = pointer->next;
        }
        pointer2 = pointer;
      }

      //pointer and pointer2 points the 2 middle nodes of this
      for(int i=1; i <= (dimx/2-1); i++){
        pointer = pointer->next;  
      }
      pointer2 = pointer->next;

      //se points to the last node of current row in inner
      se = nw;
      for(int i=1; i <= inner.dimx-1; i++){
        se = se->next;
      }
      //save the se->next or else we will lose it
      save = se->next;

      //pointer->next points to inner northwest
      pointer->next = nw;
      nw->prev = pointer;

      //pointer2->prev points to inner southeast
      pointer2->prev = se;
      se->next = pointer2;
    }

    //update dimensions of this and inner
    this->dimx += inner.dimx;
    inner.northwest = NULL;
    inner.southeast = NULL;
    inner.dimx = 0;
    inner.dimy = 0;
  }
}

// inner list must have the same horizontal resolution, horizontal block dimension, and block size
// if this list has an odd number of row blocks, then the bottom side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList& inner)
{
  //this list and inner list are the same
  if(inner.northwest == this->northwest ){
    return;
  }
  //this list has less then 2 row blocks
  else if(this->dimy < 2){
    return;
  }
  //inner list has less then 1 row blocks
  else if(inner.dimy < 1){
    return;
  }
  //horizontal block dimension and block size not equal
  else if((inner.dimx != this->dimx) || (inner.northwest->data.Dimension() != this->northwest->data.Dimension())){
    return;
  }
  else{
    //create pointers to keep track of locations
    GridNode *pointer = this->northwest;
    GridNode *pointer2 = pointer;
    GridNode *nw = inner.northwest;
    GridNode *se = inner.southeast;

    for(int i = 1; i < dimx*(dimy/2); i++){
      pointer = pointer->next;
    }
    pointer2 = pointer->next;

    pointer->next = nw;
    nw->prev = pointer;

    pointer2->prev = se;
    se->next = pointer2;

    //update dimensions of this and inner
    this->dimy += inner.dimy;
    inner.northwest = NULL;
    inner.southeast = NULL;
    inner.dimx = 0;
    inner.dimy = 0;
  }
}


// PRE:  this list and otherlist have the same pixel dimensions, block dimensions, and block size
// POST: this list and otherlist are checkered with each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList& otherlist)
{
  //two lists' dimesion x are not the same
  if(otherlist.dimx != this->dimx){
    return;
  } 
  //two lists' dimesion y are not the same
  else if(otherlist.dimy != this->dimy){
    return;
  }
  //two lists' pixel dimensions are not the same
  else if(otherlist.northwest->data.Dimension() != this->northwest->data.Dimension()){
    return;
  }
  else{
    //pointers to work with
    GridNode *currentThis;;
    GridNode *p1;
    GridNode *p2;
    GridNode *currentOther;;
    GridNode *o1;
    GridNode *o2; 

    //loop through node by node
    for(int y = 0; y < dimy; y++){
      for(int x = 0; x < dimx; x++){
        currentThis = this->northwest;
        currentOther = otherlist.northwest;

        //if it is the last node
        if(x == dimx-1 && y == dimy-1){
          //x exclusive or y is odd
          if( (x%2==1&&y%2==0) || (x%2==0&&y%2==1)){
          //set up this list
          for(int i = 1; i<=y*dimx + x; i++){
            currentThis = currentThis->next;
          }
          p1 = currentThis->prev;

          //set up other list
          for(int i = 1; i<=y*dimx + x; i++){
            currentOther = currentOther->next;
          }
          o1 = currentOther->prev;

          //swap
            currentThis->next = NULL;
            currentThis->prev = o1;
            o1->next = currentThis;

            currentOther->next = NULL;
            currentOther->prev = p1;
            p1->next = currentOther;
          }
        }
        else{
          //x exclusive or y is odd
          if( (x%2==1&&y%2==0) || (x%2==0&&y%2==1)){
            //set up this list
            for(int i = 1; i<=y*dimx + x; i++){
              currentThis = currentThis->next;
            }
            p1 = currentThis->prev;
            p2 = currentThis->next;

            //set up other list
            for(int i = 1; i<=y*dimx + x; i++){
              currentOther = currentOther->next;
            }
            o1 = currentOther->prev;
            o2 = currentOther->next;

            //swap
            currentThis->next = o2;
            currentThis->prev = o1;
            o2->prev = currentThis;
            o1->next = currentThis;

            currentOther->next = p2;
            currentOther->prev = p1;
            p2->prev = currentOther;
            p1->next = currentOther;
          }
        }
      }
    }
  }
}

// POST: this list has the negative effect applied selectively to GridNodes to form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect applied).
// Ensure that the checkering looks correct for both odd and even horizontal block dimensions
void GridList::CheckerN()
{
  //create a pointer to work with
  GridNode *current;

  //loop through node by node
  for(int y = 0; y < dimy; y++){
    for(int x = 0; x < dimx; x++){
      //set current to first node
      current = northwest;

      //x exclusive or y is odd
      if( (x%2==1&&y%2==0) || (x%2==0&&y%2==1)){

        //set current to point to node at position [x][y]
        for(int i = 1; i<=y*dimx + x; i++){
          current = current->next;
        }

        //apply negative effect
        current->data.Negative();
      }
    }
  }
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  GridNode *current = northwest;
  GridNode *after = northwest;
  while(current != NULL){
    after = current->next;
    free(current);
    current = after;
  }
  northwest = NULL;
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{
  GridNode *pointer = otherlist.northwest;
  for(int y = 0; y < otherlist.dimy; y++){
    for(int x = 0; x < otherlist.dimx; x++){
      Block b = pointer->data;
      InsertBack(b);
      pointer = pointer->next;
    }
  }
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//