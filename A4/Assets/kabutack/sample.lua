-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

red = gr.material({0.65, 0.05, 0.05}, {0.1, 0.1, 0.1}, 10)
white = gr.material({0.73,0.73, 0.73}, {0.5, 0.5, 0.5}, 10)
green = gr.material({0.12, 1.45, 0.15}, {0.1, 0.1, 0.1}, 10)

lightBlue = gr.material({0.3, 0.3, 1.0}, {0.1, 0.1, 0.1}, 10)

pearlWhite = gr.material({248/255, 246/255, 220/255}, {0.1, 0.1, 0.1}, 10)
brown = gr.material({0.8,0.5, 0.25}, {0.1, 0.1, 0.1}, 10)
darkbrown = gr.material({0.21, 0.14, 0.02}, {0.1, 0.1, 0.1}, 10)
slightDarkbrown = gr.material({0.70, 0.47, 0.23}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0,0.0, 0.0}, {0.1, 0.1, 0.1}, 10)

golden = gr.material({1.0, 0.8, 0.0}, {0.1, 0.1, 0.1}, 10)
gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)

-- Create the top level root node named 'root'.
scene = gr.node('root')
scene:rotate('X', 5)

plane = gr.mesh( 'plane', '../plane.obj' )
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)
plane:translate(0, 0, -15)

walls = gr.node("walls")

-- leftWall = gr.cube("leftWall")
-- leftWall:set_material(green)
-- leftWall:scale(1, 100, 100)
-- leftWall:translate(-50, 0, 0)

backWall = gr.cube("backWall")
backWall:set_material(white)
backWall:scale(200, 200, 1)
backWall:translate(-50, -50, -1000)

-- walls:add_child(leftWall)
walls:add_child(backWall)
scene:add_child(walls)



white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
-- magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene, 'sample.png', 256, 256,
	  {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light})
