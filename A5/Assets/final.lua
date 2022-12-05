-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

red = gr.material({0.65, 0.05, 0.05}, {0.8, 0.8, 0.8}, 25, "")
golden = gr.material({1.0, 0.8, 0.0}, {0.8, 0.8, 0.8}, 25, "")



green = gr.material({0.12, 0.45, 0.15}, {0.1, 0.1, 0.1}, 0, "")

lightBlue = gr.material({0.3, 0.3, 1.0}, {0.1, 0.1, 0.1}, 10, "", 0.8)

waterBlue = gr.material({0.3, 0.3, 1.0}, {0.1, 0.1, 0.1}, 10, "glossy", 0.1)

pearlWhite = gr.material({248/255, 246/255, 220/255}, {0.1, 0.1, 0.1}, 10, "")
brown = gr.material({0.8,0.5, 0.25}, {0.1, 0.1, 0.1}, 10, "")
darkbrown = gr.material({0.21, 0.14, 0.02}, {0.1, 0.1, 0.1}, 10, "")
slightDarkbrown = gr.material({0.70, 0.47, 0.23}, {0.1, 0.1, 0.1}, 10, "")
black = gr.material({0.0,0.0, 0.0}, {0.1, 0.1, 0.1}, 10, "")


gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25, "")
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, "")

scene = gr.node('scene')


hanger_color = gr.material({0.7,0.6, 0.5}, {0.8, 0.8, 0.8}, 1, "")
--  Texture Materials
-- ========================================
japan_wood = gr.material({0.8, 0.8, 0.4}, {0.8, 0.8, 0.8}, 10, "")
gr.texture(japan_wood, "texture/JapanWood.jpg")

bath_wall_tile = gr.material({0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 30, "")
gr.texture(bath_wall_tile, "texture/wall.jpg")

fuji_paint = gr.material({0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10, "")
gr.texture(fuji_paint, "texture/mt_fuji.jpg")

plane_tile = gr.material({0.8, 0.8, 0.6}, {0.8, 0.8, 0.8}, 30, "")
gr.texture(plane_tile, "texture/JapanWood.jpg")
metal_armor = gr.material({0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 30, "")
gr.texture(metal_armor, "texture/metal.jpg")



--  kabutack model
-- ========================================

kabutack = gr.node('kabutack')
kabutack:rotate('x', -18)
kabutack:rotate('y', -30)
kabutack:translate(4.4,-2.7,-13)
-- kabutack:translate(5, 2, -14)
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

leftArm = gr.mesh('leftArm', './kabutack/leftArm.obj')
leftArm:set_material(black)
kabutack:add_child(leftArm)    
leftforearm = gr.mesh('leftforearm', './kabutack/leftforearm.obj')
leftforearm:set_material(red)
kabutack:add_child(leftforearm)    
lefthand = gr.mesh('lefthand', './kabutack/lefthand.obj')
lefthand:set_material(black)
kabutack:add_child(lefthand)    

rightArm = gr.mesh('rightArm', './kabutack/rightArm.obj')
rightArm:set_material(black)
kabutack:add_child(rightArm)    
rightforearm = gr.mesh('rightforearm', './kabutack/rightforearm.obj')
rightforearm:set_material(red)
kabutack:add_child(rightforearm)    
righthand = gr.mesh('righthand', './kabutack/righthand.obj')
righthand:set_material(black)
kabutack:add_child(righthand)    

-- ========================================


-- the floor
plane = gr.mesh( 'plane', './kabutack/floor.obj' )
scene:add_child(plane)
plane:set_material(plane_tile)
plane:scale(20, 20, 20)
plane:rotate('x', -5)
plane:translate(0, -4, -15)

bathhouse = gr.node('bathhouse')
bathhouse:translate(1.7,-3,-10)
scene:add_child(bathhouse)

fences = gr.mesh('fences', './kabutack/fences.obj')
fences:set_material(japan_wood)
bathhouse:add_child(fences)

water = gr.mesh('water', './kabutack/water.obj')
water:set_material(waterBlue)
bathhouse:add_child(water)    

mtfuji = gr.mesh('water', './kabutack/mtfuji.obj')
mtfuji:translate(0.4,-2,0)
mtfuji:scale(1.05, 1, 1)
mtfuji:set_material(fuji_paint)
bathhouse:add_child(mtfuji)    

bath_wall = gr.mesh('wall', './kabutack/wall.obj')
bath_wall:set_material(bath_wall_tile)
bath_wall:translate(0.4,-1,0)
bathhouse:add_child(bath_wall)  

door_hanger = gr.node('door_hanger')
door_hanger:translate(-5.5, 8, 3)
bathhouse:add_child(door_hanger)

paper_white = gr.material({0.8,0.8, 0.8}, {0.5, 0.5, 0.5}, 15, "lamb")
gr.texture(paper_white, "texture/signature.jpg")
rope1 = gr.mesh('rope', './kabutack/rope.obj')
rope1:translate(0,1.7,0)
rope1:set_material(black)
door_hanger:add_child(rope1)
lightblue_refract = gr.material({0.3, 0.3, 0.6}, {0.3, 0.4, 0.3}, 15, "refract", 0.8)
s = gr.sphere('s')
s:set_material(lightblue_refract)
s:scale(0.7, 0.7, 0.7)
s:translate(0, -2.8, 0) 
door_hanger:add_child(s)
paper = gr.mesh('paper', './kabutack/paper.obj')
paper:translate(0, 0.18, 0)
paper:set_material(paper_white)
door_hanger:add_child(paper) 

drink = gr.mesh('drink', './kabutack/drink.obj')
drink:set_material(darkbrown)
bathhouse:add_child(drink)   

drink1 = gr.mesh('drink', './kabutack/drink1.obj')
drink1:translate(-2,0,0)
drink1:set_material(golden)
bathhouse:add_child(drink1)   

hanger = gr.node('hanger')
hanger:translate(-7.4, 0.3, -3)
bathhouse:add_child(hanger)

torso_hanging = gr.mesh('torso_hang', './kabutack/torso.obj')
torso_hanging:scale(1,1,1)
torso_hanging:set_material(metal_armor)
hanger:add_child(torso_hanging)

helmet_hanging = gr.mesh('head', './kabutack/head.obj')
helmet_hanging:scale(1,1,1)
helmet_hanging:set_material(metal_armor)
helmet_hanging:translate(0, 0.16, 0)
hanger:add_child(helmet_hanging)

armor_hanger = gr.mesh('armor_hanger', './kabutack/hanger.obj')
armor_hanger:set_material(hanger_color)
hanger:add_child(armor_hanger)

silver = gr.material({0.5,0.5, 0.5}, {0.6,0.6,0.6}, 25, "reflect", 0.7)
rightWall = gr.cube('right')
rightWall:scale(1, 10, 12)
rightWall:set_material(silver)
rightWall:translate(6, 0, -12)
bathhouse:add_child(rightWall)



-- The lights
l1 = gr.light({-20,20,27}, {0.8, 0.8, 0.8}, {1, 0, 0})
l2 = gr.light({-20, 0, 20}, {0.8, 0.7, 0.6}, {1, 0, 0})
-- l3 = gr.light({0, 0, 200}, {0.8, 0.8, 0.8}, {1, 0, 0})


--  4:3
-- 1024x768
-- 512x384
gr.render(scene, 'final.png', 1024, 768, 
	  {0, 2, 0}, {0, -0.3, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1})

