#include <vector>
#include <iostream>
#include <math.h>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "block.h"

using namespace std;
using namespace cs221util;

void Block::Build(PNG& im, int upper, int left, int dimension){
    for(int y = upper; y < upper + dimension; y++){
        vector<HSLAPixel> tempVector;
        for(int x = left; x < left + dimension; x++){
            HSLAPixel* pixel = im.getPixel(x,y);

            //put a copy of *pixel into 'tempVector'
            tempVector.push_back(*pixel);
        }
        //put 'tempVector' into 'data'
        data.push_back(tempVector);
    }
}

void Block::Render(PNG& im, int upper, int left) const{
    for(unsigned int y = upper; y< upper + data.size(); y++){
        for(unsigned int x = left; x< left + data.size() ; x++){
            *im.getPixel(x,y) = data[y-upper][x-left];
        }
    }
}

void Block::Negative(){
    for (unsigned x = 0; x < data.size(); x++){
        for (unsigned y = 0; y < data.size(); y++){
            data[x][y].h += 180;
            if(data[x][y].h>=360){
                data[x][y].h -= 360;
            }

            data[x][y].l = 1-data[x][y].l;
        }
    }
}

int Block::Dimension() const{
    return data.size();
}