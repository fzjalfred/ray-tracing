# Compilation
In A3 folder,
```
premake4 gmake
make
```
boot ``` ./A3 ```

# Manual
Follow assignment specifications.

I have my A3 run in Machine gl47.

Data Structure Changes:
1. make "rotate" function in JointNode to perform the joint rotation;
add curXIndex, curYIndex to record the normalization of the angle range allowed by mapping the angle range to [0, 1];
add rotationX, rotationY to record the current rotation in x-axis and y-axis.
3. add jointRotate in SceneNode to record a joint rotation, this is the one that finally used in rendering.

Noticed that I made serveral obj file with Blender v3.3.1.

For head and torso, subdivison surface is added to make it smooth. Due to that, obj file is as large as 10Mb.

It may take a bit longer while loading the obj model files.

hierarchical model:

under cs488 directory:
cd A3
premake4 gmake
make
./A3 Assets/puppet.lua

hierarchical model:

                      head_black_body               head_decorate                nose          eyes
                                  |-   -  -  -  -   -   -|-   -   -   -  -   -   -|-   -   -   -|
                                                         |
                                                       head
                                                         |
                                                       face
                                                         |
                                                     neckJoint
                                                         |
                                                     neckNode
                                                         |
                                                       neck                      
                                                         |
                                   torso_decorate -  - torso - - torso_black_body
                                                         |                     
                |----------------------------------------|--------------------------------------|
                |                                        |                                      |
        leftUpperArmNode                             waistNode                          rightUpperArmNode
                |                                        |                                      |
        leftUpperArmJoint                           waistJoint                          rightUpperArmJoint
                |                                  /          \                                 |
          leftUpperArm                      leftLegNode    rightLegNode                   rightUpperArm                        
                |                                 |             |                               |                                  
        leftLowerArmNode                     leftLegJoint   rightLegJoint               rightLowerArmNode                    
                |                                 |             |                               |                                  
        leftLowerArmJoint                    leftLegUp     rightLegUp                    rightLowerArmJoint                     
                |                                 |             |                               |                                  
          leftLowerArm                      leftLegLower  rightLegLower                   rightLowerArm                          
                |                                 |             |                               |                                  
          leftHandNode                   leftLegKneeNode   rightLegKneeNode               rightHandNode
                |                                 |             |                               |                                  
          leftHandJoint                  leftLegKneeJoint  rightLegKneeJoint             rightHandJoint
                |                                 |             |                               |
            leftHand                        leftLegLower    rightLegLower                  rightHand
                                                  |             |
                                        leftLegLowerNode    rightLegLowerNode
                                                  |             |
                                        leftLegLowerJoint   rightLegLowerJoint
                                                  |             |
                                            leftFoot       rightFoot