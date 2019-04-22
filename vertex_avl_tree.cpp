//
// Created by rkindela on 31-03-19.
//

#include "vertex_avl_tree.h"
#include <map>
#include "PointRepository.h"
#include "queue"
#include "utils.h"

VertexAvlTreeNode::VertexAvlTreeNode()
: sameVertexlist(), Comparable<VertexAvlTreeNode*>(this), leftSibling(nullptr), rightSibling(nullptr)
{
}

VertexAvlTreeNode::VertexAvlTreeNode(Vertex* vertex)
        : sameVertexlist(), Comparable<VertexAvlTreeNode*>(this), leftSibling(nullptr), rightSibling(nullptr)
{
    sameVertexlist.push_back(vertex);
}

VertexAvlTreeNode::VertexAvlTreeNode(const vector<Vertex*>& _sameVertexList)
        : sameVertexlist(), Comparable<VertexAvlTreeNode*>(this), leftSibling(nullptr), rightSibling(nullptr)
{
    map<HalfEdge*, Vertex*> vertexMap;
    for (auto vertex: _sameVertexList)
    {
        if (vertex->edge != nullptr)
            vertexMap.insert(make_pair(vertex->edge, vertex));
    }
    for (auto vertex: sameVertexlist)
    {
        if (vertex->edge != nullptr)
            vertexMap.insert(make_pair(vertex->edge, vertex));
    }
    sameVertexlist.clear();
    auto cursor = vertexMap.begin(), end = vertexMap.end();
    for(; cursor != end; ++cursor)
    {
        sameVertexlist.push_back((*cursor).second);
    }
    vertexMap.clear();
}

VertexAvlTreeNode::~VertexAvlTreeNode()
{
    sameVertexlist.clear();     // All Vertex pointers will be destroyed by someone else
}

bool VertexAvlTreeNode::operator==(const VertexAvlTreeNode& vertexAvlTreeNode)
{
    size_t size = sameVertexlist.size();
    size_t osize = vertexAvlTreeNode.sameVertexlist.size();

    if ((size == 0 || osize == 0) && (size+osize > 0))
        return false;
    if (size+osize == 0)
        return true;

    auto vertex = sameVertexlist[0];
    auto overtex = vertexAvlTreeNode.sameVertexlist[0];

    int lorder = PointRepository::instance()->computeLexicographicOrder(vertex->pointIndex, overtex->pointIndex);

    return lorder == 0;
}

bool VertexAvlTreeNode::operator<(const VertexAvlTreeNode& vertexAvlTreeNode)
{
    size_t size = sameVertexlist.size();
    size_t osize = vertexAvlTreeNode.sameVertexlist.size();

    if (size == 0)
        return true;
    if (osize == 0)
        return false;

    auto vertex = sameVertexlist[0];
    auto overtex = vertexAvlTreeNode.sameVertexlist[0];

    int lorder = PointRepository::instance()->computeLexicographicOrder(vertex->pointIndex, overtex->pointIndex);

    return lorder == -1;
}

void VertexAvlTreeNode::merge(const VertexAvlTreeNode& vertexAvlTreeNode)
{
    map<HalfEdge*, Vertex*> vertexMap;

    for (auto vertex: vertexAvlTreeNode.sameVertexlist)
    {
        if (vertex->edge != nullptr)
            vertexMap.insert(make_pair(vertex->edge, vertex));
    }
    for (auto vertex: sameVertexlist)
    {
        if (vertex->edge != nullptr)
            vertexMap.insert(make_pair(vertex->edge, vertex));
    }

    sameVertexlist.clear();
    auto cursor = vertexMap.begin(), end = vertexMap.end();
    for(; cursor != end; ++cursor)
    {
        sameVertexlist.push_back((*cursor).second);
    }
    vertexMap.clear();
}

