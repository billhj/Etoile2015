# Etoile2015

architecture of Etoile: Components based 3d system.  each component is designed for one functionality

build: sln, project documents, for example  vs2010


core: our original code
core/math: math lib, but may need to change to use Eigen Math lib, later.
core/geometry: basic scene component, different algorithms: camera, tranformation, munipulator, frustum culling, etc
               scene is composed by entitys,  entity has different components(rendering object, geometry, etc).
			   in real time, the render objects will be sent to renderer in render list based on the frustum culling (AABB test), only the object in the render list will be rendered, 
			   the other components are designed for different managers for different purposes.
			   
auxiliary: code with external libs dependence

extern: external libs

lib: compiled libs

bin: executable

USING_GLEW macro is added in visual studio when developping using glew


