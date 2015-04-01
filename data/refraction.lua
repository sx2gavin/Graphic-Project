
mat1 = gr.material({0.0, 0.0, 0.0}, {0.8, 0.8, 0.8}, 25, 1.0, 1.0, 1.53)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 5, 0.5, 0, 0)
-- mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 5)

wall_mat = gr.material({0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, 0, 0, 0, 0)
desk_mat = gr.material({0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, 5, 0.2, 0, 0)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {5, 0, 20}, 5)
scene_root:add_child(s1)
s1:set_material(mat1)

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
wall:scale(40, 30, 30)
wall:add_texture('brick_wall_texture.png', {
		{1600, 0, 0},
		{0, 0, 0},
		{0, 1200, 0},
		{1600, 1200, 0}})

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
desk:scale(40, 1, 50)
desk:add_texture('wood_texture.png', {
			{0, 0, 0},
			{1920, 0, 0},
			{1920, 1200, 0},
			{0, 1200, 0}})
			

-- white_light_1 = gr.light({0, 100.0, 100.0}, {0.9, 0.9, 0.9}, {4, 0, 0})
white_light_2 = gr.light({100.0, 100.0, 0.0}, {0.9, 0.9, 0.9}, {2, 0, 0})

area_light_1 = gr.area_light({100.0, 100.0, 0.0}, {-50.0, 0, 50.0},20, {50.0, -50.0, 50.0},20, {0.0, 0.0, 0.0}, {2, 0, 0}) 

gr.render(scene_root, 'refraction.png', 1024, 1024,
	  {0, 0, 100}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light_2}, {area_light_1})
