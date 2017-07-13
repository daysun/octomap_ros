#include <octomap/octomap.h>
#include "octomap_ros/ColorOcTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <octomap/octomap_timing.h>
#include <octomap/math/Utils.h>


using namespace std;
using namespace octomap;

///print the info of node
void print_query_info(int query, list<ColorOcTreeNode*> node) {
    if(node.size()){
            list<ColorOcTreeNode*>::iterator start ;
             list<ColorOcTreeNode*>::iterator end = node.end();

             for(start = node.begin() ; start != end ; ++start){
                 cout<< (*start)->getColor()<<"\t";
//                   cout << "occupancy probability at id " << query << ":\t " << (*start)->getOccupancy() << endl;
//                   cout << "color of node is: "  << (*start)->getColor()<< endl;
             }
    }else {
        cout << "occupancy probability at id: " << query << ":\t is unknown" << endl;
    }
}

double timediff(const timeval& start, const timeval& stop){
  return (stop.tv_sec - start.tv_sec) + 1.0e-6 *(stop.tv_usec - start.tv_usec);
}

int main() {
  double res = 0.05;  // create empty tree with resolution 0.05 (different from default 0.1 for test)
  ColorOcTree tree (res);
  // insert some measurements of occupied cells
  int num = 0;
  for (int x=-20; x<20; x++) {
    for (int y=-20; y<20; y++) {
      for (int z=-20; z<20; z++) {
        point3d endpoint ((float) x*0.05f+0.01f, (float) y*0.05f+0.01f, (float) z*0.05f+0.01f);
        ColorOcTreeNode* n = tree.updateNode(endpoint, true);
        n->setColor(z*5+100,x*5+100,y*5+100); // set color to red
        tree.integrateNodeId((float) x*0.05f+0.01f, (float) y*0.05f+0.01f, (float) z*0.05f+0.01f,num);
      }
      num += y;
    }
  }

  num = 0;
  // insert some measurements of free cells
  for (int x=-30; x<30; x++) {
    for (int y=-30; y<30; y++) {
      for (int z=-30; z<30; z++) {
        point3d endpoint ((float) x*0.02f+2.0f, (float) y*0.02f+2.0f, (float) z*0.02f+2.0f);
        ColorOcTreeNode* n = tree.updateNode(endpoint, false);
        n->setColor(255,255,0); // set color to yellow
        tree.integrateNodeId((float) x*0.02f+2.0f, (float) y*0.02f+2.0f, (float) z*0.02f+2.0f,num);
      }
      num +=x;
    }
  }

  // set inner node colors
  tree.updateInnerOccupancy();

  cout << endl;

  cout << "Performing some queries:" << endl;
  timeval t1,t2,t3;

    int id = -269;
   gettimeofday(&t1, NULL);
  list<  ColorOcTreeNode *> my_result = tree.searchId(id);
   gettimeofday(&t2, NULL);
   list<  ColorOcTreeNode *> my_result2 = tree.searchId2(id);
    gettimeofday(&t3, NULL);

   /* //time cost compare
    std::cout << "time cost by search one by one: " << timediff(t1,t2) *100000<< std::endl; //67.4
    std::cout << "time cost by search using the id-structure: " << timediff(t2,t3) *100000<< std::endl;  //3.6
*/
    tree.deleteById(-269);

    //test delete
    tree.deleteTree();
    cout<<"after delete\n";
    for (int x=-20; x<20; x++) {
      for (int y=-30; y<30; y++) {
        for (int z=-20; z<20; z++) {
          point3d endpoint ((float) x*0.05f+0.01f, (float) y*0.05f+0.01f, (float) z*0.05f+0.01f);
          ColorOcTreeNode* n = tree.updateNode(endpoint, true);
          n->setColor(z*5+100,x*5+100,y*5+100); // set color to red
          tree.integrateNodeId((float) x*0.05f+0.01f, (float) y*0.05f+0.01f, (float) z*0.05f+0.01f,num);
        }
        num += y;
      }
    }
    tree.updateInnerOccupancy();

    std::string filename ("mine.ot");
    std::cout << "after delete:Writing color tree to " << filename << std::endl;
    tree.write(filename);

//  cout << "Print:" << endl;
//  tree.printTree(tree.getRoot(),0);

  return 0;
}
















