#ifndef AG_AG_NODES_H
#define AG_AG_NODES_H

#include "postgres.h"

#include "nodes/extensible.h"
#include "nodes/nodes.h"

typedef enum ag_node_tag
{
    ag_node_invalid_t = 0,

    // projection
    cypher_return_t,
    cypher_with_t,
    // reading clause
    cypher_match_t,
    // updating clause
    cypher_create_t,
    cypher_set_t,
    cypher_set_item_t,
    cypher_delete_t,
    // pattern
    cypher_path_t,
    cypher_node_t,
    cypher_relationship_t,
    // expression
    cypher_bool_const_t,
    cypher_param_t,
    cypher_map_t,
    cypher_list_t,
    // string match
    cypher_string_match_t
} ag_node_tag;

void register_ag_nodes(void);

ExtensibleNode *_new_ag_node(Size size, ag_node_tag tag);

#define new_ag_node(size, tag) \
    ( \
        AssertMacro((size) >= sizeof(ExtensibleNode)), \
        AssertMacro(tag != ag_node_invalid_t), \
        _new_ag_node(size, tag) \
    )

#define make_ag_node(type) \
    ((type *)new_ag_node(sizeof(type), CppConcat(type, _t)))

static inline bool _is_ag_node(Node *node, const char *extnodename)
{
    ExtensibleNode *extnode;

    if (!IsA(node, ExtensibleNode))
        return false;

    extnode = (ExtensibleNode *)node;
    if (strcmp(extnode->extnodename, extnodename) == 0)
        return true;

    return false;
}

#define is_ag_node(node, type) _is_ag_node((Node *)(node), CppAsString(type))

#endif
