-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

red = gr.material({0.65, 0.05, 0.05}, {0.1, 0.1, 0.1}, 1, "")
white = gr.material({0.5,0.5, 0.5}, {0.5, 0.5, 0.5}, 1, "")
green = gr.material({0.12, 0.45, 0.15}, {0.1, 0.1, 0.1}, 0, "")

lightBlue = gr.material({0.3, 0.3, 1.0}, {0.1, 0.1, 0.1}, 10, "", 0.8)

waterBlue = gr.material({0.3, 0.3, 1.0}, {0.1, 0.1, 0.1}, 10, "glossy", 0.2)

pearlWhite = gr.material({248/255, 246/255, 220/255}, {0.1, 0.1, 0.1}, 10, "")
brown = gr.material({0.8,0.5, 0.25}, {0.1, 0.1, 0.1}, 10, "")
darkbrown = gr.material({0.21, 0.14, 0.02}, {0.1, 0.1, 0.1}, 10, "")
slightDarkbrown = gr.material({0.70, 0.47, 0.23}, {0.1, 0.1, 0.1}, 10, "")
black = gr.material({0.0,0.0, 0.0}, {0.1, 0.1, 0.1}, 10, "")

golden = gr.material({1.0, 0.8, 0.0}, {0.1, 0.1, 0.1}, 10, "")
gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25, "")
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, "")

scene = gr.node('scene')

-- cow = gr.mesh('cow', 'cow.obj')
-- cow:set_material(slightDarkbrown)
-- cow:scale(0.5,0.5,0.5)
-- cow:rotate('Y', 60)
-- cow:translate(-1, -1.8, -10)
-- scene:add_child(cow)

kabutack = gr.node('kabutack')
kabutack:translate(0,-3, -14)
scene:add_child(kabutack)

helmet = gr.mesh('head', './kabutack/head.obj')
helmet:set_material(red)
kabutack:add_child(helmet)

face = gr.mesh('face', './kabutack/face.obj')
face:set_material(pearlWhite)
kabutack:add_child(face)

torso = gr.mesh('torso', './kabutack/torso.obj')
torso:set_material(red)
kabutack:add_child(torso)

torso_deco = gr.mesh('torso_deco', './kabutack/torso_decorate.obj')
torso_deco:set_material(golden)
kabutack:add_child(torso_deco)

eyes = gr.mesh('eyes', './kabutack/eyes.obj')
eyes:set_material(black)
kabutack:add_child(eyes)

nose = gr.mesh('nose', './kabutack/nose.obj')
nose:set_material(lightBlue)
kabutack:add_child(nose)    


water = gr.mesh('water', './kabutack/water.obj')
water:set_material(waterBlue)
water:translate(0,-3,-10)
scene:add_child(water)    


-- the floor
plane = gr.mesh( 'plane', 'plane.obj' )
scene:add_child(plane)
plane:set_material(white)
plane:scale(200, 30, 500)
plane:translate(0, -3, -15)

walls = gr.node('walls')
walls:translate(0,0, -5)
scene:add_child(walls)

-- ceil = gr.cube('ceil')
-- ceil:scale(8, 1, 9)
-- ceil:set_material(white)
-- ceil:translate(-4, 3, -14)
-- walls:add_child(ceil)

-- backWall = gr.cube('back')
-- backWall:scale(6, 6, 1)
-- backWall:set_material(white)
-- backWall:translate(-3, -3, -14)
-- walls:add_child(backWall)

leftWall = gr.cube('left')
leftWall:scale(1, 6, 24)
leftWall:set_material(green)
leftWall:translate(-4, -3, -28)
walls:add_child(leftWall)

rightWall = gr.cube('right')
rightWall:scale(1, 6, 24)
rightWall:set_material(red)
rightWall:translate(3, -3, -28)
walls:add_child(rightWall)



-- The lights
l1 = gr.light({200,200,400}, {0.8, 0.8, 0.8}, {1, 0, 0})
-- l2 = gr.light({1, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})
-- l3 = gr.light({0, 0, 200}, {0.8, 0.8, 0.8}, {1, 0, 0})

gr.render(scene, 'final.png', 512, 512, 
	  {0, 0, 0,}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1})

