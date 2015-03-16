-- A polygonal approximation to a cylinder

mat1 = gr.material({1.0, 1.0, 1.0}, {0.5, 0.7, 0.5}, 5)
mat2 = gr.material({0.0, 1.0, 0.0}, {0.5, 0.7, 0.5}, 50)
mat3 = gr.material({0.0, 0.0, 1.0}, {0.5, 0.7, 0.5}, 5)

scene_root = gr.node('root')

-- triangle = gr.mesh('triangle', { {0.0000, 0.0000, 0.0000},
-- 								 {0.0000, 1.0000, 0.0000},
-- 								 {1.0000, 0.0000, 0.0000}},
-- 							   { {0, 1, 2}})
-- scene_root:add_child(triangle);
-- triangle:set_material(mat1);
-- 
-- 
-- triangle_2 = gr.mesh('triangle_2', { {0.0000, 0.0000, 1.0000},
-- 								 {0.0000, 0.5000, 1.0000},
-- 								 {0.5000, 0.0000, 1.0000}},
-- 							   { {0, 1, 2}})
-- 
-- scene_root:add_child(triangle_2);
-- triangle_2:set_material(mat2);

s1 = gr.sphere('s1')
scene_root:add_child(s1)
s1:set_material(mat1)
s1:scale(0.5, 0.5, 0.5);

s4 = gr.nh_sphere('s4', {1, 0, -1}, 1)
scene_root:add_child(s4)
s4:set_material(mat3)

s2 = gr.cube('s2')
scene_root:add_child(s2)
s2:set_material(mat2)
s2:translate( 1, 0, 0)
s2:scale(2, 0.5, 1)

s3 = gr.nh_box('s3', {0, -1, 1}, 0.25)
scene_root:add_child(s3)
s3:set_material(mat2)

white_light = gr.light({-6.0, 0.0, 6.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({6.0, 0.0, 6.0}, {1.0, 0.0, 0.0}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 500, 500, {0, 0, 7}, {0, 0, -1}, {0, 1, 0}, 50, {0.1, 0.1, 0.1}, {orange_light, white_light}) 
