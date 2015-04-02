mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 5, 0, 0, 0)

wall_mat = gr.material({0.765, 0.705, 0.482}, {0.5, 0.7, 0.5}, 0, 0, 0, 0)
desk_mat = gr.material({0.713, 0.608, 0.298}, {0.3, 0.3, 0.3}, 0, 0, 0, 0)

scene_root = gr.node('root')

s1 = gr.cone('s1', {-1, 0, 1}, {0, 0, 0}, 10, 4)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.cylinder('s2', {0.5, 0, 1}, {2, -2, 0}, 10, 2)
scene_root:add_child(s2)
s2:set_material(mat2)

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
wall:translate(0, 0, -10)
wall:scale(30, 30, 30)

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
desk:translate(0, -10, 0)
desk:scale(30, 30, 30)


white_light_1 = gr.light({0, 20.0, 20.0}, {0.5, 0.5, 0.5}, {2, 0, 0})
white_light_2 = gr.light({20.0, 0.0, 20.0}, {0.5, 0.5, 0.5}, {2, 0, 0})
area_light = gr.area_light({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0, {0.0, 0.0, 0.0}, 0, {0.0, 0.0, 0.0}, {0, 0, 0}) 

gr.render(scene_root, 'cone_cylinder.png', 512, 512,
	  {0, 0, 50}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light_1, white_light_2}, {area_light})
