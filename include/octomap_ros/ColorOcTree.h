#ifndef COLOROCTREE_H
#define COLOROCTREE_H
#include <iostream>
#include <list>
#include <octomap/OcTreeNode.h>
#include <octomap/OccupancyOcTreeBase.h>
using namespace std;


///define a new kind of tree
// the node of which contains the property 'id'-vector
//for the reason of visualising, use the same name
namespace octomap {

  // node definition
  class ColorOcTreeNode : public OcTreeNode {
  public:

      std::vector<int> id;//father node contains the son node's id
    class Color {
    public:
    Color() : r(255), g(255), b(255) {}
    Color(unsigned char _r, unsigned char _g, unsigned char _b)
      : r(_r), g(_g), b(_b) {}
      inline bool operator== (const Color &other) const {
        return (r==other.r && g==other.g && b==other.b);
      }
      inline bool operator!= (const Color &other) const {
        return (r!=other.r || g!=other.g || b!=other.b);
      }
      unsigned char r, g, b;
    };

  public:
    ColorOcTreeNode() : OcTreeNode() {}

    ColorOcTreeNode(const ColorOcTreeNode& rhs) : OcTreeNode(rhs), color(rhs.color),id(rhs.id) {}

    bool operator==(const ColorOcTreeNode& rhs) const{
      return (rhs.value == value && rhs.color == color && rhs.id == id);
    }

//    inline ColorOcTreeNode* getSelf() {
//      return static_cast<ColorOcTreeNode*> (OcTreeNode::getChild(i));
//    }    

    // children
    inline ColorOcTreeNode* getChild(unsigned int i) {
      return static_cast<ColorOcTreeNode*> (OcTreeNode::getChild(i));
    }
    inline const ColorOcTreeNode* getChild(unsigned int i) const {
      return static_cast<const ColorOcTreeNode*> (OcTreeNode::getChild(i));
    }

    bool createChild(unsigned int i) {
      if (children == NULL) allocChildren();
      children[i] = new ColorOcTreeNode();
      return true;
    }

    bool pruneNode();
    void expandNode();

    inline Color getColor() const { return color; }
    inline void  setColor(Color c) {this->color = c; }
    inline void  setColor(unsigned char r, unsigned char g, unsigned char b) {
      this->color = Color(r,g,b);
    }

    Color& getColor() { return color; }

    // has any color been integrated? (pure white is very unlikely...)
    inline bool isColorSet() const {
      return ((color.r != 255) || (color.g != 255) || (color.b != 255));
    }

    ///id start-------------
    inline vector<int> getId() const { return id; }
    inline void  setId(vector<int> i) {this->id.swap(i); }
    vector<int>  getId() { return id; }

    // has any id been integrated, 0 is very unlikely
    inline bool isIdSet() const {
      return (id.size() != 0);
    }
   // void updateIdChildren();
    //int getAverageChildId() const; //don't think the id need this
    ///id done-----------
    void updateColorChildren(); //children's color --average color
    ColorOcTreeNode::Color getAverageChildColor() const;

    // file I/O
    std::istream& readValue (std::istream &s);
    std::ostream& writeValue(std::ostream &s) const;

    //change
    ColorOcTreeNode * search(double x, double y, double z, unsigned int depth = 0) const;

  protected:
    Color color;
  };


  // tree definition
  class ColorOcTree : public OccupancyOcTreeBase <ColorOcTreeNode> {

  public:
    /// Default constructor, sets resolution of leafs
    ColorOcTree(double resolution);

    /// virtual constructor: creates a new object of same type
    /// (Covariant return type requires an up-to-date compiler)
    ColorOcTree* create() const {return new ColorOcTree(resolution); }

    std::string getTreeType() const {return "ColorOcTree";}

    // set node color at given key or coordinate. Replaces previous color.
    ColorOcTreeNode* setNodeColor(const OcTreeKey& key, const unsigned char& r,
                                 const unsigned char& g, const unsigned char& b);

    ColorOcTreeNode* setNodeId(const OcTreeKey& key, const vector<int>  id);

    ColorOcTreeNode* setNodeColor(const float& x, const float& y,
                                 const float& z, const unsigned char& r,
                                 const unsigned char& g, const unsigned char& b) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return setNodeColor(key,r,g,b);
    }

    ColorOcTreeNode* setNodeId(const float& x, const float& y,
                                 const float& z, const vector<int>  id) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return setNodeId(key,id);
    }

    // integrate color measurement at given key or coordinate. Average with previous color
    ColorOcTreeNode* averageNodeColor(const OcTreeKey& key, const unsigned char& r,
                                  const unsigned char& g, const unsigned char& b);

    ColorOcTreeNode* averageNodeColor(const float& x, const float& y,
                                      const float& z, const unsigned char& r,
                                      const unsigned char& g, const unsigned char& b) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return averageNodeColor(key,r,g,b);
    }

    // integrate color measurement at given key or coordinate. Average with previous color
    ColorOcTreeNode* IntegrateNodeColor(const OcTreeKey& key, const unsigned char& r,
                                  const unsigned char& g, const unsigned char& b);
    ColorOcTreeNode* integrateNodeId(const OcTreeKey& key, const unsigned int & id);

    ColorOcTreeNode* IntegrateNodeColor(const float& x, const float& y,
                                      const float& z, const unsigned char& r,
                                      const unsigned char& g, const unsigned char& b) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return IntegrateNodeColor(key,r,g,b);
    }
    ColorOcTreeNode* integrateNodeId(const float& x, const float& y,
                                      const float& z, const unsigned int & id) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return integrateNodeId(key,id);
    }

    // update inner nodes, sets color to average child color
    void updateInnerOccupancy();

    // uses gnuplot to plot a RGB histogram in EPS format
    void writeColorHistogram(std::string filename);

    ///search id
    list<ColorOcTreeNode*> searchId(int id);
    list<ColorOcTreeNode*> searchId2(int id);
    void deleteById(int id);
    void searchDeleteById( list<ColorOcTreeNode*> & listId,ColorOcTreeNode* node, unsigned int depth,int sId);
    void searchDeleteById(ColorOcTreeNode* node, unsigned int depth,int sId);
    void pruneTree(ColorOcTreeNode* node, unsigned int depth);

    int getTreeDepth(){
        return this->tree_depth;
    }

    ColorOcTreeNode *  getRoot(){
        return this->root;
    }

    void printTree(ColorOcTreeNode * root, unsigned int depth);

  protected:
    void updateInnerOccupancyRecurs(ColorOcTreeNode* node, unsigned int depth);

    /**
     * Static member object which ensures that this OcTree's prototype
     * ends up in the classIDMapping only once. You need this as a
     * static member in any derived octree class in order to read .ot
     * files through the AbstractOcTree factory. You should also call
     * ensureLinking() once from the constructor.
     */
    class StaticMemberInitializer{
       public:
         StaticMemberInitializer() {
           ColorOcTree* tree = new ColorOcTree(0.1);
           AbstractOcTree::registerTreeType(tree);
         }

         /**
         * Dummy function to ensure that MSVC does not drop the
         * StaticMemberInitializer, causing this tree failing to register.
         * Needs to be called from the constructor of this octree.
         */
         void ensureLinking() {};
    };
    /// static member to ensure static initialization (only once)
    static StaticMemberInitializer ColorOcTreeMemberInit;

  };

  //! user friendly output in format (r g b)
  std::ostream& operator<<(std::ostream& out, ColorOcTreeNode::Color const& c);

} // end namespace


#endif // COLOROCTREE_H
