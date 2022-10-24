#include "JointNode.hpp"
#include <iostream>
#include <glm/glm.hpp>

class JointRotateCommand {
    
    JointNode* jn;
    float angle_factor; // [0,1]
    
    // recovery
    glm::mat4 jointRotate;
    double curXIndex;
    double curYIndex;

    public:

    JointRotateCommand() {}
    
    JointRotateCommand(JointNode* jn,
    float angle_factor): jn(jn), angle_factor(angle_factor) {
        jointRotate = jn->jointRotate;
        curXIndex = jn->curXIndex;
        curYIndex = jn->curYIndex;
    }

    void execute() {
        jn->rotate(angle_factor);
    }

    void undo() {
        jn->rotate(-angle_factor);
    }
};