void VertexAvlTreeNode::try_2_add(Vertex* vertex)
{
    if (vertex == nullptr)
        return;

    auto cursor = sameVertexlist.begin(), end = sameVertexlist.end();
    bool isKnown = false;
    while (cursor != end and !isKnown)
    {
        isKnown = ((*cursor)->edge == vertex->edge);            // if current vertex edge exists in list
        ++cursor;
    }

    if (!isKnown)                                               // if vertex is a new verson of itself then add it
    {
        sameVertexlist.push_back(vertex);
    }
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 * VertexLocator Class Implementation
 */

VertexLocator* VertexLocator::vtxlocator = nullptr;

VertexLocator::VertexLocator()
{
    this->vertexTree = new AvlTree<VertexAvlTreeNode*>();
}

VertexLocator::~VertexLocator()
{
}

VertexLocator* VertexLocator::instance()
{
    if (vtxlocator == nullptr)
        vtxlocator = new VertexLocator();

    return vtxlocator;
}

void VertexLocator::buildTreeFromVertexes(const vector<Vertex*>& vertexes)
{
    for (auto vertex: vertexes)
    {
        VertexAvlTreeNode* v = new VertexAvlTreeNode(vertex);

        // add a node to the balanced binary tree
        VTnode* node = vertexTree->Search(v);
        if (node != nullptr)
        {
            node->Key()->try_2_add(vertex);
        }
        else
        {
            node = vertexTree->Insert(v);

            v->rightSibling = (VertexAvlTreeNode*)0;
            v->leftSibling = (VertexAvlTreeNode*)0;

            // add a node to the balanced binary tree
            VTnode* nx = vertexTree->Next(node);
            VTnode* np = vertexTree->Prev(node);

            if (nx != nullptr) {
                v->rightSibling = (VertexAvlTreeNode*)nx->Data();
                v->rightSibling->leftSibling = v;
            }
            if (np != nullptr) {
                v->leftSibling = (VertexAvlTreeNode*)np->Data();
                v->leftSibling->rightSibling = v;
            }
        }
    }
}

VertexAvlTreeNode* VertexLocator::add(Vertex* vertex)
{
    VertexAvlTreeNode* v = new VertexAvlTreeNode(vertex);

    // add a node to the balanced binary tree
    VTnode* node = vertexTree->Search(v);
    if (node != nullptr)
    {
        node->Key()->merge(*v);
        v = node->Key();
    }
    else
    {
        node = vertexTree->Insert(v);
        v = (VertexAvlTreeNode*)node->Data();
        v->rightSibling = (VertexAvlTreeNode*)0;
        v->leftSibling = (VertexAvlTreeNode*)0;

        // add a node to the balanced binary tree
        VTnode* nx = vertexTree->Next(node);
        VTnode* np = vertexTree->Prev(node);

        if (nx != nullptr) {
            v->rightSibling = (VertexAvlTreeNode*)nx->Data();
            v->rightSibling->leftSibling = v;
        }
        if (np != nullptr) {
            v->leftSibling = (VertexAvlTreeNode*)np->Data();
            v->leftSibling->rightSibling = v;
        }
    }

    return v;
}

Face* VertexLocator::getFaceWithPointInside(Vertex* vertex, int& _inside)
{

    // add a node to the balanced binary tree
    VertexAvlTreeNode* vatn = this->add(vertex);
    _inside = -1;

    Face* face = nullptr;

    if (vatn == nullptr)
        return face;

    vector<VertexAvlTreeNode*> current_vatn;
    current_vatn.push_back(vatn->leftSibling);
    current_vatn.push_back(vatn->rightSibling);
    if (vatn->leftSibling)
        current_vatn.push_back(vatn->leftSibling->leftSibling);
    if (vatn->rightSibling)
        current_vatn.push_back(vatn->rightSibling->rightSibling);

    int i = 0, size = current_vatn.size();
    while(i < size && face == nullptr)
    {
        if (current_vatn[i] != nullptr)
        {
            auto cursor = current_vatn[i]->sameVertexlist.begin(), end = current_vatn[i]->sameVertexlist.end();

            while (cursor != end && face == nullptr) {
                auto edge = (*cursor)->edge;

                if (edge != nullptr && edge->face != nullptr)
                {

                    _inside = edge->face->inside(vertex->pointIndex);
                    if (_inside != -1)
                        face = edge->face;
                }

                ++cursor;
            }
        }

        ++i;
    }
    current_vatn.clear();

    return face;
}

void VertexLocator::reset()
{
    if(vertexTree != nullptr)
        delete vertexTree;
    vertexTree = nullptr;
    VertexLocator::vtxlocator = nullptr;
}

void VertexLocator::remove(VTnode *node)
{
    // remove the node from the balanced binary tree
    VTnode* nd = node;

    if (nd == (VTnode*)0) {
        const char* m = " internal error:  attempt to remove segment not in tree";
        fprintf(stderr, "%s\n", m);

        vertexTree->DumpTree();
        throw runtime_error(m);
    }
    VertexAvlTreeNode* s = (VertexAvlTreeNode*)node->Data();
    // get the above and below segments pointing to each other
    VTnode* nx = vertexTree->Next(nd);
    if (nx != (VTnode*)0) {
        VertexAvlTreeNode* sx = (VertexAvlTreeNode*)(nx->Data());
        sx->rightSibling = s->rightSibling;
    }
    VTnode* np = vertexTree->Prev(nd);
    if (np != (VTnode*)0) {
        VertexAvlTreeNode* sp = (VertexAvlTreeNode*)(np->Data());
        sp->leftSibling = s->leftSibling;
    }
    vertexTree->Delete(nd->Key());       // now can safely remove it
    delete s;  // note:  s == nd->Data()
}
