#include "JointNode.hpp"
#include <iostream>
#include <glm/glm.hpp>

class JointRecord {
    
    JointNode* jn;
    
    // execute
    glm::mat4 jointRotateRecord;
    double XIndexRecord;
    double YIndexRecord;

    // recovery
    glm::mat4 jointRotate;
    double curXIndex;
    double curYIndex;

    public:

    JointRecord() {}
    
    JointRecord(JointNode* jn): jn(jn) {
        jointRotate = jn->jointRotate;
        curXIndex = jn->curXIndex;
        curYIndex = jn->curYIndex;
    }

    void record() {
        this->jointRotateRecord = jn->jointRotate;
        this->XIndexRecord = jn->curXIndex;
        this->YIndexRecord = jn->curYIndex;
    }

    void execute() {
        jn->jointRotate = jointRotateRecord;
        jn->curXIndex = XIndexRecord;
        jn->curYIndex = YIndexRecord;
    }

    void undo() {
        jn->jointRotate = jointRotate;
        jn->curXIndex = curXIndex;
        jn->curYIndex = curYIndex;
    }
    
    void print() {
        if (curXIndex != XIndexRecord && curYIndex !=YIndexRecord)
        std::cout<<"Cmd "<<jn->m_name<<": from ("<<curXIndex<<", "<<curYIndex<<") to "<<"("<<XIndexRecord<<", "<<YIndexRecord<<") "<<std::endl;
    }

    void printJoint() {
        if (curXIndex != XIndexRecord && curYIndex !=YIndexRecord)
        std::cout<<jn->m_name<<": ("<<jn->curXIndex<<", "<<jn->curYIndex<<")"<<std::endl;
    }

};