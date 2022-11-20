/**************************************************
Scene.cpp contains the implementation of the draw command
*****************************************************/
#include "RTScene.h"
#include "RTCube.h"
#include "RTObj.h"

// The scene init definition 
#include "RTScene.inl"


using namespace glm;

/**
 * builds the triangle soup by traversing over the scene graph, apply the proper model matrix 
 * to a common coordinate system (world or camera), and assign the proper material for each
 * triangle.
 */
void RTScene::buildTriangleSoup() { //TODO: can it be a static function?

    camera -> computeMatrices();
    
    // Define stacks for depth-first search (DFS)
    std::stack<Node*> dfs_stack;
    std::stack<mat4>  matrix_stack;
    
    // Initialize the current state variable for DFS
    Node* cur = node["world"]; // root of the tree
    mat4 cur_VM = camera -> view; // Initially, we are at the "world" node, whose modelview matrix is just camera's view matrix.
    
    dfs_stack.push(cur);
    
    // push the initial VM to vm stack
    matrix_stack.push(cur_VM);
    
    // Compute total number of connectivities in the graph; this would be an upper bound for
    // the stack size in the depth first search over the directed acyclic graph
    int total_number_of_edges = 0; 
    for(const auto &n : node) total_number_of_edges += n.second->childnodes.size();
    
    // If you want to print some statistics of your scene graph
    // std::cout << "total numb of nodes = " << node.size() << std::endl;
    // std::cout << "total number of edges = " << total_number_of_edges << std::endl;
    
    while(!dfs_stack.empty() ){
        // Detect whether the search runs into infinite loop by checking
        // whether the stack is longer than the number of edges in the graph.
        if ( dfs_stack.size() > total_number_of_edges ){
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }
        
        // top-pop the stacks
        cur = dfs_stack.top();  dfs_stack.pop();
        cur_VM = matrix_stack.top(); matrix_stack.pop();
        
        // draw all the models at the current node
        for ( size_t i = 0; i < cur -> models.size(); i++ ){
            // update corresponding VM
            glm::mat4 new_VM = cur_VM * cur-> modeltransforms[i];
            
            // From model to triangle
            // Add triangle to the triangle soup
            for (Triangle tri : (cur->models[i]) -> geometry -> elements) {
                Triangle new_tri;
                new_tri.pos = std::vector<glm::vec3>(3);
                new_tri.normal = std::vector<glm::vec3>(3);

                for(int j = 0; j < 3; j++) {
                    new_tri.pos[j] = new_VM * glm::vec4(tri.pos[j], 1.0f);
                    glm::mat3 AInvT = glm::transpose(glm::inverse(glm::mat3(new_VM)));
                    new_tri.normal[j] = glm::normalize(AInvT * tri.normal[j]);
                }
                
                new_tri.material = (cur->models[i]) -> material;
                
                triangle_soup.push_back(new_tri);
            }
        }
        
        // Continue the DFS: put all the child nodes of the current node in the stack
        for ( size_t i = 0; i < cur -> childnodes.size(); i++ ){
            dfs_stack.push( cur -> childnodes[i] );
      
            // push vm corresponding to the childnode to vm stack
            matrix_stack.push(cur_VM * cur -> childtransforms[i]);
        }
    }
}