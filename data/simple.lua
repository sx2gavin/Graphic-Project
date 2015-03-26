-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.5, 0, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 5, 0.1, 0, 0)
-- mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 5)

wall_mat = gr.material({0.765, 0.705, 0.482}, {0.5, 0.7, 0.5}, 0, 0, 0, 0)
desk_mat = gr.material({0.713, 0.608, 0.298}, {0.3, 0.3, 0.3}, 5, 0.2, 0, 0)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {-10, 0, 0}, 10)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.nh_sphere('s2', {10, 0, 0}, 10)
scene_root:add_child(s2)
s2:set_material(mat2)

s3 = gr.nh_sphere('s3', {0, -7, 30}, 3)
scene_root:add_child(s3)
s3:set_material(mat2)

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


white_light_1 = gr.light({0, 100.0, 100.0}, {0.9, 0.9, 0.9}, {4, 0, 0})
white_light_2 = gr.light({100.0, 0.0, 100.0}, {0.9, 0.9, 0.9}, {4, 0, 0})

gr.render(scene_root, 'simple.png', 1024, 1024,
	  {0, 0, 100}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light_1, white_light_2})
