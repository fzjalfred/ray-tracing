#include "JointNode.hpp"
#include <iostream>
#include <glm/glm.hpp>

class JointRecord {
    
    JointNode* jn;
    
    // execute
    glm::mat4 jointRotateRecord;
    double XIndexRecord;
    double YIndexRecord;
    float headAngleRecord;

    // recovery
    glm::mat4 jointRotate;
    double curXIndex;
    double curYIndex;
    float headAngle;

    public:

    JointRecord() {}
    
    JointRecord(JointNode* jn): jn(jn) {
        jointRotate = jn->jointRotate;
        curXIndex = jn->curXIndex;
        curYIndex = jn->curYIndex;
        headAngle = jn->headAngle;
    }

    void record() {
        this->jointRotateRecord = jn->jointRotate;
        this->XIndexRecord = jn->curXIndex;
        this->YIndexRecord = jn->curYIndex;
        this->headAngleRecord = jn->headAngle;
    }

    void execute() {
        jn->jointRotate = jointRotateRecord;
        jn->curXIndex = XIndexRecord;
        jn->curYIndex = YIndexRecord;
        jn->headAngle = headAngleRecord;
    }

    void undo() {
        jn->jointRotate = jointRotate;
        jn->curXIndex = curXIndex;
        jn->curYIndex = curYIndex;
        jn->headAngle = headAngle;
    }
    
    void print() {
        if ( (curXIndex != XIndexRecord && curYIndex !=YIndexRecord) || headAngle != headAngleRecord) {
            std::cout<<"Cmd "<<jn->m_name<<": from ("<<curXIndex<<", "<<curYIndex<<") to "<<"("<<XIndexRecord<<", "<<YIndexRecord<<") "<<std::endl;
            std::cout<<"HeadAngle from "<<headAngle<<" to "<<headAngleRecord<<std::endl;
        }
        
    }

    void printJoint() {
        if ((curXIndex != XIndexRecord && curYIndex !=YIndexRecord) || headAngle != headAngleRecord) {
            std::cout<<jn->m_name<<": ("<<jn->curXIndex<<", "<<jn->curYIndex<<")"<<std::endl;
            std::cout<<"HeadAngle: "<<jn->headAngle<<std::endl;
        }
        
    }

};