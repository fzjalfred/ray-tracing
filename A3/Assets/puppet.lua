-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

-- Create the top level root node named 'root'.
rootNode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
lightBlue = gr.material({0.3, 0.3, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
pearlWhite = gr.material({248/255, 246/255, 220/255}, {0.1, 0.1, 0.1}, 10)
brown = gr.material({0.8,0.5, 0.25}, {0.1, 0.1, 0.1}, 10)
darkbrown = gr.material({0.21, 0.14, 0.02}, {0.1, 0.1, 0.1}, 10)
slightDarkbrown = gr.material({0.70, 0.47, 0.23}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0,0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0,1.0, 1.0}, {0.5, 0.5, 0.5}, 10)
golden = gr.material({1.0, 0.8, 0.0}, {0.1, 0.1, 0.1}, 10)
---------------------------------------torso--------------------------------------
torso = gr.mesh('torso', 'torso')

torso:set_material(red)

rootNode:add_child(torso)

torso_decorate = gr.mesh('torso_decorate', 'torso_decorate')
torso_decorate:set_material(golden)
torso:add_child(torso_decorate)

torso_black_body = gr.mesh('torso_black_body', 'torso_black_body')
torso_black_body:set_material(black)
torso:add_child(torso_black_body)

---------------------------------NeckNode-----------------------------
neckNode = gr.node('neckNode')
torso:add_child(neckNode)
---------------------------------NeckJoint-----------------------------
neckJoint = gr.joint('neckJoint', {-5, 0, 5}, {-80, 0, 80});
neckJoint:translate(0.0, 1.5, 0.0)
neckNode:add_child(neckJoint)

------------------------------------Head-----------------------------
head = gr.mesh('head', 'head')
head:set_material(red)

face = gr.mesh('face', 'face')
face:set_material(pearlWhite)
face:translate(0.0, -1.5, 0.0)

neckJoint:add_child(face)
face:add_child(head)

head_decorate = gr.mesh('head_decorate', 'head_decorate')
head_decorate:set_material(golden)
head:add_child(head_decorate)

head_black_body = gr.mesh('head_black_body', 'head_black_body')
head_black_body:set_material(black)
head:add_child(head_black_body)

eyes = gr.mesh('eyes', 'eyes')
eyes:set_material(black)
head:add_child(eyes)

nose = gr.mesh('nose', 'nose')
nose:set_material(lightBlue)
head:add_child(nose)


---------------------------------RightUpperArmJoint-----------------------------
-- I don't know why the fuck I need this Node. But rotation origin will not move if I put translation into Joint.
rightUpperArmNode = gr.node('rightUpperArmNode')
rightUpperArmNode:translate(0.76, 1.1, 0.0)
torso:add_child(rightUpperArmNode)
rightUpperArmJoint = gr.joint('rightUpperArmJoint', {-90, -90, 90}, {0, 0, 0});
rightUpperArmNode:add_child(rightUpperArmJoint)

rightUpperArm = gr.mesh('ArmUp', 'rightUpperArm')
rightUpperArm:set_material(black)
rightUpperArmJoint:add_child(rightUpperArm)
---------------------------------rightLowerArmNode-----------------------------
rightLowerArmNode = gr.node('rightLowerArmNode')
rightLowerArmNode:translate(1.33-0.76, 0.6058-1.1, 0)
rightUpperArm:add_child(rightLowerArmNode)
rightLowerArmJoint = gr.joint('rightLowerArmJoint', {-60, 0, 10}, {0, 0, 0});
rightLowerArmNode:add_child(rightLowerArmJoint)

rightLowerArm = gr.mesh('ArmLower', 'rightLowerArm')
rightLowerArm:set_material(red)
rightLowerArmJoint:add_child(rightLowerArm)

---------------------------------RightHandNode---------------------------
rightHandNode = gr.node('rightHandNode')
rightLowerArm:add_child(rightHandNode)
rightHandNode:translate(-0.05, 0, 0.8)
rightHandJoint = gr.joint('rightHandJoint', {-15, 0, 15}, {0, 0, 0});
rightHandNode:add_child(rightHandJoint)

rightHand = gr.mesh('hand', 'rightHand')
rightHand:set_material(black)

rightHandJoint:add_child(rightHand)


---------------------------------leftUpperArmJoint-----------------------------
-- I don't know why the fuck I need this Node. But rotation origin will not move if I put translation into Joint.
leftUpperArmNode = gr.node('leftUpperArmNode')
leftUpperArmNode:rotate('x', 180)
leftUpperArmNode:rotate('z', 180)
leftUpperArmNode:translate(-1.5, 0.04, 0.0)
leftUpperArmNode:translate(0.76, 1.1, 0.0)

torso:add_child(leftUpperArmNode)
leftUpperArmJoint = gr.joint('leftUpperArmJoint', {-90, -90, 90}, {0, 0, 0});
leftUpperArmNode:add_child(leftUpperArmJoint)

leftUpperArm = gr.mesh('ArmUp', 'leftUpperArm')
leftUpperArm:set_material(black)
leftUpperArmJoint:add_child(leftUpperArm)
---------------------------------leftLowerArmNode-----------------------------
leftLowerArmNode = gr.node('leftLowerArmNode')
leftLowerArmNode:rotate('y', 180)
leftLowerArmNode:rotate('z', 180)
leftLowerArmNode:translate(1.33-0.76, 0.5858-1.1, 0)
leftUpperArm:add_child(leftLowerArmNode)
leftLowerArmJoint = gr.joint('leftLowerArmJoint', {-60, 0, 10}, {0, 0, 0});
leftLowerArmNode:add_child(leftLowerArmJoint)

leftLowerArm = gr.mesh('ArmLower', 'leftLowerArm')
leftLowerArm:set_material(red)
leftLowerArmJoint:add_child(leftLowerArm)

---------------------------------leftHandNode---------------------------
leftHandNode = gr.node('leftHandNode')
leftLowerArm:add_child(leftHandNode)
leftHandNode:translate(-0.05, 0, 0.8)
leftHandJoint = gr.joint('leftHandJoint', {-15, 0, 15}, {0, 0, 0});
leftHandNode:add_child(leftHandJoint)

leftHand = gr.mesh('hand', 'leftHand')
leftHand:set_material(black)

leftHandJoint:add_child(leftHand)



-- testNode = gr.mesh('sphere', 'rightUpperArmJoint')
-- testNode:set_material(white)
-- testNode:scale(0.05, 0.05, 0.05)
-- testNode:translate(1.33, 0.6758, 0)
-- torso:add_child(testNode)


---------------------------------WaistNode-----------------------------
waistNode = gr.node('waistNode')
waistNode:translate(0, -0.3, 0)
torso:add_child(waistNode)

waistJoint = gr.joint('waistJoint', {-30,0, 30}, {0, 0, 0});
waistNode:add_child(waistJoint)

---------------------------------rightLegNode-----------------------------
rightLegNode = gr.node('rightLegNode')
rightLegNode:rotate('y', 180)
rightLegNode:translate(0.4, 0, 0)
waistJoint:add_child(rightLegNode)
rightLegJoint = gr.joint('rightLegJoint', {-50, -50, 50}, {0, 0, 0})
rightLegNode:add_child(rightLegJoint)

rightLegUp = gr.mesh('LegUp', 'rightLegUp')
rightLegUp:set_material(black)
rightLegJoint:add_child(rightLegUp)


---------------------------------rightLegUpperNode-----------------------------
rightLegKneeNode = gr.node('rightLegKneeNode')
rightLegKneeNode:rotate('y', 180)
rightLegKneeNode:translate(0.03,-0.3,0)
rightLegUp:add_child(rightLegKneeNode)

rightLegKneeJoint = gr.joint('rightLegKneeJoint', {0, 0, 45}, {0, 0, 0})
rightLegKneeNode:add_child(rightLegKneeJoint)

rightLegLower = gr.mesh('LegLower', 'rightLegLower')
rightLegLower:set_material(red)
rightLegKneeJoint:add_child(rightLegLower)

-- ---------------------------------rightLegLowerNode---------------------------
rightLegLowerNode =gr.node('rightLegLowerNode')
rightLegLowerNode:translate(0.02, -1, -0.1)
rightLegLower:add_child(rightLegLowerNode)

rightLegLowerJoint = gr.joint('rightLegLowerJoint', {-45, 0, 45}, {0, 0, 0})
rightLegLowerNode:add_child(rightLegLowerJoint)

rightFoot = gr.mesh('foot', 'rightFoot')
rightFoot:set_material(red)
rightLegLowerJoint:add_child(rightFoot)

---------------------------------rightLegNode-----------------------------
leftLegNode = gr.node('leftLegNode')
leftLegNode:translate(-0.4, 0, 0)
waistJoint:add_child(leftLegNode)
leftLegJoint = gr.joint('leftLegJoint', {-50, -50, 50}, {0, 0, 0})
leftLegNode:add_child(leftLegJoint)

leftLegUp = gr.mesh('LegUp', 'leftLegUp')
leftLegUp:set_material(black)
leftLegJoint:add_child(leftLegUp)


---------------------------------leftLegUpperNode-----------------------------
leftLegKneeNode = gr.node('leftLegKneeNode')
leftLegKneeNode:translate(0.03,-0.3,0)
leftLegUp:add_child(leftLegKneeNode)

leftLegKneeJoint = gr.joint('leftLegKneeJoint', {0, 0, 45}, {0, 0, 0})
leftLegKneeNode:add_child(leftLegKneeJoint)

leftLegLower = gr.mesh('LegLower', 'leftLegLower')
leftLegLower:set_material(red)
leftLegKneeJoint:add_child(leftLegLower)

-- ---------------------------------leftLegLowerNode---------------------------
leftLegLowerNode =gr.node('leftLegLowerNode')
leftLegLowerNode:translate(0.02, -1, -0.1)
leftLegLower:add_child(leftLegLowerNode)

leftLegLowerJoint = gr.joint('leftLegLowerJoint', {-45, 0, 45}, {0, 0, 0})
leftLegLowerNode:add_child(leftLegLowerJoint)

leftFoot = gr.mesh('foot', 'leftFoot')
leftFoot:set_material(red)
leftLegLowerJoint:add_child(leftFoot)


rootNode:rotate('y', -15)
rootNode:translate(0.0, -0.5, -5.0)


return rootNode;