////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_obj_waveinfo.hpp ----------
/*!
\file s1_model_obj_waveinfo.hpp
\brief Outline description of the OBJ geometry definition file format
\author Gareth Edwards

OBJ (or .OBJ) is a geometry definition file format first developed by
Wavefront Technologies for its Advanced Visualizer animation package.

The file format is open and has been adopted by other 3D graphics
application vendors.

The OBJ file format is a simple data-format that represents 3D geometry
alone — namely, the position of each vertex, the UV position of each
texture coordinate vertex, vertex normals, and the faces that make
each polygon defined as a list of vertices, and texture vertices.

Vertices are stored in a counter-clockwise order by default, making
explicit declaration of face normals unnecessary. OBJ coordinates have
no units, but OBJ files can contain scale information in a human
readable comment line.

*/


/*

File Format
===========

Lines beginning with a hash character(#) are comments.

# this is a comment
An OBJ file may contain vertex data, free - form curve / surface
attributes, elements, free - form curve / surface body statements,
connectivity between free - form surfaces, grouping and display /
render attribute information.The most common elements are geometric
vertices, texture coordinates, vertex normals and polygonal faces :


# List of geometric vertices, with(x, y, z[, w]) coordinates,
w is optional and defaults to 1.0.

v 0.123 0.234 0.345 1.0
v ...
...

# List of texture coordinates, in(u, [v, w]) coordinates, these
will vary between 0 and 1, v and w are optional and default to 0.
vt 0.500 1[0]

vt ...
...


# List of vertex normals in(x, y, z) form; normals might not be
unit vectors.
vn 0.707 0.000 0.707
vn ...
...


# Parameter space vertices in(u[, v][, w]) form; free form geometry
statement

vp 0.310000 3.210000 2.100000
vp ...
...


# Polygonal face element

f 1 2 3
f 3 / 1 4 / 2 5 / 3
f 6 / 4 / 1 3 / 5 / 3 7 / 6 / 5
f 7//1 8//2 9//3
f ...
...


# Line element

l 5 8 1 2 4 9


# Point elements

p 5 8 1 2 4 9


Geometric vertex
================

A vertex can be specified in a line starting with the letter v.
That is followed by(x, y, z[, w]) coordinates.W is optional and
defaults to 1.0.Some applications support vertex colors, by putting
red, green and blue values after x y and z.

The color values range from 0 to 1.[1]


Parameter space vertices
========================
A free - form geometry statement can be specified in a line starting
with the string vp.Define points in parameter space of a curve or
surface.u only is required for curve points, u and v for surface
points and control points of non - rational trimming curves, and
u, v and w(weight) for control points of rational trimming curves.


Face elements
=============
Faces are defined using lists of vertex, texture and normal indices
which start at 1. Polygons such as quadrilaterals can be defined by
using more than three vertex / texture / normal indices.


OBJ files also support free - form geometry which use curves and
surfaces to define objects, such as NURBS surfaces.


Vertex indices
==============
A valid vertex index matches the corresponding vertex elements of a
previously defined vertex list.If an index is positive then it refers
to the offset in that vertex list, starting at 1. If an index is
negative then it relatively refers to the end of the vertex list,
-1 referring to the last element.

Each face can contain three or more vertices.

f v1 v2 v3 ....


Vertex texture coordinate indices
=================================
Optionally, texture coordinate indices can be used to specify
texture coordinates when defining a face.To add a texture coordinate
index to a vertex index when defining a face, one must put a slash
immediately after the vertex index and then put the texture coordinate
index.No spaces are permitted before or after the slash.

A valid texture coordinate index starts from 1 and matches the
corresponding element in the previously defined list of texture
coordinates.Each face can contain three or more elements.

f v1 / vt1 v2 / vt2 v3 / vt3 ...


Vertex normal indices
=====================
Optionally, normal indices can be used to specify normal vectors for
vertices when defining a face.To add a normal index to a vertex index
when defining a face, one must put a second slash after the texture
coordinate index and then put the normal index.A valid normal index
starts from 1 and matches the corresponding element in the previously
defined list of normals.Each face can contain three or more elements.

f v1 / vt1 / vn1 v2 / vt2 / vn2 v3 / vt3 / vn3 ...


Vertex normal indices without texture coordinate indices
========================================================
As texture coordinates are optional, one can define geometry without
them, but one must put two slashes after the vertex index before
putting the normal index.

f v1//vn1 v2//vn2 v3//vn3 ...


Line elements
=============
Records starting with the letter "l" specify the order of the
vertices which build a polyline.

l v1 v2 v3 v4 v5 v6 ...


Other geometry formats
======================
Obj files support higher - order surfaces using several different
kinds of interpolation, such as Taylor and B - splines, although
support for those features in third party file readers is far from
universal.Obj files also do not support mesh hierarchies or any
kind of animation or deformation, such as vertex skinning or mesh
morphing.


Referencing materials
=====================
Materials that describe the visual aspects of the polygons are
stored in external.mtl files.More than one external MTL material
file may be referenced from within the OBJ file.The.mtl file may
contain one or more named material definitions.

mtllib[external.mtl file name]
...

This tag specifies the material name for the element following it.
The material name matches a named material definition in an
external.mtl file.

usemtl[material name]
...

Named objects and polygon groups are specified via the following tags.

o[object name]
...

g[group name]
...

Smooth shading across polygons is enabled by smoothing groups.

s 1
...

# Smooth shading can be disabled as well.
s off
...


Relative and absolute indices
=============================
OBJ files, due to their list structure, are able to reference vertices,
normals, etc.either by their absolute position(1 represents the first
defined vertex, N representing the Nth defined vertex), or by their
relative position(-1 represents the latest defined vertex).However,
not all software supports the latter approach, and conversely some
software inherently writes only the latter form(due to the convenience
of appending elements without needing to recalculate vertex offsets,
etc.), leading to occasional incompatibilities.


*/