#include <octomap/octomap.h>
#include "octomap_ros/ColorOcTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <octomap/octomap_timing.h>
#include <octomap/math/Utils.h>

#include "testing.h"

using namespace std;
using namespace octomap;

///print only one
void print_query_info(int query, list<ColorOcTreeNode*> node) {
  if ((*node.begin()) != NULL) {

    cout << "occupancy probability at id:" << query << ":\t " << (*node.begin())->getOccupancy() << endl;
    //cout << "color of node is: " << (*node.begin())->getColor()<< endl;
    cout<<"id of node is:"<<(*node.begin())->getId()<<endl;
  }
  else
    cout << "occupancy probability at id: " << query << ":\t is unknown" << endl;
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
        //n->setId(num);
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
        n->setId(num);
      }
      num +=x;
    }
  }

  // set inner node colors
  tree.updateInnerOccupancy();

  cout << endl;

  std::string filename ("mine.ot");
  std::cout << "Writing color tree to " << filename << std::endl;
  // write color tree
  //EXPECT_TRUE(tree.write(filename));
  tree.write(filename);

/*
  // read tree file
  cout << "Reading color tree from "<< filename <<"\n";
  AbstractOcTree* read_tree = AbstractOcTree::read(filename);
  EXPECT_TRUE(read_tree);
  EXPECT_EQ(read_tree->getTreeType().compare(tree.getTreeType()), 0);
  EXPECT_FLOAT_EQ(read_tree->getResolution(), tree.getResolution());
  EXPECT_EQ(read_tree->size(), tree.size());
  ColorOcTree* read_color_tree = dynamic_cast<ColorOcTree*>(read_tree);
  EXPECT_TRUE(read_color_tree);
  */


  cout << "Performing some queries:" << endl;
  {
    int id = 0;
  list<  ColorOcTreeNode *> my_result = tree.searchId(id);
//    ColorOcTreeNode* result = tree.search (query);
//    ColorOcTreeNode* result2 = read_color_tree->search (query);
    std::cout << "READ: ";
    print_query_info(id, my_result);
 //   std::cout << "WRITE: ";
 //   print_query_info(id, my_result2);
//    EXPECT_TRUE(result);
//    EXPECT_TRUE(result2);
//    EXPECT_EQ(result->getColor(), result2->getColor());
//    EXPECT_EQ(result->getLogOdds(), result2->getLogOdds());
  }

  return 0;
}
