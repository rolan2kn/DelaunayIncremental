//
// Created by rkindela on 31-03-19.
//

#ifndef PMTOOL_VERTEX_AVL_TREE_H
#define PMTOOL_VERTEX_AVL_TREE_H

#include "avl.h"
#include "HalfEdge.h"

struct VertexAvlTreeNode: public Comparable<VertexAvlTreeNode*>
{
    vector<Vertex*> sameVertexlist;
    VertexAvlTreeNode* leftSibling;
    VertexAvlTreeNode* rightSibling;

    VertexAvlTreeNode();
    VertexAvlTreeNode(Vertex* vertex);
    VertexAvlTreeNode(const vector<Vertex*>& vertex);
    ~VertexAvlTreeNode();
    bool operator==(const VertexAvlTreeNode& vertexAvlTreeNode);
    bool operator<(const VertexAvlTreeNode& vertexAvlTreeNode);

    cmp_t Compare(VertexAvlTreeNode* key) const {
        return (*key == *this) ? EQ_CMP
                               : ((*key < *this) ? MIN_CMP : MAX_CMP);
    }

    void merge(const VertexAvlTreeNode& vertexAvlTreeNode);
    void try_2_add(Vertex* vertex);
};

typedef AvlNode<VertexAvlTreeNode*> VTnode;

class VertexLocator
{
private:
    AvlTree<VertexAvlTreeNode*>* vertexTree;
    static VertexLocator* vtxlocator;
    VertexLocator();

public:
    static VertexLocator* instance();
    ~VertexLocator();
    void buildTreeFromVertexes(const vector<Vertex*>& vertexes);
    VertexAvlTreeNode* add(Vertex* vertex);
    Face* getFaceWithPointInside(Vertex* vertex, int& inside);
    void reset();
    void cleanTree(VTnode *p) {
        if (!p) return;
        delete p->Data();
        cleanTree(p->Subtree(AvlNode<VertexAvlTreeNode*>::LEFT));
        cleanTree(p->Subtree(AvlNode<VertexAvlTreeNode*>::RIGHT));
    }
    void remove(VTnode * node);
};


#endif //PMTOOL_VERTEX_AVL_TREE_H
