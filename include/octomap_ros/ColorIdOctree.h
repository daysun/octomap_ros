#ifndef COLORIDOCTREE_H
#define COLORIDOCTREE_H
#include <iostream>
#include <octomap/OcTreeNode.h>
#include<octomap/ColorOcTree.h>
#include <octomap/OccupancyOcTreeBase.h>

///define a new kind of tree
/// the node of which contains the property 'id'
namespace octomap {

  // node definition
  class ColorIdOcTreeNode : public OcTreeNode {
  public:

      int id;
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
    ColorIdOcTreeNode() : OcTreeNode() {}

    ColorIdOcTreeNode(const ColorIdOcTreeNode& rhs) : OcTreeNode(rhs), color(rhs.color),id(rhs.id) {}

    bool operator==(const ColorIdOcTreeNode& rhs) const{
      return (rhs.value == value && rhs.color == color && rhs.id == id);
    }

    // children
    inline ColorIdOcTreeNode* getChild(unsigned int i) {
      return static_cast<ColorIdOcTreeNode*> (OcTreeNode::getChild(i));
    }
    inline const ColorIdOcTreeNode* getChild(unsigned int i) const {
      return static_cast<const ColorIdOcTreeNode*> (OcTreeNode::getChild(i));
    }

    bool createChild(unsigned int i) {
      if (children == NULL) allocChildren();
      children[i] = new ColorIdOcTreeNode();
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
    inline int getId() const { return id; }
    inline void  setId(int i) {this->id = i; }
    int & getId() { return id; }

    // has any id been integrated, 0 is very unlikely
    inline bool isIdSet() const {
      return (id != 0);
    }
    void updateIdChildren();
    //int getAverageChildId() const; //don't think the id need this

    ///id done-----------

    void updateColorChildren();


    ColorIdOcTreeNode::Color getAverageChildColor() const;

    // file I/O
    std::istream& readValue (std::istream &s);
    std::ostream& writeValue(std::ostream &s) const;

  protected:
    Color color;
    int id;
  };


  // tree definition
  class ColorIdOcTree : public OccupancyOcTreeBase <ColorIdOcTreeNode> {

  public:
    /// Default constructor, sets resolution of leafs
    ColorIdOcTree(double resolution);

    /// virtual constructor: creates a new object of same type
    /// (Covariant return type requires an up-to-date compiler)
    ColorIdOcTree* create() const {return new ColorIdOcTree(resolution); }

    std::string getTreeType() const {return "ColorIdOcTree";}

    // set node color at given key or coordinate. Replaces previous color.
    ColorIdOcTreeNode* setNodeColor(const OcTreeKey& key, const unsigned char& r,
                                 const unsigned char& g, const unsigned char& b);

    ColorIdOcTreeNode* setNodeId(const OcTreeKey& key, const unsigned int & id);

    ColorIdOcTreeNode* setNodeColor(const float& x, const float& y,
                                 const float& z, const unsigned char& r,
                                 const unsigned char& g, const unsigned char& b) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return setNodeColor(key,r,g,b);
    }

    ColorIdOcTreeNode* setNodeId(const float& x, const float& y,
                                 const float& z, const unsigned int & id) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return setNodeColor(key,id);
    }

    // integrate color measurement at given key or coordinate. Average with previous color
    ColorIdOcTreeNode* averageNodeColor(const OcTreeKey& key, const unsigned char& r,
                                  const unsigned char& g, const unsigned char& b);

    ColorIdOcTreeNode* averageNodeColor(const float& x, const float& y,
                                      const float& z, const unsigned char& r,
                                      const unsigned char& g, const unsigned char& b) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return averageNodeColor(key,r,g,b);
    }

    // integrate color measurement at given key or coordinate. Average with previous color
    ColorIdOcTreeNode* integrateNodeColor(const OcTreeKey& key, const unsigned char& r,
                                  const unsigned char& g, const unsigned char& b);
    ColorIdOcTreeNode* integrateNodeId(const OcTreeKey& key, const unsigned int & id);

    ColorIdOcTreeNode* integrateNodeColor(const float& x, const float& y,
                                      const float& z, const unsigned char& r,
                                      const unsigned char& g, const unsigned char& b) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return integrateNodeColor(key,r,g,b);
    }
    ColorIdOcTreeNode* integrateNodeId(const float& x, const float& y,
                                      const float& z, const unsigned int & id) {
      OcTreeKey key;
      if (!this->coordToKeyChecked(point3d(x,y,z), key)) return NULL;
      return integrateNodeColor(key,id);
    }

    // update inner nodes, sets color to average child color
    void updateInnerOccupancy();

    // uses gnuplot to plot a RGB histogram in EPS format
    void writeColorHistogram(std::string filename);

  protected:
    void updateInnerOccupancyRecurs(ColorIdOcTreeNode* node, unsigned int depth);

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
           ColorIdOcTree* tree = new ColorIdOcTree(0.1);
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
    static StaticMemberInitializer ColorIdOcTreeMemberInit;

  };

  //! user friendly output in format (r g b)
  std::ostream& operator<<(std::ostream& out, ColorIdOcTreeNode::Color const& c,int const & id);

} // end namespace


#endif // COLORIDOCTREE_H
