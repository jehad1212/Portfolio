from ast import And
from Node import Node

"""
Tree
----------

This class represents the Binary Tree used to model our baby mobile. 

Each Tree consists of the following properties:
    - root: The root of the Tree

The class also supports the following functions:
    - put(node, child, left_child): Adds child to the given node as the left or right child depending on the value of left_child
    - move_subtree(node_a, node_b, left_child): Move node_a to the left or right child of node_b depending on the value of left_child
    - find_max_imbalance(): Finds the node with the maximum imbalance in the tree
"""


class Tree():
    
    root: Node

    def __init__(self, root: Node = None) -> None:
        """
        The constructor for the Tree class.
        :param root: The root node of the Tree.
        """
        self.root = root

    def put(self, node: Node, child: Node, left_child: bool) -> None:
        """
        Adds the given child to the given node as the left or right child depending on the value of left_child.
        If a node already has a child at the indicated position, this function should do nothing.
        You are guranteed that the given node is not already part of the tree
        :param node: The node to add the child to.
        :param child: The child to add to the node.
        :param left_child: True if the child should be added to the left child, False otherwise.
        """

        if left_child and (node.left_child != None):
            return
        elif not left_child and (node.right_child != None):
            return
        
        if left_child:
            node.add_left_child(child);
        elif not left_child:
            node.add_right_child(child);
        else:
            raise Exception("wasn't a bool");    
        return






        

    def move_subtree(self, node_a: Node, node_b: Node, left_child: bool) -> None:
        """
        Moves the subtree rooted at node_a to the left or right child of node_b depending on the value of left_child.
        If node_b already has a child at the indicated position, this function should do nothing
        You can safely assume that node_b is not descendent of node_a.
        :param node_a: The root of the subtree to move.
        :param node_b: The node to add the subtree to.
        :param left_child: True if the subtree should be added to the left child, False otherwise.
        """


        if left_child and (node_b.left_child != None):
            return
        elif not left_child and (node_b.right_child != None):
            return


        if not node_a.is_external() and left_child:
            node_b.left_child = node_a;
            node_b = node_a;
            move_subtree(node_a.left_child, node_b,1);
            move_subtree(node_a.right_child, node_b,0);
        elif not node_a.is_external() and not left_child:
            node_b.right_child = node_a;
            node_b = node_a;
            move_subtree(node_a.left_child, node_b,1);
            move_subtree(node_a.right_child, node_b, 0);




        

    def find_max_imbalance(self) -> int:
        """
        Finds the node with the maximum imbalance in the tree.
        :return: The node with the maximum imbalance.
        """
        
        


        max = root.get_imbalance();
        n = root;

        if n.is_external():
            return
        if n.get_imbalance()> max:
            max = n.get_imbalance();
        find_max_imbalance(n.get_left_child);
        find_max_imbalance(n.get_right_child);

        return max;

