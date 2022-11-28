-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

red = gr.material({0.65, 0.05, 0.05}, {0.1, 0.1, 0.1}, 1, "")
white = gr.material({0.5,0.5, 0.5}, {0.5, 0.5, 0.5}, 1, "")
lightblue_refract = gr.material({0.3, 0.3, 0.6}, {0.5, 0.7, 0.5}, 25, "refract", 0.5)
green = gr.material({0.12, 0.45, 0.15}, {0.1, 0.1, 0.1}, 0, "")

lightBlue = gr.material({0.3, 0.3, 1.0}, {0.1, 0.1, 0.1}, 30, "", 0.8)

pearlWhite = gr.material({248/255, 246/255, 220/255}, {0.1, 0.1, 0.1}, 10, "")
brown = gr.material({0.8,0.5, 0.25}, {0.1, 0.1, 0.1}, 10, "")
darkbrown = gr.material({0.21, 0.14, 0.02}, {0.1, 0.1, 0.1}, 10, "")
slightDarkbrown = gr.material({0.70, 0.47, 0.23}, {0.1, 0.1, 0.1}, 10, "")
black = gr.material({0.0,0.0, 0.0}, {0.1, 0.1, 0.1}, 10, "")

golden = gr.material({1.0, 0.8, 0.0}, {0.1, 0.1, 0.1}, 10, "")
gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25, "")
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, "")

scene = gr.node('scene')
scene:rotate('X', 0)


s = gr.sphere('test')
s:scale(2,2,2)
s:translate(0, 0, -10)
s:set_material(lightblue_refract)
scene:add_child(s)

-- s1 = gr.sphere('s1_near')
-- s1:scale(1,1,1)
-- s1:translate(1, 0.0, -8)
-- s1:set_material(white_refract)
-- scene:add_child(s1)

s2 = gr.sphere('s2_near')
s2:scale(2,2,2)
s2:translate(1, 0, -28)
s2:set_material(green)
scene:add_child(s2)

-- cow = gr.mesh('cow', 'cow.obj')
-- cow:set_material(slightDarkbrown)
-- cow:scale(0.5,0.5,0.5)
-- cow:rotate('Y', 60)
-- cow:translate(-1, -1.8, -10)
-- scene:add_child(cow)

-- helmet = gr.mesh('head', './kabutack/head.obj')
-- helmet:set_material(pearlWhite)
-- helmet:scale(0.5,0.5,0.5)
-- helmet:translate(1, -3.2, -7)
-- scene:add_child(helmet)

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

-- leftWall = gr.cube('left')
-- leftWall:scale(1, 6, 24)
-- leftWall:set_material(green)
-- leftWall:translate(-4, -3, -28)
-- walls:add_child(leftWall)

-- rightWall = gr.cube('right')
-- rightWall:scale(1, 6, 24)
-- rightWall:set_material(red)
-- rightWall:translate(3, -3, -28)
-- walls:add_child(rightWall)



-- The lights
l1 = gr.light({-30,5,40}, {0.9, 0.9, 0.9}, {1, 0, 0})
-- l2 = gr.light({1, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})
-- l3 = gr.light({30, 30, 20}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene, 'sample.png', 512, 512, 
	  {0, 0, 0,}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1, l3})

