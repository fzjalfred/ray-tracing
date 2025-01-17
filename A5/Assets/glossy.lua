-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, "", 0.9)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, "reflect", 0.6)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, "")
refract = gr.material({0.3, 0.6, 0.6}, {0.5, 0.7, 0.5}, 35, "glossy", 0.1, "reflect", 0, "refract", 0.6)
lightBlue = gr.material({0.3, 0.6, 0.6}, {0.5, 0.7, 0.5}, 35, "", 0.6)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
scene_root:add_child(s1)
s1:set_material(mat1)

-- s5 = gr.nh_sphere('s1', {-100, 0, 300}, 100)
s5 = gr.mesh( 'dodec', 'cube.obj' )
s5:scale(100, 100, 100)
s5:translate(-200, -40, 0)
scene_root:add_child(s5)
s5:set_material(mat1)

refract_s = gr.nh_sphere('refract_s', {-50, 20, 300}, 75)
scene_root:add_child(refract_s)
refract_s:set_material(refract)

s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
scene_root:add_child(s2)
s2:set_material(mat1)

s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(mat2)

s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
scene_root:add_child(s4)
s4:set_material(mat3)

s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat1)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

scene_root:translate(-20, 0, 0)
-- s2:translate(400, 0, 0)
-- s4:translate(800, 0, 0)

gr.render(scene_root, 'glossy.png', 512, 512,
	  {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light})
