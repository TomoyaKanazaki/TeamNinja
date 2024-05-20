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
 -1.76069;0.18416;6.70099;,
 5.25264;0.18416;4.14144;,
 5.52884;11.03410;4.14144;,
 -0.22708;15.04111;6.70099;,
 5.25264;0.18416;-4.14144;,
 5.52885;11.03410;-4.14144;,
 -1.76068;0.18416;-6.70099;,
 -0.22708;15.04111;-6.70099;,
 -6.09517;0.18416;0.00000;,
 -3.78444;17.51758;0.00000;,
 -1.76069;0.18416;6.70099;,
 -0.22708;15.04111;6.70099;,
 5.25264;0.18416;4.14144;,
 -1.76069;0.18416;6.70099;,
 0.17776;0.18416;0.00000;,
 5.25264;0.18416;-4.14144;,
 -1.76068;0.18416;-6.70099;,
 -6.09517;0.18416;0.00000;,
 -0.22708;15.04111;6.70099;,
 5.52884;11.03410;4.14144;,
 1.36383;13.93360;0.00000;,
 5.52885;11.03410;-4.14144;,
 -0.22708;15.04111;-6.70099;,
 -3.78444;17.51758;0.00000;;
 
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
  1;
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
 }
 MeshNormals {
  12;
  0.362990;-0.024135;0.931480;,
  0.999676;-0.025449;0.000001;,
  0.362990;-0.024135;-0.931480;,
  -0.848233;0.100490;-0.520003;,
  0.798351;0.602193;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.571342;0.820712;0.000000;,
  0.798351;0.602193;0.000001;,
  -0.848233;0.100490;0.520002;,
  0.571342;0.820712;0.000001;,
  0.571342;0.820712;-0.000001;,
  0.571341;0.820713;0.000000;;
  15;
  4;0,0,0,0;,
  4;1,1,4,7;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;8,8,8,8;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;9,7,6;,
  3;7,4,6;,
  3;4,10,6;,
  3;10,11,6;,
  3;11,9,6;;
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
  0.373590;0.935590;,
  0.548280;0.992350;,
  0.500000;0.850000;,
  0.373590;0.751910;,
  0.548280;0.695150;,
  0.656250;0.843750;,
  0.548280;0.007650;,
  0.373590;0.064410;,
  0.500000;0.162500;,
  0.373590;0.248090;,
  0.548280;0.304850;,
  0.656250;0.156250;;
 }
}
