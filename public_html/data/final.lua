
require('readobj')

m_sphere = gr.material({0.5, 0.0, 0.0}, {0.8, 0.8, 0.8}, 25, 0.2, 1.0, 1.52)
m_cone = gr.material({0.0, 0.5, 0.0}, {0.5, 0.7, 0.5}, 25, 0.2, 1.0, 1.52)
m_cylinder = gr.material({0.0, 0.0, 0.5}, {0.5, 0.7, 0.5}, 25, 0.2, 1.0, 1.52)
m_cube = gr.material({0.5, 0.5, 0.0}, {0.5, 0.7, 0.5}, 25, 0.2, 1.0, 1.52)
m_objects = gr.material({0.0, 0.0, 0.0}, {0.5, 0.7, 0.5}, 25, 1.0, 0.0, 0.0)

wall_mat = gr.material({0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, 0, 0, 0, 0)
desk_mat = gr.material({0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, 5, 0.2, 0, 0)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {20, -5, 20}, 5)
scene_root:add_child(s1)
s1:set_material(m_sphere)

s2 = gr.cone('s2', {0, -1, 0}, {10, 0, 20}, 10, 5)
scene_root:add_child(s2)
s2:set_material(m_cone)

s3 = gr.cylinder('s3', {0, -1, 0}, {0, 0, 20}, 10, 3)
scene_root:add_child(s3)
s3:set_material(m_cylinder)

s4 = gr.nh_box('s4', {-13, -10, 20}, 7)
scene_root:add_child(s4)
s4:set_material(m_cube)

require('mickey')
scene_root:add_child(mickey)
mickey:set_material(m_objects)
mickey:translate(-5, -1, 20)
mickey:scale(10, 10, 10)
mickey:rotate('y', -90)
mickey:rotate('x', -90)

cow = gr.mesh('cow', readobj('cow.obj'))
factor = 2;
scene_root:add_child(cow)
cow:set_material(m_objects)
cow:translate(-23, -3, 20)
cow:rotate('y', -45)
cow:scale(factor, factor, factor);

wall = gr.mesh('wall', {
		{1, 1, 0},
		{-1, 1, 0},
		{-1, -1, 0},
		{1, -1, 0}
	}, {
		{0, 1, 2, 3}
	})
scene_root:add_child(wall)
wall:set_material(wall_mat) 
wall:translate(0, 20, -10)
wall:scale(60, 30, 30)
wall:add_texture('brick_wall_texture.png', {
		{1600, 0, 0},
		{0, 0, 0},
		{0, 1200, 0},
		{1600, 1200, 0}})

wall:add_bump('wall_bump.png', { 
		{1024, 0, 0},
		{0, 0, 0},
		{0, 1024, 0},
		{1024, 1024, 0}})

desk = gr.mesh('desk', {
		   { -1, 0, -1 }, 
		   {  1, 0, -1 }, 
		   {  1,  0, 1 }, 
		   { -1, 0, 1  }
		}, {
		   {3, 2, 1, 0}
		})
scene_root:add_child(desk)
desk:set_material(desk_mat)
desk:translate(0, -10, 30)
desk:scale(60, 1, 50)
desk:add_texture('wood_texture.png', {
			{0, 0, 0},
			{1920, 0, 0},
			{1920, 1200, 0},
			{0, 1200, 0}})
			

-- white_light_1 = gr.light({0, 100.0, 100.0}, {0.9, 0.9, 0.9}, {4, 0, 0})
white_light_2 = gr.light({100.0, 100.0, 100.0}, {0.9, 0.9, 0.9}, {2, 0, 0})

area_light_1 = gr.area_light({100.0, 100.0, 100.0}, {-1.0, 0, 1.0}, 50, {1.0, -1.0, 1.0}, 50, {0.0, 0.0, 0.0}, {2, 0, 0}) 

gr.render(scene_root, 'final.png', 1920, 1080,
	  {0, 0, 100}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light_2}, {area_light_1})
