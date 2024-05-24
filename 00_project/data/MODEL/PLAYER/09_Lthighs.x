xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 24;
 -3.74900;-16.49959;8.04100;,
 4.66700;-16.49959;4.96954;,
 4.99844;-3.47967;4.96954;,
 -1.90867;1.32874;8.04100;,
 4.66700;-16.49959;-4.96992;,
 4.99845;-3.47967;-4.96992;,
 -3.74900;-16.49959;-8.04139;,
 -1.90867;1.32874;-8.04139;,
 -8.95037;-16.49959;-0.00019;,
 -6.17750;4.30050;-0.00019;,
 -3.74900;-16.49959;8.04100;,
 -1.90867;1.32874;8.04100;,
 4.66700;-16.49959;4.96954;,
 -3.74900;-16.49959;8.04100;,
 -1.42287;-16.49959;-0.00019;,
 4.66700;-16.49959;-4.96992;,
 -3.74900;-16.49959;-8.04139;,
 -8.95037;-16.49959;-0.00019;,
 -1.90867;1.32874;8.04100;,
 4.99844;-3.47967;4.96954;,
 0.00041;-0.00027;-0.00019;,
 4.99845;-3.47967;-4.96992;,
 -1.90867;1.32874;-8.04139;,
 -6.17750;4.30050;-0.00019;;
 
 15;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 3;12,13,14;,
 3;15,12,14;,
 3;16,15,14;,
 3;17,16,14;,
 3;13,17,14;,
 3;18,19,20;,
 3;19,21,20;,
 3;21,22,20;,
 3;22,23,20;,
 3;23,18,20;;
 
 MeshMaterialList {
  3;
  15;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.097255;0.150588;0.279216;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.008000;0.008000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  16;
  0.362990;-0.024135;0.931480;,
  0.999676;-0.025448;-0.000000;,
  0.362990;-0.024135;-0.931480;,
  -0.848233;0.100490;-0.520002;,
  0.798350;0.602193;-0.000000;,
  -0.000000;-1.000000;0.000000;,
  0.571342;0.820712;-0.000000;,
  0.798350;0.602193;0.000000;,
  -0.848233;0.100490;0.520002;,
  -0.000000;-1.000000;0.000000;,
  -0.000000;-1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-1.000000;0.000000;,
  0.571342;0.820712;-0.000000;,
  0.571342;0.820712;-0.000000;,
  0.571342;0.820712;-0.000000;;
  15;
  4;0,0,0,0;,
  4;1,1,4,7;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;8,8,8,8;,
  3;9,10,5;,
  3;11,9,5;,
  3;11,11,5;,
  3;12,11,5;,
  3;10,12,5;,
  3;13,7,6;,
  3;7,4,6;,
  3;4,14,6;,
  3;14,15,6;,
  3;15,13,6;;
 }
 MeshTextureCoords {
  24;
  0.375000;0.687500;,
  0.425000;0.687500;,
  0.425000;0.311560;,
  0.375000;0.311560;,
  0.475000;0.687500;,
  0.475000;0.311560;,
  0.525000;0.687500;,
  0.525000;0.311560;,
  0.575000;0.687500;,
  0.575000;0.311560;,
  0.625000;0.687500;,
  0.625000;0.311560;,
  0.373591;0.935591;,
  0.548284;0.992353;,
  0.500000;0.850000;,
  0.373591;0.751909;,
  0.548284;0.695147;,
  0.656250;0.843750;,
  0.548284;0.007647;,
  0.373591;0.064409;,
  0.500000;0.162500;,
  0.373591;0.248091;,
  0.548284;0.304853;,
  0.656250;0.156250;;
 }
}
